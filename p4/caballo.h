#ifndef CABALLO_H
#define CABALLO_H
#include "semaforos.h"
#define CERROR -1
#define COK 0
#define LEER 0
#define ESCRIBIR 1

int caballo(int * pipe, int semId, int numCaballo,
  int numCaballos);

int caballos(int numero, int ** pipes, int semId);

#endif
