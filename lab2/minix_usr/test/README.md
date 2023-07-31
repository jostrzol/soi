# Testy
## Kompilacja
`make`

## Zmiana proporcji czasu wykonywania dla grup
`./setpercenta <PERCENTA>`

## Test inicjalizacji grupy
`./group <dla A: 0, dla B:1>`  
Wyświetli się grupa procesu zainicjowana przez system, a następnie grupa procesu przydzielona na żądanie procesu.  
Nastpępnie wykona się pracochłonne zadanie (nieważne dla tego testu, można przerwać `ctrl + c`).  
Po kilkukrotnym włączeniu tego programu powinien być widoczny wzór, w jakim system inicjuje nowym procesom grupy.

## Test współbieżności 1
`./test`  
Test włączy 2 instancje programu `group` - najpierw z grupą A (0), a potem B (1). Procesy będą wykonywać się równolegle raportując co jakiś czas swój postęp, a po ich skończeniu pojawi się ilość czasu wykonywania się procesu.  
Niestety jest to czas spędzony w procesie, a nie czas od włączenia procesu do zakończenia procesu, dlatego czas ten będzie podobny w obu przypadkach. Dla czasu absolutnego Minix ma domyślnie rozdzielczość aż 1s, dlatego zdecydowałem się na nie używanie go.  
Przy ustawianiu `PERCENTA` na wartości, przez które nie dzieli się 100, mogą wystąpić niepożądane, ale wyjaśnialne efekty.  
Np. dla `PERCENTA = 13` program A i tak wykona się pierwszy, ponieważ nie da nawet czasu na zainicjowanie się programu B (program `test` kilkukrotnie musi być w stanie unready, zanim zainicjuje program B, dając tym samym dużo czasu programowi A na wykonanie się).  
Problem dałoby się rozwiązać, wymuszając częstsze szeregowanie procesów. W tym rozwiązaniu ustawiłem `SCHED_RATE` na 1 (zmniejszyłem 6-krotnie), więc aby dalej zwiększyć częstotliwość szeregowania procesów, trzeba by ingerować w częstotliwość przerwań zegarowych. Przyjąłem, że wychodzi to poza zakres tego zadania laboratoryjnego w moim rozwiązaniu.

## Test współbieżności 2
`./test2.sh`  
Test uruchomi program `proc_a` oraz `proc_b`, które są podobne do programu `group`, tylko że wykonują się aż do przerwania ich sygnałem.  
Można oglądać liczbę przypisanych cykli zegarowych dla każdego procesu pod klawiszem F1, w kolumnie `user`.  
Wyniki wydają się sprzeczne z założeniami - np. dla `PERCENTA = 75` stosunek cykli A do B wynosi około 2:1, zamiast 3:1. Prawdopodobnie jest to błąd Minixa - czasami nie odnotowuje prawidłowo przypisania cyklu do procesu. Może wynikać on z tego, że w rozwiązaniu niestandardowo manipuluję kolejką w funkcji `sched`.  
Niestety nie mogłem zautomatyzować zamykania programów `proc_a` i `proc_b`, ponieważ po zmianach w jądrze przestała działać komenda `ps`.