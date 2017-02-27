#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main (int argc, char* argv[])
{
	int pid;
	int num_proc = argc-2;
	int i;
    char *prog[2];
	for (i=1; i <= num_proc; i++){
		if ((pid=fork()) <0 ){
			printf("Error haciendo fork\n");
			exit(EXIT_FAILURE);
		}else if (pid ==0){
			if (strcmp(argv[argc-1], "-l") == 0){
				execl(argv[i], argv[i], (char*)NULL);
			}
			if (strcmp(argv[argc-1], "-lp") == 0){
				execlp(argv[i], argv[i], (char*)NULL);
			}
			if (strcmp(argv[argc-1], "-v") == 0){
				prog[0] = argv[i];
                prog[1] = NULL;
				execv(argv[i], prog);
			}
			if (strcmp(argv[argc-1], "-vp") == 0){
				prog[0] = argv[i];
                prog[1] = NULL;
				execvp(argv[i], prog); 
			}  
		}    
	}
	wait(NULL);
	exit(EXIT_SUCCESS);
}
