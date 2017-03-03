/**
* @brief Fuente del ejercicio 5a
*
* Este programa ejecuta una serie de forks de tal forma que un padre solo puede tener un único hijo.
* @file ejercicio5a.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 1.0
* @date 02-03-2017
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define NUM_PROC 3
int main (void)
{
    int pid;
    int i;
    int status;
    for (i=0; i < NUM_PROC; i++){
        if ((pid=fork()) <0 ){
            printf("Error haciendo fork\n");
            exit(EXIT_FAILURE);
        }else if (pid ==0){
            printf("HIJO %d\n", i);
	    }else{
            printf ("PADRE %d\n", i);
            wait(&status);
            break;
        }
    
    }
    exit(EXIT_SUCCESS);
}
