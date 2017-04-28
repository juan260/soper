#include "apostador.h"

void apostador(int numapostadores, int numcaballos, int msqid, int *tiempo){	
	srand(time(NULL)*getpid()>>16);
	while (*tiempo>0){
		ApuestaMsg apuesta;
		apuesta.mtype = 1;
		apuesta.numapostador = rand()%numapostadores;
		apuesta.numcaballo = rand()%numcaballos;
		apuesta.apuesta = (double)rand()/RAND_MAX*1000;
		msgsnd(msqid, (struct msgbuf*) &apuesta, sizeof(ApuestaMsg)-sizeof(long), 0);
		usleep(100000);
	}
}