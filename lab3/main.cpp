#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <semaphore>
#include <chrono>
#include <sstream>
#include <string>
#include <queue>

#define M 10

template <class T, int N>
struct SyncQueue
{
    std::binary_semaphore mutex{1};
    std::counting_semaphore<N> full{0};
    std::counting_semaphore<N> empty{N};
    std::queue<T> queue;
    std::string name;
};

std::binary_semaphore stdout_mutex(1);
std::chrono::milliseconds consumer_timeout(100);
int max_produced;

template <class T, int N>
void run_producer(
    std::string name,
    T value,
    std::initializer_list<SyncQueue<T, N> *> buffers)
{
    for (int i = 0; max_produced <= 0 || i < max_produced; i++)
    {
        for (auto &b : buffers)
        {
            b->empty.acquire();
            b->mutex.acquire();
            stdout_mutex.acquire();

            b->queue.push(value);
            std::cout << "Producer '" << name
                      << "' produced  to  queue '" << b->name
                      << "' value '" << value << "'. "
                      << "Queue has now "
                      << b->queue.size() << "/" << M
                      << " places occupied.\n";

            stdout_mutex.release();
            b->mutex.release();
            b->full.release();
        }
    }
}

template <class T, int N>
void run_consumer(
    std::string name,
    SyncQueue<T, N> &buffer)
{
    while (true)
    {
        buffer.full.acquire();
        buffer.mutex.acquire();
        stdout_mutex.acquire();

        T value = buffer.queue.front();
        buffer.queue.pop();
        std::cout << "Consumer '" << name
                  << "' consumed from queue '" << buffer.name
                  << "' value '" << value << "'. "
                  << "Queue has now "
                  << buffer.queue.size() << "/" << M
                  << " places occupied.\n";

        stdout_mutex.release();
        buffer.mutex.release();
        buffer.empty.release();
    }
}

int main(int argc, char **argv)
{
    std::stringstream arg;
    switch (argc)
    {
    case 1:
        max_produced = 0;
        break;
    case 2:
        arg = std::stringstream(argv[1]);
        arg >> max_produced;
        if (!arg)
        {
            std::cerr << "Invalid max_produced: " << max_produced << "\n";
            return 1;
        }
        break;

    default:
        std::cerr << "Usage: " << argv[0] << " [max_produced]"
                  << "\n";
        std::cerr << "Arguments:\n";
        std::cerr << "\tmax_produced: maximum number of items "
                  << "each producer can produce to each queue "
                  << "(default=infinity)\n";
        return 2;
    }

    SyncQueue<int, M> q0, q1, q2;
    q0.name = "q0";
    q1.name = "q1";
    q2.name = "q2";

    std::thread p0([&]()
                   { run_producer<int, M>("p0", 0, {&q0, &q1}); });
    std::thread p1([&]()
                   { run_producer<int, M>("p1", 1, {&q1, &q2}); });
    std::thread c0([&]()
                   { run_consumer<int, M>("c0", q0); });
    std::thread c1([&]()
                   { run_consumer<int, M>("c1", q1); });
    std::thread c2([&]()
                   { run_consumer<int, M>("c2", q2); });

    p0.join();
    p1.join();

    std::this_thread::sleep_for(consumer_timeout);

    c0.detach();
    c1.detach();
    c2.detach();

    return 0;
}