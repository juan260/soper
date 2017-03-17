/**
* @brief Fuente del ejercicio 6
*
* Este programa crea un proceso hijo que escribe por pantalla y espera 5 segundos.
* El proceso padre espera 30 y mata al proceso hijo.
* @file ejercicio3a.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 1.0
* @date 15-03-2017
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h> 

int main()
{
	int ret;	
	if((ret=fork())==-1){
		perror("Error al hacer fork.\n");

	}else if(ret==0){
		printf("Soy el proceso hijo con PID: %d\n", getpid());
		sleep(5);
	} else {
		sleep(30);
		kill(ret, SIGINT);
	}
	exit(EXIT_SUCCESS);
}
