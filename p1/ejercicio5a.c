#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define NUM_PROC 3
int main (void)
{
    int pid;
    int i;
    int status;
    for (i=0; i < NUM_PROC; i++){
        if ((pid=fork()) <0 ){
            printf("Error haciendo fork\n");
            exit(EXIT_FAILURE);
        }else if (pid ==0){
            printf("HIJO %d\n", i);
        	exit(EXIT_SUCCESS);
	}else{
            printf ("PADRE %d\n", i);
            wait(&status);
            exit(EXIT_SUCCESS);
        }
    
    }
    }
