#ifndef APOSTADOR_H
#define APOSTADOR_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

typedef struct apuestamsg{
	long mtype;
	int numapostador;
	int numcaballo;
	double apuesta;
}ApuestaMsg;

void apostador(int numapostadores, int numcaballos, int msqid, int tiempop);


#endif