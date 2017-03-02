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
