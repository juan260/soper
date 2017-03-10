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
