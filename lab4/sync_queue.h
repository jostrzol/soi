#pragma once

#include "monitor.h"
#include <queue>
#include <string>
#include <iostream>

template <class T, int N>
class SyncQueue : protected Monitor
{
private:
    Condition not_full, not_empty;
    std::queue<T> queue;
    Semaphore &stdout_mutex;

public:
    std::string name;

    void push(T item);
    T pop();
    std::size_t size();

    SyncQueue(Semaphore &stdout_mutex, std::string name);
};

template <class T, int N>
void SyncQueue<T, N>::push(T item)
{
    enter();
    if (queue.size() == N)
        wait(not_full);
    stdout_mutex.p();

    queue.push(item);
    std::cout << item << " -> " << name << " [ "
              << queue.size() << "/" << N
              << " ]\n";

    stdout_mutex.v();
    if (queue.size() == 1)
        signal(not_empty);
    leave();
}

template <class T, int N>
T SyncQueue<T, N>::pop()
{
    enter();
    if (queue.size() == 0)
        wait(not_empty);
    stdout_mutex.p();

    T item = queue.front();
    queue.pop();
    std::cout << item << " <- " << name << " [ "
              << queue.size() << "/" << N
              << " ]\n";

    stdout_mutex.v();
    if (queue.size() == N - 1)
        signal(not_full);
    leave();
    return item;
}

template <class T, int N>
std::size_t SyncQueue<T, N>::size()
{
    return queue.size();
}

template <class T, int N>
SyncQueue<T, N>::SyncQueue(Semaphore &stdout_mutex, std::string name)
    : stdout_mutex(stdout_mutex), name(name) {}
