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

public:
    std::string name;

    void push(T item);
    T pop();
    std::size_t size();

    SyncQueue(std::string name);
};

template <class T, int N>
void SyncQueue<T, N>::push(T item)
{
    enter();
    if (queue.size() == N)
        wait(not_full);

    queue.push(item);
    std::cout << item << " -> " << name << " [ "
              << queue.size() << "/" << N
              << " ]\n";

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

    T item = queue.front();
    queue.pop();
    std::cout << item << " <- " << name << " [ "
              << queue.size() << "/" << N
              << " ]\n";

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
SyncQueue<T, N>::SyncQueue(std::string name)
    : name(name) {}
