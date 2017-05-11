#include "gestor.h"

int gestor (int nventanillas, int napostadores, int ncaballos, int *mid, int tiempop, int msqid, int semid){
	int ret, i;
	if(ret=fork()<0){
		perror("Error de fork");
		return -1;
	}
	else if(ret!=0){
		pthread_t ventanillas[nventanillas];
		GestorInfo *g = (GestorInfo*) malloc(sizeof(GestorInfo));
		if(g==NULL){
			perror("Error al reservar memoria en el gestor de apuesta\n");
			*tiempo=-1;
			exit(EXIT_FAILURE);
		}
		if((tiempo = shmat(tiempop, NULL, 0))==(void*)-1){
			perror("Error al obtener la zona compartida de memoria en el gestor de apuesta\n");
			*tiempo=-1;
			exit(EXIT_FAILURE);
		}
		
		for(i=0; i<nventanillas; i++){
			 pthread_create(, NULL, first_matrix, (void*) m);
		}
	}

}

typedef struct gestorinfo{
	double **cxa;
	int *tiempo;
	int msqid;
	int mutex;
}GestorInfo;

NULL* ventanilla(NULL *info){
	GestorInfo *g = (GestorInfo*) info;
	while(g->(*tiempo)>0){
		ApuestaMsg rcv;
		msgrcv(msqid,  (struct msgbuf*)&rcv, sizeof(ApuestaMsg)-sizeof(long), 1, 0);
		if(Down_Semaforo(mutex, 2, SEM_UNDO)==ERROR){
			perror("Error de semaforos en el gestor de apuestas")
			return NULL;
		}
		g->cxa[rcv.numapostador][rcv.numcaballo]+=rcv.apuesta;
		if(Up_Semaforo(mutex, 2, SEM_UNDO)==ERROR){
			perror("Error de semaforos en el gestor de apuestas")
			return NULL;
		}
	}
	return NULL;
}