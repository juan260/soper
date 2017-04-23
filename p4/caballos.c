#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "semaforos.h"
#include "caballo.h"
#define MAXBUF 100
#define PROJID 1245

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

int main(int argc, char * argv[]){

	/* Declaracion de variables */
	int nCaballos, longCarrera, nApostadores, nVentanillas, ret,
		keySem, i, j;
	int ** pipePadreACaballo;
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


	/* Declaracion de variables de memoria compartida */

	/* Esta variable tendrá los segundos restantes hasta
	que empiece la carrera, cuando llegue a 0, todo empieza
	a funcionar. Cuando tome el valor -1 la carrera habra
	terminado y seguira bajando hasta llegar a -15, momento
	en el cual el monitor sabrá que tiene que mostrar todas
	las estadisticas */
	int tiempo;

	/* Array de ints con la posicion de todos los caballos */
	int * posicionCaballo;

	/* Array arrays de ints con la cantidad apostada a cada caballo
		por cada apostador.*/
	int ** matrizApuestas;


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
	/* Creacion de los medios de comunicacion entre el
	proceso padre y los caballos, asi como los semaforos */
	pipePadreACaballo = pipesCaballos(nCaballos);
	posicionCaballo = (int*)malloc(sizeof(int)*nCaballos);
	for(i=0;i<nCaballos;i++){
		posicionCaballo[i]=0;
	}
	keySem = ftok("a.txt", PROJID);
	Crear_Semaforo(keySem, 1, &semCaballos);

	/* Crea proceso monitor */

	/* Crea proceso gestor de apuestas */

	/* Crea los procesos apostadores */

	/* Crea los caballos */
	if(caballos(nCaballos, pipePadreACaballo, semCaballos)==CERROR){
		while(wait(NULL)>0);
		exit(EXIT_FAILURE);
	}

	/* Ir bajando la variable tiempo de 15 a 0 */
	
	/* Bucle que va despertando a los caballos y les
	va enviando y recibiendo las posiciones de los
	caballos */
	while(1 /* La carrera no haya terminado,
		es decir, mientras la variable tiempo valga 0 */){
		sleep(1);
		for(i=0;i<nCaballos;i++){
			for(j=0;j<nCaballos;j++){
				sprintf(buffer, "%d ", posicionCaballo[j]);
			}
			write(pipePadreACaballo[i][ESCRIBIR], buffer, strlen(buffer)-1);
			Up_Semaforo(semCaballos, i, SEM_UNDO);
			/* Recibir el mensaje y actualizar posicionCaballo */
		}
	}

	/*Borrar_Semaforo(mutex);
	Borrar_Semaforo(semCaballos);
	freePipesCaballos(pipePadreACaballo);*/
	while(wait(NULL)>0);

	exit(EXIT_SUCCESS);
}
