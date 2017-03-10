
CC = gcc -ansi -pedantic
CFLAGS = -Wall -g
EXE = ejercicio3a ejercicio6 

all : $(EXE)

clean :
	rm -f *.o $(EXE)

$(EXE) : % : %.o
	$(CC) $(CFLAGS) -o $@ $@.o

ejercicio6.o : ejercicio6.c 
	$(CC) -g -c ejercicio6.c

ejercicio3a.o : ejercicio3a.c
	$(CC) $(CFLAGS) -c ejercicio3a.c


