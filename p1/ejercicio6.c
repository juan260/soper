#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
  #include <sys/types.h>
  #include <sys/wait.h>
int main(int argc, char** argv){
	char * cadena;
	int pid;
	cadena=(char*)malloc(sizeof(char)*80);
	pid=fork();
	if(pid<0){
		printf("Error al ejecutar fork()");
		exit(EXIT_FAILURE);	
	}else if(pid==0){
		strcpy(cadena, "Soy hijo\n");
			
	}else{
		strcpy(cadena, "Soy padre\n");
	}
	printf("%s", cadena);
	wait(NULL);
	exit(EXIT_SUCCESS);
}
