#include <stdio.h>
#include <stdlib.h>
#include "semaforos.h"
#define PROJID 1301
#define UNIQUEPATH "fichero.txt"
/* This define will be define de number of semaphores to be created */
#define SEMNUM 5

int main(){
    
    int error, semid, ret, key, i, values[SEMNUM];
    
    union semun {
        int val;
        struct semid_ds *semstat;
        unsigned short *array;
    } arg;
    
    arg.array=(unsigned short*)malloc(sizeof(unsigned short)*SEMNUM);
    if(arg.array==NULL){
        exit(EXIT_FAILURE);
        }
    printf("Iniciando test de semaforos\n");
    
    printf("Probando la creación de semaforos\n");
    key = ftok(UNIQUEPATH, PROJID);
    if(Crear_Semaforo(key, SEMNUM, &semid)==ERROR){
        perror("Error en la creación del semáforo\n");
        free(arg.array);
        exit(EXIT_FAILURE);
    }

    printf("Semaforo creado.\n");
    if(semctl(semid, 0, GETALL, arg)==-1){
        perror("Error al obtener valores");
    }
    
    for(i=0;i<SEMNUM;i++){
        if(arg.array[i]!=0){
            printf("El semaforo %d no está bien inicializado, tenia el valor %hu\n", i, arg.array[i]);
            Borrar_Semaforo(semid);
            exit(EXIT_FAILURE);
            }
    }
    Borrar_Semaforo(semid);
    exit(EXIT_SUCCESS);
}
