#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "caballo.h"
#define MAXBUF 100

int caballo(int * pipe, int semId, int numCaballo,
				int numCaballos){
	int ret, i, posicion, tirada;
	int *posicionCaballo;
	char buffer[MAXBUF];

	if((ret = fork())<0){
		return CERROR;
	} else if(ret == 0){
		/* Codigo del caballo */
		srand(time(NULL)^getpid()<<16);
		posicionCaballo = (int*)malloc(sizeof(int)*numCaballos);
		while(1/* No se ha acabado la carrera */){
			Down_Semaforo(semId, numCaballo, SEM_UNDO);

			read(pipe[LEER], (void *)buffer, MAXBUF-1);
			for(i=0;i<numCaballos;i++){
				sscanf(buffer, "%d ", posicionCaballo+i);
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

			/* Los proximos printf's se deberian de borrar */
			for(i=0;i<numCaballos;i++){
				printf("%d ", posicionCaballo[i]);
			}
			printf("\n");


			/* Envía un mensaje a su padre que contenga
			posicionCaballo */
		}
		free(posicionCaballo);
	} else {
		return ret;
	}
}

int caballos(int numero, int ** pipes, int semId){
	int i;
	for(i=0;i<numero;i++){
		if(caballo(pipes[i], semId, i, numero)==CERROR){
			return CERROR;
		}
	}
	return COK;
}
