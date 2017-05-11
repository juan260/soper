
CC = gcc -ansi -pedantic -D_GNU_SOURCE
CFLAGS = -Wall -g

all : caballos ejercicio1

clean :
	rm -f *.o caballos ejercicio1

ejercicio1 : ejercicio1.o
	$(CC) $(CFLAGS) -o $@ $@.o

caballos : caballos.o semaforos.o caballo.o monitor.o gestor.o apostador.o
	$(CC) $(CFLAGS) -o $@ $@.o semaforos.o caballo.o monitor.o gestor.o apostador.o

semaforos.o : semaforos.c semaforos.h
	$(CC) -g -c $<

caballos.o : caballos.c semaforos.h
	$(CC) -g -c $<

caballo.o : caballo.c caballo.h semaforos.h
	$(CC) -g -c $<

monitor.o : monitor.c monitor.h semaforos.h
	$(CC) -g -c $<

gestor.o : gestor.c gestor.h semaforos.h
	$(CC) -g -c $<

apostador.o : apostador.c apostador.h semaforos.h
	$(CC) -g -c $<
