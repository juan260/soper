#include <stdio.h>
#include <stdlib.h>
#include "semaforos.h"

void procesoMonitor(){}
	
int main(int argc, char * argv[]){
	
	/* Declaracion de variables */
	int nCaballos, longCarrera, nApostadores, nVentanillas, ret;
	
	
	/* Comprobacion de los argumentos de entrada */

	if(argc != 5){
		perror("Error en la entrada de argumentos.\n"
			"El comando de entrada deberia ser:"
			"\n\n\t./caballos A B C D\n\nDonde:\n"
			"-A: numero de caballos\n-B: longitud de"
			" la carrera\n-C: numero de apostadores\n"
			"-D: numero de ventanillas.\n");
		exit(EXIT_FAILURE);
	}
	
	nCaballos = atoi(argv[1]);
	longCarrera = atoi(argv[2]);
	nApostadores = atoi(argv[3]);
	nVentanillas = atoi(argv[4]);
	
	if(nCaballos > 10||nCaballos <= 0||longCarrera <= 0||
		nApostadores <= 0||nApostadores > 10||nVentanillas <= 0){
		perror("Alguno de los argumentos de entrada no es "
			"valido. \nEjecutar el programa sin argumentos "
			"para ver el comando de ejecucion");
		exit(EXIT_FAILURE);
	}

	/* Crea proceso monitor */
	ret = fork();
	if(ret < 0){
		/* Error */
		exit(EXIT_FAILURE);
	} else if(ret == 0) {
		/* Proceso hijo */
		exit(EXIT_SUCCESS);
	}
 
	/* Crea proceso gestor de apuestas */
	ret = fork();
	if(ret < 0){
		/* Error */
		exit(EXIT_FAILURE);
	} else if(ret == 0) {
		/* Proceso hijo */
		exit(EXIT_SUCCESS);
	}	
	/* Crea los procesos apostadores */
	ret = fork();
	if(ret < 0){
		/* Error */
		exit(EXIT_FAILURE);
	} else if(ret == 0) {
		/* Proceso hijo */
		exit(EXIT_SUCCESS);
	}
	/* Crea los caballos */
	ret = fork();
	if(ret < 0){
		/* Error */

		exit(EXIT_FAILURE);
	} else if(ret == 0) {
		/* Proceso hijo */
		exit(EXIT_SUCCESS);
	}

	while(wait(NULL)>0);

	exit(EXIT_SUCCESS);
}
