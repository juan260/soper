/**
* @brief Fuente del ejercicio 6
*
* En este programa se simula el problema clásico del productor-consumidor, un proceso "produce" (escribe en una
* memoria compartida) y otro "consume" lo que se ha prducido (lee de la memoria compartida lo que ha escrito el
* productor)
* @file ejercicio6.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 1.0
* @date 07-abril-2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include "semaforos.h"

#define MAXBUF 26
#define PROJID 1300

int main (int argc, char *argv[]) {
	char *buffer;
	int keySh, id_zone, semid, ret, status;
	unsigned short ini[3]={1, 0, 26};



	keySh = ftok("fichero6.txt", PROJID);

	if(Crear_Semaforo(keySh, 3, &semid)==ERROR){/*Mutex, contador de letras, contador de espacios*/
		perror("Error creando semaforos");
		exit(EXIT_FAILURE);
	}

	if(Inicializar_Semaforo(semid, ini)==ERROR){
		perror("Error inicializando semaforos");
		Borrar_Semaforo(semid);

	}

	if((id_zone = shmget (keySh, sizeof(char)*MAXBUF, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W)) == -1){
		perror("Error al inicializar la zona de memoria compartida");
		Borrar_Semaforo(semid);
		exit(EXIT_FAILURE);
	}

	if((buffer = shmat(id_zone, NULL, 0))==NULL){
		perror("Error al obtener la variable de memoria compartida.\n");
		Borrar_Semaforo(semid);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*)NULL);            
        exit(EXIT_FAILURE);
	}
	
	ret=fork();
	if(ret<0){
		perror("Error de fork");
		Borrar_Semaforo(semid);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*)NULL);            
        exit(EXIT_FAILURE);
	}
	else if (ret !=0){
		int i;
		for (i=0; i<26; i++){
			Down_Semaforo(semid, 0, SEM_UNDO);
			Down_Semaforo(semid, 2, SEM_UNDO);
			buffer[i]=i+97;
			Up_Semaforo(semid, 0, SEM_UNDO);
			Up_Semaforo(semid, 1, SEM_UNDO);
		}
		wait(&status);
		Borrar_Semaforo(semid);
        shmctl(id_zone, IPC_RMID, (struct shmid_ds*)NULL);            
        exit(EXIT_SUCCESS);
	}
	else {
		int i;
		for (i=0; i<26; i++){
			Down_Semaforo(semid, 0, SEM_UNDO);
			Down_Semaforo(semid, 1, SEM_UNDO);
			printf("Recurso extraido: %c\n", buffer[i]);
			Up_Semaforo(semid, 0, SEM_UNDO);
			Up_Semaforo(semid, 2, SEM_UNDO);
		}
		exit(EXIT_SUCCESS);
	}
}
