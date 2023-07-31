<div style="padding: 2% 5%">
<h1 style="text-align: center;">
<div style="color:grey; font-size: 0.6em;">Jakub Ostrzołek</div>
<div>SOI - lab 3</div>
</h1>

## Wykorzystane technologie i biblioteki
* Linux
* C++20 (gcc 11)
* `<queue>`
* `<thread>`
* `<semaphore>`
* `<iostream>`
* `<initializer_list>`

## Plan wykonania zadania
1. Stworzenie template'u struktury `SyncQueue<class T, int N>`:
   * pole `std::queue<T>` - kolejka FIFO
   * pole `std::binary_semaphore mutex(1)` - czy kolejka jest wolna do zapisu / odczytu
   * pole `std::counting_semaphore<N> full(0)` - ilość zapełnionych miejsc w danej kolejce
   * pole `std::counting_semaphore<N> empty(N)` - ilość pustych miejsc w danej kolejce
2. Zdefiniowanie długości kolejek: `#define M 10`
3. Stworzenie 3 kolejek: `SyncQueue<int, M> q0, q1, q2`
4. Stworzenie template'u funkcji producentów `void run_producer<class T, int N> (T value, std::initializer_list<SyncQueue<T, N>> buffers)`:
   * w nieskończonej pętli:
     * dla każdego bufora `b` z `buffers`:
       * `b.empty.acquire();`
       * `b.mutex.acquire();`
       * `b.queue.push(value);`
       * `b.mutex.release();`
       * `b.full.release();`
5. Stworzenie template'u funkcji konsumentów `void run_consumer<class T, int N> (SyncQueue<T, N> buffer)`:
   * w nieskończonej pętli:
     * `buffer.full.acquire();`
     * `buffer.mutex.acquire();`
     * `T value = buffer.queue.front()`
     * `buffer.queue.pop();`
     * `buffer.mutex.release();`
     * `buffer.empty.release();`
     * wypisz `value` do stdout
6. Stworzenie wątków 2 producentów:
   * `std::thread p0 (run_producer<int, M>, 0, q0, q1)` - zapełnia na zmianę `q0` i `q1` wartościami `0`
   * `std::thread p1 (run_producer<int, M>, 1, q1, q2)` - zapełnia na zmianę `q1` i `q2` wartościami `1`
7. Stworzenie wątków 3 kosumentów:
   * `std::thread c0 (run_consumer<int, M>, q0)` - wyciąga wartość z `q0` i wypisuje ją do stdout
   * `std::thread c1 (run_consumer<int, M>, q1)` - wyciąga wartość z `q1` i wypisuje ją do stdout
   * `std::thread c2 (run_consumer<int, M>, q2)` - wyciąga wartość z `q2` i wypisuje ją do stdout
8. Uruchomienie wszystkich wątków