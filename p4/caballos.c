#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "semaforos.h"
#include "caballo.h"
#include "monitor.h"
#include "gestor.h"
#include "apostador.h"
#include <errno.h>
#define MAXBUF 100
#define PROJID 45

void argumentosEntrada(int argc, char * argv[], int * nCaballos,
	int * longCarrera, int * nApostadores, int * nVentanillas){
	if(argc == 1){
		perror("Error en la entrada de argumentos.\n"
			"El comando de entrada deberia ser:"
			"\n\n\t./caballos A B C D\n\nDonde:\n"
			"-A: numero de caballos\n-B: longitud de"
			" la carrera\n-C: numero de apostadores\n"
			"-D: numero de ventanillas.\n"
		"Asignando valores por defecto equivalentes al comando:\n\n"
		"\t./caballos 5 100 5 5\n\n");
		*nCaballos = 5;
		*longCarrera = 100;
		*nApostadores = 5;
		*nVentanillas = 5;
		return;
	}

	if(argc != 5){
		perror("Error en la entrada de argumentos.\n"
			"El comando de entrada deberia ser:"
			"\n\n\t./caballos A B C D\n\nDonde:\n"
			"-A: numero de caballos\n-B: longitud de"
			" la carrera\n-C: numero de apostadores\n"
			"-D: numero de ventanillas.\n");
		exit(EXIT_FAILURE);
	}

	*nCaballos = atoi(argv[1]);
	*longCarrera = atoi(argv[2]);
	*nApostadores = atoi(argv[3]);
	*nVentanillas = atoi(argv[4]);

	if(*nCaballos > 10||*nCaballos <= 0||*longCarrera <= 0||
		*nApostadores <= 0||*nApostadores > 10||*nVentanillas <= 0){
		perror("Alguno de los argumentos de entrada no es "
			"valido. \nEjecutar el programa sin argumentos "
			"para ver el comando de ejecucion");
		exit(EXIT_FAILURE);
	}
}

int ** pipesCaballos(int nCaballos){
	int ** pipes;
	int i;
	pipes = (int**)malloc(nCaballos*sizeof(int*));
	for(i=0;i<nCaballos;i++){
			pipes[i]=(int*)malloc(sizeof(int)*2);
			if(pipe(pipes[i])==-1){
					perror("Error al hacer pipe\n");
					i--;
					for(;i>=0;i--){
							close(pipes[i][LEER]);
							close(pipes[i][ESCRIBIR]);
					}
					return NULL;
					}
	}
	return pipes;
}

void freePipesCaballos(int ** pipes, int nCaballos){
	int i;
	for(i=0;i<nCaballos;i++){
		close(pipes[i][LEER]);
		close(pipes[i][ESCRIBIR]);
		free(pipes[i]);
	}
	free(pipes);
}

int incializarVariablesCompartidas(int nCaballos, int nApostadores, int sid[3],
 int **matrizApuestasId){
		int key, i, size, j;
		int * posicionCaballo;
		int * tiempo;
		double * arrayApuestas;

		key = ftok("/bin/ls", PROJID+7);
		if((sid[0] = shmget(key, sizeof(int),
				IPC_CREAT | SHM_R | SHM_W))==-1){
				return -1;
		}



		key = ftok("/bin/ls", PROJID+8);
		if((sid[1] = shmget(key, sizeof(int)*nCaballos,
				IPC_CREAT| IPC_EXCL| SHM_R | SHM_W))==-1){
				if(errno==17 /*file exists*/){
					shmctl(shmget(key, sizeof(int)*nCaballos,
							IPC_CREAT| SHM_R | SHM_W), IPC_RMID,
							(struct shmid_ds*)NULL);
				}
				shmctl(sid[0], IPC_RMID, (struct shmid_ds*)NULL);
				return -1;
		}

	
		/* Inicializamos las posiciones de los caballos */
		if((posicionCaballo = (int *)shmat(sid[1], NULL, 0))==(void*)-1){
			shmctl(sid[0], IPC_RMID, (struct shmid_ds*)NULL);
			shmctl(sid[1], IPC_RMID, (struct shmid_ds*)NULL);
			return -1;
		}
		/* No controlamos la concurrencia porque no hemos hecho ningun fork aun*/
		for(i=0;i<nCaballos;i++){
			posicionCaballo[i]=0;
		}
		if(shmdt(posicionCaballo)==-1){
			shmctl(sid[0], IPC_RMID, (struct shmid_ds*)NULL);
			shmctl(sid[1], IPC_RMID, (struct shmid_ds*)NULL);
			return -1;
		}

		/* Inicializamos la variable tiempo */
		if((tiempo = (int *)shmat(sid[0], NULL, 0))==(void*)-1){
			shmctl(sid[0], IPC_RMID, (struct shmid_ds*)NULL);
			shmctl(sid[1], IPC_RMID, (struct shmid_ds*)NULL);
			return -1;
		}
		/* No controlamos la concurrencia porque no hemos hecho ningun fork aun*/
		*tiempo = 15;
		if(shmdt(tiempo)==-1){
			shmctl(sid[0], IPC_RMID, (struct shmid_ds*)NULL);
			shmctl(sid[1], IPC_RMID, (struct shmid_ds*)NULL);
			return -1;
		}

		/* Inicializamos la matriz de apuestas */
		*matrizApuestasId = (int *)malloc(sizeof(int)*nCaballos);
		for(i=0;i<nCaballos;i++){
			key = ftok("/bin/ls", PROJID+9+i);
			if((((*matrizApuestasId)[i]) = shmget(key, sizeof(double)*nApostadores,
					IPC_CREAT| IPC_EXCL| SHM_R | SHM_W))==-1){
					if(errno==17 /*file exists*/){
						shmctl(shmget(key, sizeof(double)*nApostadores,
								IPC_CREAT| SHM_R | SHM_W), IPC_RMID,
								(struct shmid_ds*)NULL);
					}
					i--;
					for(;i>=0;i--){
						shmctl((*matrizApuestasId)[i], IPC_RMID, (struct shmid_ds*)NULL);
					}
					shmctl(sid[1], IPC_RMID, (struct shmid_ds*)NULL);
					shmctl(sid[0], IPC_RMID, (struct shmid_ds*)NULL);
					return -1;
			}

			if((arrayApuestas = (double *)shmat((*matrizApuestasId)[i], NULL, 0))==(void*)-1){
				for(;i>=0;i--){
					shmctl((*matrizApuestasId)[i], IPC_RMID, (struct shmid_ds*)NULL);
				}
				shmctl(sid[0], IPC_RMID, (struct shmid_ds*)NULL);
				shmctl(sid[1], IPC_RMID, (struct shmid_ds*)NULL);
				return -1;
			}
		/* No controlamos la concurrencia porque no hemos hecho ningun fork aun*/
			for(j=0;j<nApostadores;j++){
				arrayApuestas[j]=0.0;
			}
			if(shmdt(arrayApuestas)==-1){
				for(;i>=0;i--){
					shmctl((*matrizApuestasId)[i], IPC_RMID, (struct shmid_ds*)NULL);
				}
				shmctl(sid[0], IPC_RMID, (struct shmid_ds*)NULL);
				shmctl(sid[1], IPC_RMID, (struct shmid_ds*)NULL);
				return -1;
			}
		}

		return 0;
}

void freeVariablesCompartidas(int * sid){
	if((shmctl(sid[0], IPC_RMID, (struct shmid_ds*)NULL)==-1) ||
		 (shmctl(sid[1], IPC_RMID, (struct shmid_ds*)NULL)==-1)){
		perror("Error al eliminar la memoria compartida.");
	}
}

void freeEverything(int semCaballos, int mutex, int ** pipePadreACaballo,
	int * sid, int nCaballos){
	Borrar_Semaforo(semCaballos);
	Borrar_Semaforo(mutex);
	freePipesCaballos(pipePadreACaballo, nCaballos);
	freeVariablesCompartidas(sid);
}

int main(int argc, char * argv[]){

	/* Declaracion de variables */
	int nCaballos, longCarrera, nApostadores, nVentanillas, ret,
		keySem, keyMsg, i, j, tiempoId, posicionCaballoId,
		carreraIniciada=0;
	int sid[3];
	int ** pipePadreACaballo=NULL;
	int * matrizApuestasId;
	char buffer[MAXBUF];


	/* Declaracion de los semaforos:
		-semCaballos: array de semáforos del tamaño del numero
			de caballos. Cada caballo tiene su semaforo asignado
			en este array.
		-mutex: array de semaforos que contiene los mutex de
			todas las variables globales, en el orden en el que
			son declaradas mas adelante.
	*/
	int semCaballos, mutex;

	/* Declaracion de la cola de mensajes y mensajes*/
	int msqid;
	CaballoMsg caballorcv;

	/* Declaracion de variables de memoria compartida */

	/* Esta variable tendrá los segundos restantes hasta
	que empiece la carrera, cuando llegue a 0, todo empieza
	a funcionar. Cuando tome el valor -1 la carrera habra
	terminado y seguira bajando hasta llegar a -15, momento
	en el cual el monitor sabrá que tiene que mostrar todas
	las estadisticas */
	int * tiempo;

	/* Array de ints con la posicion de todos los caballos */
	int * posicionCaballo;

	/* Array arrays de ints con la cantidad apostada a cada caballo
		por cada apostador. (apostadorxcaballo) */
	int * matrizApuestas[10];

 printf("si esto sale dos veces estamos jodidos\n");

	/* Comprobacion de los argumentos de entrada */
	argumentosEntrada(argc, argv, &nCaballos, &longCarrera, &nApostadores,
				&nVentanillas);

	srand(time(NULL)*getpid()<<6);
	/* Creacion de los medios de comunicacion entre el
	proceso padre y los caballos, asi como los semaforos */
	pipePadreACaballo = pipesCaballos(nCaballos);

	keySem = ftok("/bin/ls", PROJID+1);

	if(Crear_Semaforo(keySem, nCaballos, &semCaballos)==ERROR){
		freePipesCaballos(pipePadreACaballo, nCaballos);
		perror("Error en semCaballos");
		exit(EXIT_FAILURE);
	}

	keySem = ftok("/bin/ls", PROJID+4);
	if(Crear_Semaforo(keySem, 3, &mutex)==ERROR){
		Borrar_Semaforo(semCaballos);
		freePipesCaballos(pipePadreACaballo, nCaballos);
		perror("Error en mutex");
		exit(EXIT_FAILURE);
	}

	keyMsg = ftok("/bin/ls", PROJID+5);
	if((msqid=msgget(keyMsg, 0600|IPC_CREAT))==-1){
		Borrar_Semaforo(semCaballos);
		Borrar_Semaforo(mutex);
		freePipesCaballos(pipePadreACaballo, nCaballos);
		perror("Error creacion de cola de mensajes");
		exit(EXIT_FAILURE);
	}



	/* Reservamos memoria para las variables de memoria compartida */
	if(incializarVariablesCompartidas(nCaballos, nApostadores, sid,
			&matrizApuestasId)==-1){
		perror("Error al incializar las variables de memoria compartida.\n");
		Borrar_Semaforo(semCaballos);
		Borrar_Semaforo(mutex);
		freePipesCaballos(pipePadreACaballo, nCaballos);
		exit(EXIT_FAILURE);
	}
		printf("is this the real life\n" );
	/* Crea proceso monitor */
	/*monitor(nCaballos, nApostadores, sid, mutex, matrizApuestasId);*/
	/* Crea proceso gestor de apuestas */
	if((ret=gestor (nVentanillas, nApostadores, nCaballos, matrizApuestasId, sid[0], msqid, mutex))==-1){
		freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
		while(wait(NULL)>0);
		exit(EXIT_FAILURE);
	}
	else if(ret==2){
		exit(EXIT_SUCCESS);
	}
	printf("or is just fantasy\n");
	/* Crea los procesos apostadores */
	if(apostador(nApostadores, nCaballos, msqid, sid[0])==-1){
		freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
		while(wait(NULL)>0);
		exit(EXIT_FAILURE);
	}
	printf("puta bida tt\n");
	/* Crea los caballos */
	if(caballos(nCaballos, pipePadreACaballo, semCaballos, msqid,
				mutex, sid)==CERROR){
		while(wait(NULL)>0);
		exit(EXIT_FAILURE);
	}

	/* Ir bajando la variable tiempo de 15 a 0 */
	if((tiempo = shmat(sid[0], NULL, 0))==(void*)-1){
		perror("Error al obtener la zona compartida de memoria.\n");
		freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
	}

	if(Up_Semaforo(mutex, 0, SEM_UNDO)==ERROR){
		perror("Error al hacer down del mutex.\n");
		freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
	}

	carreraIniciada = 0;
	while(carreraIniciada==0){
		sleep(1);
		if(Down_Semaforo(mutex, 0, SEM_UNDO)==ERROR){
			perror("Error al hacer down del mutex.\n");
			freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
		}
		if(*tiempo>0){
			*tiempo--;
		} else {
			carreraIniciada = 1;
		}
		if(Up_Semaforo(mutex, 0, SEM_UNDO)==ERROR){
			perror("Error al hacer up del mutex.\n");
			freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
			exit(EXIT_FAILURE);
		}
	}

	if((posicionCaballo = (int *)shmat(sid[1], NULL, 0))==(void*)-1){
		perror("Error al obtener posicionCaballo.\n");
		freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
		exit(EXIT_FAILURE);
	}
	/* Bucle que va despertando a los caballos y les
	va enviando y recibiendo las posiciones de los
	caballos */
	while(carreraIniciada==1){

		/* Despierto a los caballos y espero sus mensajes */
		for(i=0;i<nCaballos&&carreraIniciada==1;i++){
			for(j=0;j<nCaballos;j++){
				sprintf(buffer, "%d ", posicionCaballo[j]);
			}

			if(write(pipePadreACaballo[i][ESCRIBIR], buffer, strlen(buffer)+1)==-1){
				perror("Error al escribir en el pipe.");
				freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
			}

			if(Up_Semaforo(semCaballos, i, SEM_UNDO)==ERROR){
				perror("Error al hacer up de semCaballos.\n");
				freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
			}

			msgrcv(msqid, (struct msgbuf*)&caballorcv, sizeof(CaballoMsg)-sizeof(long), 2, 0);

			Down_Semaforo(mutex, 1, SEM_UNDO);
			for (j=0;j<nCaballos;j++){
				posicionCaballo[j]=caballorcv.posiciones[i];
			}

			Up_Semaforo(mutex, 1, SEM_UNDO);

			/* Actualizamos el estado de la variable tiempo */
			if(Down_Semaforo(mutex, 0, SEM_UNDO)==ERROR){
				perror("Error al hacer down de mutex.\n");
				freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
			}

			/* Si ha ganado algun caballo lo actualizamos */

			if(Up_Semaforo(mutex, 0, SEM_UNDO)==ERROR){
				perror("Error al hacer up de mutex.\n");
				freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
			}
		}
	}

	/* Sabemos que algun caballo ha terminado la carrera, les despertamos una
	ultima vez para que lean de la memoria compartida que la carrera ha terminado
	y terminen ellos */
	for(i=0;i<nCaballos;i++){
		if(Up_Semaforo(semCaballos, i, SEM_UNDO)==ERROR){
			perror("Error al hacer up de semCaballos.\n");
			freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
		}
	}

	while(*tiempo>-15){
		sleep(1);
		if(Down_Semaforo(mutex, 0, SEM_UNDO)==ERROR){
			perror("Error al hacer down del mutex.\n");
			freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
		}
		*tiempo--;
		if(Up_Semaforo(mutex, 0, SEM_UNDO)==ERROR){
			perror("Error al hacer up del mutex.\n");
			freeEverything(semCaballos, mutex, pipePadreACaballo, sid, nCaballos);
			exit(EXIT_FAILURE);
		}
	}
	freeEverything(semCaballos,mutex,pipePadreACaballo,sid,nCaballos);
	while(wait(NULL)>0);

	exit(EXIT_SUCCESS);
}
