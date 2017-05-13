/**
* @brief Fuente del ejercicio 4
*
* En este fuente se implementa la librería de semáforos propuesta en el enunciado.
* @file semaforos.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 1.0
* @date 06-abril-2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "semaforos.h"

/***************************************************************
Nombre: Inicializar_Semaforo.
Descripcion: Inicializa los semaforos indicados.
Entrada:
	int semid: Identificador del semaforo.
	unsigned short *array: Valores iniciales.
Salida:
	int: OK si todo fue correcto, ERROR en caso de error.
************************************************************/
int Inicializar_Semaforo(int semid, unsigned short *array){
    union semun {
        int val;
        struct semid_ds *semstat;
        unsigned short *array;
    } arg;

    arg.array=array;

    if(semctl(semid,0,SETALL,arg)==-1){
        perror("Error al inicializar el semaforo\n");
    }

	return OK;
}

/***************************************************************
Nombre: Borrar_Semaforo.
Descripcion: Borra un semaforo.
Entrada:
	int semid: Identificador del semaforo.
Salida:
	int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int Borrar_Semaforo(int semid){
	if(semctl(semid,0,IPC_RMID)<0){
		return ERROR;
	}
	return OK;
}

/***************************************************************
Nombre: Crear_Semaforo.
Descripcion: Crea un semaforo con la clave y el tamaño
especificado. Lo inicializa a 0.
Entrada:
	key_t key: Clave precompartida del semaforo.
	int size: Tamaño del semaforo.
Salida:
	int *semid: Identificador del semaforo.
	int: ERROR en caso de error,
		0 si ha creado el semaforo,
		1 si ya estaba creado.
**************************************************************/
int Crear_Semaforo(key_t key, int size, int *semid){
	int sem, ret, i;
  unsigned short *array;

  sem = semget(key, size, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
	ret = 0;
	if(sem==-1){
		if(errno==EEXIST){
			sem = semget(key, size,
				IPC_CREAT | SHM_R | SHM_W);
			ret = 1;
		}
    return ERROR;
	}


	array=(unsigned short*)malloc(sizeof(unsigned short)*size);

    if(array==NULL){
        Borrar_Semaforo(sem);
        perror("Error en la creacion del semaforo\n");
        return ERROR;
    }

    for(i=0;i<size;i++){
        array[i]=0;
    }
    if(Inicializar_Semaforo(sem, array)==ERROR){
        Borrar_Semaforo(sem);
        perror("Error al inicializar el semaforo\n");
	    return ERROR;
    }
    *semid=sem;
	free(array);
    return ret;
}

/**************************************************************
Nombre: Down_Semaforo
Descripcion: Baja el semaforo indicado
Entrada:
	int semid: Identificador del semaforo.
	int num_sem: Semaforo dentro del array.
	int undo: Flag de modo persistente pese a finalización
		abrupta.
Salida:
	int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int Down_Semaforo(int id, int num_sem, int undo){
	struct sembuf sem_oper;
	sem_oper.sem_num = num_sem;
	sem_oper.sem_op =-1;
	sem_oper.sem_flg = undo;
	if(semop (id, &sem_oper, 1)==-1){
  	        return ERROR;
        }
        return OK;

	}

/***************************************************************
Nombre: DownMultiple_Semaforo
Descripcion: Baja todos los semaforos del array indicado
	por active.
Entrada:
	int semid: Identificador del semaforo.
	int size: Numero de semaforos del array.
	int undo: Flag de modo persistente pese a finalización
		abrupta.
	int *active: Semaforos involucrados.
Salida:
	int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int DownMultiple_Semaforo(int id,int size,int undo,int *active){
    int i;
    for(i=0;i<size;i++){
        if(Down_Semaforo(id, active[i], undo)==ERROR)
            return ERROR;
    }
    return OK;
}

/**************************************************************
Nombre:Up_Semaforo
Descripcion: Sube el semaforo indicado
Entrada:
	int semid: Identificador del semaforo.
	int num_sem: Semaforo dentro del array.
	int undo: Flag de modo persistente pese a finalizacion
		abupta.
Salida:
	int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int Up_Semaforo(int id, int num_sem, int undo){
        struct sembuf sem_oper;
        sem_oper.sem_num = num_sem;
        sem_oper.sem_op =+1;
        sem_oper.sem_flg = undo;
        if(semop (id, &sem_oper, 1)==-1){
		return ERROR;
	}
	return OK;

}

/***************************************************************
Nombre: UpMultiple_Semaforo
Descripcion: Sube todos los semaforos del array indicado
	por active.
Entrada:
	int semid: Identificador del semaforo.
	int size: Numero de semaforos del array.
	int undo: Flag de modo persistente pese a finalización
		abrupta.
	int *active: Semaforos involucrados.
Salida:
	int: OK si todo fue correcto, ERROR en caso de error.
***************************************************************/
int UpMultiple_Semaforo(int id,int size, int undo, int *active){
    int i;
    for(i=0;i<size;i++){
        if(Up_Semaforo(id, active[i], undo)==ERROR)
            return ERROR;
    }
    return OK;

}
