/**
* @brief Fuente del ejercicio 6
*
* Este programa reserva memoria para una cadena, y hace un fork. En cada proceso
* resultante del fork almacena un valor en la cadena (uno pedido al usuario por línea de comandos
* y otro ya elegido) y muestra, finalmente, los dos valores, demostrando que no tienen relación entre ellos.
* @file ejercicio6.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 1.0
* @date 02-03-2017
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char** argv){
	char * cadena;
	int pid, status;
	cadena=(char*)malloc(sizeof(char)*80);
	pid=fork();
	if(pid<0){
		printf("Error al ejecutar fork()");
		free(cadena);
        exit(EXIT_FAILURE);	
	}else if(pid==0){
		printf("\nEscriba una cadena para almacenar en la variable del proceso hijo: \n");
		scanf("%s", cadena);	
		printf("Ahora se imprimira el contenido de 'cadena' en el proceso padre e hijo para ver si coinciden:\n");
	}else{
		
		wait(&status);
		strcpy(cadena, "Soy padre\n");
	}
	printf("%s\n", cadena);
    	free(cadena);
	exit(EXIT_SUCCESS);
}
