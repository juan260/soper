
CC = gcc -ansi -pedantic -D_GNU_SOURCE
CFLAGS = -Wall -g 

all : caballos ejercicio1

clean :
	rm -f *.o caballos ejercicio1

ejercicio1 : ejercicio1.o
	$(CC) $(CFLAGS) -o $@ $@.o
caballos : caballos.o semaforos.o caballo.o
	$(CC) $(CFLAGS) -o $@ $@.o semaforos.o caballo.o

semaforos.o : semaforos.c semaforos.h
	$(CC) -g -c $<

caballos.o : caballos.c semaforos.h
	$(CC) -g -c $<

caballo.o : caballo.c caballo.h semaforos.h
	$(CC) -g -c $<
