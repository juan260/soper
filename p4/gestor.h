#ifndef GESTOR_H
#define GESTOR_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "semaforos.h"
#include "apostador.h"
#include <pthread.h>

int gestor (int nventanillas, int napostadores, int ncaballos, int *mid, int tiempop, int msqid, int semid);