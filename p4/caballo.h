#ifndef CABALLO_H
#define CABALLO_H
#include "semaforos.h"
#define CERROR -1
#define COK 0
#define LEER 0
#define ESCRIBIR 1

typedef struct caballomsg{
	long mtype;
	int posiciones[10];
}CaballoMsg;

int caballo(int * pipe, int semId, int numCaballo,
  int numCaballos, int msqid, int mutex, int *sid);

int caballos(int numero, int ** pipes, int semId,
  int msqid, int mutex, int *sid);

#endif
