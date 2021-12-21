#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>

#define M 10

const std::chrono::milliseconds C_TIMEOUT(100);
std::mutex stdout_mutex;
int max_produced;

template <class T, int N>
struct SyncQueue
{
    std::mutex mutex;
    sem_t full;
    sem_t empty;
    std::queue<T> queue;
    std::string name;

    SyncQueue(const std::string &name)
    {
        sem_init(&full, 0, 0);
        sem_init(&empty, 0, N);
        this->name = name;
    }
    ~SyncQueue()
    {
        sem_destroy(&full);
        sem_destroy(&empty);
    }
};

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
            sem_wait(&b->empty);
            const std::lock_guard<std::mutex>
                buffer_lock(b->mutex);
            const std::lock_guard<std::mutex> stdout_lock(stdout_mutex);

            b->queue.push(value);
            std::cout << "Producer '" << name
                      << "' produced  to  queue '" << b->name
                      << "' value '" << value << "'. "
                      << "Queue has now "
                      << b->queue.size() << "/" << M
                      << " places occupied.\n";

            sem_post(&b->full);
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
        sem_wait(&buffer.full);
        const std::lock_guard<std::mutex>
            buffer_lock(buffer.mutex);
        const std::lock_guard<std::mutex> stdout_lock(stdout_mutex);

        T value = buffer.queue.front();
        buffer.queue.pop();
        std::cout << "Consumer '" << name
                  << "' consumed from queue '" << buffer.name
                  << "' value '" << value << "'. "
                  << "Queue has now "
                  << buffer.queue.size() << "/" << M
                  << " places occupied.\n";

        sem_post(&buffer.empty);
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

    SyncQueue<int, M> q0("q0"), q1("q1"), q2("q2");

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

    std::this_thread::sleep_for(C_TIMEOUT);

    c0.detach();
    c1.detach();
    c2.detach();

    return 0;
}