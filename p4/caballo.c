#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "caballo.h"
#define MAXBUF 100

int caballo(int * pipe, int semId, int numCaballo,
				int numCaballos, int msqid, int mutex, int *sid){
	int ret, i, posicion, tirada;
	int posicionCaballo[10];
	int * tiempo;
	char buffer[MAXBUF];
	CaballoMsg mensaje;


	if((ret = fork())<0){
		return CERROR;
	} else if(ret == 0){
		/* Codigo del caballo */
		srand(time(NULL)^getpid()<<16);
		if((tiempo = shmat(sid[0], NULL, 0))==(void*)-1){
			exit(EXIT_FAILURE);
		}
		while(1){
			if(Down_Semaforo(semId, numCaballo, SEM_UNDO)==CERROR){
				close(pipe[LEER]);
				close(pipe[ESCRIBIR]);
				shmdt(tiempo);
				perror("Error al hacer down en el caballo.\n");
				exit(EXIT_FAILURE);
			}

			/* Comprobamos si ha terminadola carrera */
			if(Down_Semaforo(mutex,0,SEM_UNDO)==CERROR){
				close(pipe[LEER]);
				close(pipe[ESCRIBIR]);
				shmdt(tiempo);
				perror("Error al hacer down del mutex en el caballo.\n");
				exit(EXIT_FAILURE);
			}
			if(*tiempo<0){
				break;
			}
			if(Up_Semaforo(mutex,0,SEM_UNDO)==CERROR){
				close(pipe[LEER]);
				close(pipe[ESCRIBIR]);
				shmdt(tiempo);
				perror("Error al hacer up del mutex en el caballo.\n");
				exit(EXIT_FAILURE);
			}

			if(read(pipe[LEER], buffer, MAXBUF-1)==-1){
				close(pipe[LEER]);
				close(pipe[ESCRIBIR]);
				shmdt(tiempo);
				perror("Error al hacer read.\n");
				exit(EXIT_FAILURE);
			}
			for(i=0;i<numCaballos;i++){
				sscanf(buffer, "%d ", &posicionCaballo[i]);
			}
			/* Comprobamos la posicion del caballo
			y despues si es el primero o ultimo.
			Hemos considerado que si hay un empate:

			 	-Si dos caballos van en ultima posicion,
					los dos tendran tirada de dos dados.
				-Si dos caballos van primeros, ninguno
					de los dos tendra tirada especial */

			for(i=0, posicion=0;i<numCaballos;i++){
				if(posicionCaballo[i]<posicionCaballo[numCaballo]){
					posicion++;
				}
			}

			if(posicion==numCaballos-1){
				/* Este caballo va primero, tira entre
				1 y 7 */
				posicionCaballo[numCaballo]=rand()%6+1;
			} else if(posicion==0){
				/* Este caballo va ultimo, tira entre
				2 y 12 */
				posicionCaballo[numCaballo]=rand()%10+2;
			} else {
				/* Este caballo no va ni primero ni ultimo
				tendra una tirada normal entre 1 y 6*/
				posicionCaballo[numCaballo]=rand()%5+1;
			}

			mensaje.mtype=2;
			for(i=0; i<numCaballos; i++){
				mensaje.posiciones[i]=posicionCaballo[i];
			}
			if(msgsnd(msqid, (struct msbuff*) &mensaje, sizeof(CaballoMsg)-sizeof(long), 0)==-1){
				close(pipe[LEER]);
				close(pipe[ESCRIBIR]);
				shmdt(tiempo);
				perror("Error al enviar el mensaje desde el caballo.\n");
				exit(EXIT_FAILURE);
			}
		}
		shmdt(tiempo);
		close(pipe[LEER]);
		close(pipe[ESCRIBIR]);
		exit(EXIT_SUCCESS);
	} else {
		return ret;
	}
}

int caballos(int numero, int ** pipes, int semId, int msqid, int mutex, int *sid){
	int i;
	for(i=0;i<numero;i++){
		if(caballo(pipes[i], semId, i, numero, msqid, mutex, sid)==CERROR){
			return CERROR;
		}
	}
	return COK;
}
