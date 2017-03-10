#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{	
	clock_t start;
	clock_t end;
	double seconds; 
	int i, ret;
	start=clock();
	for(i=0;i<100;i++)
	{
		if((ret=fork())<0)
		{
			perror("Error al hacer fork");
			while(wait(NULL)>0){}
			exit(EXIT_FAILURE);
		} else if(ret) {
							
			exit(EXIT_SUCCESS);
		}
	}
	
	while(wait(NULL)>0){}
	end=clock();
	seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Milisegundos transcurridos: %f\n",(double) seconds*(double)1000);
	exit(EXIT_SUCCESS);
}
