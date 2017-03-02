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
