<div style="padding: 2% 5%">
<h1 style="text-align: center;">
<div style="color:grey; font-size: 0.6em;">Jakub Ostrzołek</div>
<div>SOI - lab 2</div>
</h1>

## Plan wykonania zadania
### Główne kroki
* `src/kernel/proc.h`
  * \+ zmienna `int TIMES_A` wskazująca ile razy powinien wykonać się proces z grupy A
  * \+ zmienna `int TIMES_B` wskazująca ile razy powinien wykonać się proces z grupy B
  * \+ zmienna `int PROC_SCHED_COUNTER` wyznaczająca pozostałą liczbę wykonań obecnego procesu do zmiany procesu na następny w kolejce
  * \+ zmienna `int PROC_GROUP_INIT` wyznaczająca aktualną grupę dla nowych procesów 
  * \+ zmienna `int PROC_GROUP_INIT_COUNTER` wyznaczająca pozostałą liczbę procesów do zainicjowania grupą `PROC_GROUP_INIT`, przed zmianą `PROC_GROUP_INIT` na grupę przeciwną
  * `struct proc`
    * \+ pole `int group` wskazujące na grupę procesu w sposób: 0 -> A; 1 -> B
* `src/kernel/proc.c`
  * `sched()`
    * jeśli `PROC_SCHED_COUNTER == 0` - przesuń kolejkę i ustaw licznik na `TIMES_X`, gdzie `X` to grupa kolejnego procesu w kolejce
    * w przeciwnym przypadku - nie przesuwaj kolejki
* `src/kernel/system.c`
  * `do_fork()`
    * \+ inicjalizacja zmiennej `group` w procesach klasy `USER` z użyciem `PROC_GROUP_INIT` i `PROC_GROUP_INIT_COUNTER`
  * \+ `do_setpercenta(m_ptr)`
    * `int percent_a` - z parametru z `m_ptr`
    * `int percent_b = 100-percent_a`
    * `int gcd` - największa wspólna wielokrotność `percent_a` i `percent_b`
    * `TIMES_A = percent_a/gcd`
    * `TIMES_B = percent_b/gcd`
  * `systask()`
    * dodać `do_setpercenta()` do switcha zadań systemowych dla case `SYS_SETPERCENTA`
* `include/minix/com.h`
  * \+ stała `SYS_SETPERCENTA` 
* `src/kernel/main.c`
  * `main()`
    * \+ inicjalizacja zmiennych globalnych dodanych w `src/kernel/proc.h`
    * \+ inicjalizacja zmiennej `group` dla procesów tworzonych w `main()`
* \+ wywołania systemowe (na podstawie poprzedniego labolatorium):
  * `SETPERCENTA` - przekazuje kontrolę do zadania systemowego `SYS_SETPERCENTA`
  *  `SETGROUP` - zmienia grupę aktualnemu procesowi
### Opcjonalne kroki
* `kernel/clock.c`
  * \+ zmienna `int EFFECTIVE_SCHED_RATE` i podmienienie w reszcie kodu stałej `SCHED_RATE` nową zmienną
* `src/kernel/system.c`
  * `do_setpercenta(m_ptr)`
    * \+ ustawianie `EFFECTIVE_SCHED_RATE` na odpowiednie ułamki `SCHED_RATE` tak, aby nie zatykać systemu operacyjnego uruchamianiem np. 99 razy tego samego procesu z rzędu
## Testowanie
* `proc_a.c`
  * zmiana swojej grupy na A za pomocą wywołania systemowego `SETGROUP`
  * wykonanie kosztownej operacji
* `proc_b.c`
  * zmiana swojej grupy na B za pomocą wywołania systemowego `SETGROUP`
  * wykonanie tej samej kosztownej operacji co w `proc_a.c`
* kompilacja -> `proc_a`, `proc_b`
* `main.c`
  * zmierzenie czasu wykonywania się `proc_a` i `proc_b` za pomocą biblioteki `<time.h>`
  * wyświetlenie wyników