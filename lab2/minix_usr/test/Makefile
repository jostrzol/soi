CC=cc

all: test group setpercenta

test: test.c
	$(CC) -o $@ test.c 
setpercenta: setpercenta.o syscalls.o
	$(CC) -o $@ setpercenta.o syscalls.o
group: group.o syscalls.o
	$(CC) -o $@ group.o syscalls.o
proc_a: proc_a.o syscalls.o
	$(CC) -o $@ proc_a.o syscalls.o
proc_b: proc_b.o syscalls.o
	$(CC) -o $@ proc_b.o syscalls.o

syscalls.o: syscalls.h
group.o: syscalls.h
proc_a.o: syscalls.h
proc_b.o: syscalls.h
setpercenta.o: syscalls.h
