#include "gestor.h"

typedef struct gestorinfo{
	double **axc;
	int *tiempo;
	int msqid;
	int mutex;
}GestorInfo;

void* ventanilla(void *info){
	GestorInfo *g = (GestorInfo*) info;
	while(*(g->tiempo)>0){
		ApuestaMsg rcv;
		msgrcv(g->msqid,  (struct msgbuf*)&rcv, sizeof(ApuestaMsg)-sizeof(long), 1, 0);
		if(Down_Semaforo(g->mutex, 2, SEM_UNDO)==ERROR){
			perror("Error de semaforos en el gestor de apuestas");
			return NULL;
		}
		g->axc[rcv.numapostador][rcv.numcaballo]+=rcv.apuesta;
		if(Up_Semaforo(g->mutex, 2, SEM_UNDO)==ERROR){
			perror("Error de semaforos en el gestor de apuestas");
			return NULL;
		}
	}
	return NULL;
}


int gestor (int nventanillas, int napostadores, int ncaballos, int *mid, int tiempop, int msqid, int semid){
	int ret, i;
	if((ret=fork())<0){
		perror("Error de fork");
		return -1;
	}
	else if(ret==0){
		pthread_t *ventanillas = (pthread_t*) malloc((sizeof(pthread_t)*nventanillas));
		GestorInfo *g = (GestorInfo*) malloc(sizeof(GestorInfo));
		double *axc[10];
		int *tiempo;

		if((tiempo = shmat(tiempop, NULL, 0))==(void*)-1){
			perror("Error al obtener la zona compartida de memoria en el gestor de apuesta\n");
			exit(EXIT_FAILURE);
		}

		if(g==NULL){
			perror("Error al reservar memoria en el gestor de apuesta\n");
			*tiempo=-1;
			shmdt(tiempo);
			exit(EXIT_FAILURE);
		}

		for(i=0; i<napostadores; i++){
			if((axc[i] = shmat(mid[i], NULL, 0))==(void*)-1){
				for (;i>=0;i--){
					shmdt(axc[i]);
				}
				free(g);
				shmdt(tiempo);
			}
		}
		g->tiempo=tiempo;
		g->axc=axc;
		g->msqid=msqid;
		g->mutex=semid;
		for(i=0; i<nventanillas; i++){
			 pthread_create(&ventanillas[i], NULL, ventanilla, (void*) g);
		}
		for(i=0; i<nventanillas; i++){
			pthread_join(ventanillas[i], NULL);
		}
		return 2;
	}
	else{
		printf("JODEEEEEEEEEEEEEEEEEER\n"	);
		return 1;
	}

}
