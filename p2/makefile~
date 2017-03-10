
CC = gcc -ansi -pedantic
CFLAGS = -Wall -g
EXE = ejercicio4a ejercicio4b ejercicio5a ejercicio5b ejercicio6 ejercicio8 ejercicio9 

all : $(EXE)

clean :
	rm -f *.o $(EXE)

$(EXE) : % : %.o
	$(CC) $(CFLAGS) -o $@ $@.o

ejercicio4a.o : ejercicio4a.c 
	$(CC) $(CFLAGS) -c ejercicio4a.c


ejercicio4b.o : ejercicio4b.c
	$(CC) $(CFLAGS) -c ejercicio4b.c

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
    
