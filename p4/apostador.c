#include "apostador.h"

int apostador(int numapostadores, int numcaballos, int msqid, int tiempop){
	int ret, i;
	int *tiempo;
	srand(time(NULL)*getpid()>>16);
	if((tiempo = shmat(tiempop, NULL, 0))== (void*)-1){
		perror("Error al obtener la zona compartida de memoria en el apostador\n");
		*tiempo=-1;
		return -1;
	}
	if((ret=fork())<0){
		perror("Error haciendo fork en el proceso apostador");
		*tiempo=-1;
	}
	else if (ret==0){
		for (i=0; i<numapostadores; i++){
			if(ret=fork()<0){
				perror("Error haciendo fork en el proceso apostador");
				*tiempo=-1;
				return -1;
			}
			if(ret==0){
				while (*tiempo>0){
					ApuestaMsg apuesta;
					apuesta.mtype = 1;
					apuesta.numapostador = i;
					apuesta.numcaballo = rand()%numcaballos;
					apuesta.apuesta = ((double)rand()/RAND_MAX)*1000;
					msgsnd(msqid, (struct msgbuf*) &apuesta, sizeof(ApuestaMsg)-sizeof(long), 0);
					usleep(100000);
				}
				shmdt(tiempo);
				exit(EXIT_SUCCESS);
			}
		}
		while(wait(NULL)>0);
		exit(EXIT_SUCCESS);
	}
	else{
		return 1;
	}
}
