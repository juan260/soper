
CC = gcc -ansi -pedantic -D_GNU_SOURCE
CFLAGS = -Wall -g 

all : caballos

clean :
	rm -f *.o caballos

caballos : caballos.o semaforos.o
	$(CC) $(CFLAGS) -o $@ $@.o semaforos.o

semaforos.o : semaforos.c semaforos.h
	$(CC) -g -c $<

caballos.o : caballos.c semaforos.h
	$(CC) -g -c $<
