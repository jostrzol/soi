#include "sync_queue.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>

#define M 10

const std::chrono::milliseconds C_TIMEOUT(100);
int max_produced;

template <class T, int N>
void run_producer(
    T value,
    std::initializer_list<SyncQueue<T, N> *> buffers)
{
    for (int i = 0; max_produced <= 0 || i < max_produced; i++)
    {
        for (auto &b : buffers)
        {
            b->push(value);
        }
    }
}

template <class T, int N>
void run_consumer(SyncQueue<T, N> &buffer)
{
    while (true)
    {
        buffer.pop();
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
                   { run_producer<int, M>(0, {&q0, &q1}); });
    std::thread p1([&]()
                   { run_producer<int, M>(1, {&q1, &q2}); });
    std::thread c0([&]()
                   { run_consumer<int, M>(q0); });
    std::thread c1([&]()
                   { run_consumer<int, M>(q1); });
    std::thread c2([&]()
                   { run_consumer<int, M>(q2); });

    p0.join();
    p1.join();

    std::this_thread::sleep_for(C_TIMEOUT);

    c0.detach();
    c1.detach();
    c2.detach();

    return 0;
}