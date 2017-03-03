/**
* @brief Fuente del ejercicio 5b
*
* Este programa ejecuta una serie de forks de tal forma que un único padre tiene una serie de hijos, pero éstos no tienen más hijos.
* @file ejercicio5b.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
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
	int pid, i, status;
	for (i=0; i < NUM_PROC; i++){
		if ((pid=fork()) <0 ){
			printf("Error haciendo fork\n");
			exit(EXIT_FAILURE);
		}else if (pid ==0){
			printf("HIJO %d\n", i);
			break;
		}else{
			printf ("PADRE %d\n", i);
		}
	}

	/*Esperamos a que acabe la ejecución de todos los procesos hijo*/

	while(wait(&status)>0){}
	exit(EXIT_SUCCESS);
}
