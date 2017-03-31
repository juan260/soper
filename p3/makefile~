
CC = gcc -ansi -pedantic
CFLAGS = -Wall -g
EXE = ejercicio2

all : $(EXE)

clean :
	rm -f *.o $(EXE)

$(EXE) : % : %.o
	$(CC) $(CFLAGS) -pthread -o $@ $@.o

ejercicio2.o : ejercicio2.c 
	$(CC) -g -c ejercicio2.c

