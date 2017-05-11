#ifndef MONITOR_H
#define MONITOR_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "semaforos.h"

int monitor(int nCaballos, int nApostadores, int * sid, int mutex,
  int* matrizApuestasId);

#endif
