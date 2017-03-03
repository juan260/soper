/**
* @brief Fuente del ejercicio 4, apartado b
*
* Este modulo hace una serie de forks e imprime
* los pid del padre y del hijo de cada uno de estos.
* Además el padre espera a que sus hijos terminen
* @file ejercicio4b.c
* @author Pareja 21: Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 1.0
* @date 02-03-2017
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define NUM_PROC 3

int main (void)
    {
        int pid;
        int i;
        printf("id superpadre = %d\n", getpid());
        for (i=0; i < NUM_PROC; i++){
            if ((pid=fork()) <0 ){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }else if (pid ==0){
                printf("HIJO: pid %d, pid del padre %d\n", getpid(), getppid());
            }
        }
        wait(NULL);
        exit(EXIT_SUCCESS);

                                         }
