
CC = gcc -ansi -pedantic
CFLAGS = -Wall -g
EXE = ejercicio3a ejercicio3b ejercicio6 ejercicio10

all : $(EXE)

clean :
	rm -f *.o $(EXE)

$(EXE) : % : %.o
	$(CC) $(CFLAGS) -pthread -o $@ $@.o

ejercicio6.o : ejercicio6.c 
	$(CC) -g -c ejercicio6.c

ejercicio3a.o : ejercicio3a.c
	$(CC) $(CFLAGS) -c ejercicio3a.c

ejerciio3b.o : ejercicio3b.c
	$(CC) $(CFLAGS) -c ejercicio3b.c 

ejercicio10.o : ejercicio10.c
	$(CC) $(CFLAGS) -c ejercicio10.c
