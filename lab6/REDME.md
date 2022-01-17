<div style="padding: 2% 5%;">

<h1 style="text-align: center;">
<div style="color:grey; font-size: 0.6em;">Jakub Ostrzołek</div>
<div>SOI laboratorium 6</div>
</h1>

## Główne struktury
* `struct Superblock`
  * `char[2] magic_number` - identyfikator systemu plików
  * `unsigned int size` - rozmiar systemu plików
  * `Inode *inode_first` - wskaźnik na pierwszy Inode
  * `unsigned int inode_max` - maksymalna ilośc Inodów
  * `unsigned int inode_size` - rozmiar Inoda
  * `Datablock *datablock_first` - wskaźnik na pierwszy Datablock
  * `unsigned int datablock_max` - maksymalna ilośc Datablocków
  * `unsigned int datablock_size` - rozmiar Datablocku
* `struct Inode`
  * `char *name` - nazwa pliku
  * `unsigned int size` - rozmiar pliku
  * `Datablock *datablock_first` - wskaźnik na pierwszy datablock pliku
  * `Inode *inode_next` - wskaźnik na kolejny Inode
* `struct Datablock`
  * `void *data` - zawartość datablocku
  * `Datablock *datablock_next` - wskaźnik na kolejny datablock pliku

## Interakcja z użytkownikiem
Program `my-fs`. Komendy:
  * `create` - tworzy system plików
    * `FILEPATH` - ścieżka do pliku, w którym ma się znajdować system plików
  * `cp-to` - kopiuje do systemu plików
    * `FILESYSTEM` - ścieżka do pliku systemu plików
    * `SRC` - ścieżka pliku źródłowego
    * `DST` - ścieżka pliku docelowego
  * `cp-from` - kopiuje z systemu plików
    * `FILESYSTEM` - ścieżka do pliku systemu plików
    * `SRC` - ścieżka pliku źródłowego
    * `DST` - ścieżka pliku docelowego
  * `ls` - wyświetla katalog systemu plików
    * `FILESYSTEM` - ścieżka do pliku systemu plików
  * `rm` - usuwa plik z systemu plików
    * `FILESYSTEM` - ścieżka do pliku systemu plików
    * `FILE` - ścieżka do pliku do usunięcia
  * `info` - wyświetla informacje ile bloków danego typu jest zajętych na ile możliwych

Nie implementuję programu usuwającego system plików, bo zakładam, że można przeprowadzić taką operację z nadrzędnego systemu plików (`rm <FILESYSTEM_PATH>`)

## Fragmentacja
Planuję zaimplementować plik jako jednokierunkową listę Datablocków (jak widać po przybliżonej definicji struktur), dzięki czemu poszczególne datablocki danego pliku nie muszą być ustawione po kolei. W połączeniu z ograniczoną ilością datablocków i ich stałym rozmiarem, zjawisko fragmentacji zewnętrznej nie będzie problemem.  
Niestety wadą takiego rozwiązania jest występowanie zjawiska fragmentacji wewnętrznej, która przy dużej ilości niewielkich plików może znacząco obniżyć efektywną pojemność systemu plików.

</div>