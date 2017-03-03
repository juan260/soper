#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>
#define NUM_PROC 3
int main (void)
    {
        int pid;
        int i;
        printf("id superpadre = %d\n", getpid());
        for (i=0; i < NUM_PROC; i++){
            if ((pid=fork()) <0 ){
                printf("Error al emplear fork\n");
                exit(EXIT_FAILURE);
            }else if (pid ==0){
                printf("HIJO: pid %d, pid del padre %d\n", getpid(), getppid());
            }
        }
        wait(NULL);
        exit(EXIT_SUCCESS);

                                         }
