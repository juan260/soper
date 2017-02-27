
CC = gcc -ansi -pedantic
CFLAGS = -Wall -g
EXE = ejercicio4a1 ejercicio4a2 ejercicio4b1 ejercicio5a ejercicio5b ejercicio6 ejercicio8 ejercicio9 

all : $(EXE)

clean :
	rm -f *.o $(EXE)

$(EXE) : % : %.o
	$(CC) $(CFLAGS) -o $@ $@.o

ejercicio4a1.o : ejercicio4a1.c 
	$(CC) $(CFLAGS) -c ejercicio4a1.c

ejercicio4a2.o : ejercicio4a2.c
	$(CC) $(CFLAGS) -c ejercicio4a2.c

ejercicio4b1.o : ejercicio4b1.c
	$(CC) $(CFLAGS) -c ejercicio4b1.c

ejercicio5a.o : ejercicio5a.c
	$(CC) $(CFLAGS) -c ejercicio5a.c

ejercicio5b.o : ejercicio5b.c
	$(CC) $(CFLAGS) -c ejercicio5b.c

ejercicio6.o : ejercicio6.c
	$(CC) $(CFLAGS) -c ejercicio6.c

ejercicio8.o : ejercicio8.c
	$(CC) $(CFLAGS) -c ejercicio8.c

ejercicio9.o : ejercicio9.c
	$(CC) $(CFLAGS) -c ejercicio9.c
    
