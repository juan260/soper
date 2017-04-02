
CC = gcc -ansi -pedantic -D_GNU_SOURCE
CFLAGS = -Wall -g 
EXE = ejercicio2

all : $(EXE) ejercicio5

clean :
	rm -f *.o testSemaforos $(EXE)

$(EXE) : % : %.o
	$(CC) $(CFLAGS)  -pthread -o $@ $@.o

ejercicio5 : ejercicio5.o semaforos.o
	$(CC) $(CFLAGS) -o $@ $@.o semaforos.o

ejercicio2.o : ejercicio2.c 
	$(CC) -g -c  $<

semaforos.o : semaforos.c semaforos.h
	$(CC) -g -c $<

ejercicio5.o : ejercicio5.c semaforos.h
	$(CC) -g -c $<
