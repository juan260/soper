#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define MAXBUF 200


int main()
{
    int ret, fileread, filewrite, i=0;
    char buffer[MAXBUF];
    char word[][]={"EL", "PROCESO"};
    
    while(i<50){
        ret=fork();
        if(ret<0){
            perror("Error al hacer fork\n");
            while(wait(NULL)<0){}
            exit(EXIT_FAILURE);
        } else if (ret==0) {
            filewrite=open("fichero.txt", O_WRONLY, O_CREAT);
            do{
                i=(int)(rand()%2);
            }while(strcmp(word[i],"FIN"))
            close(filewrite);
            exit(EXIT_SUCCESS);
        } else {
            fileread=open("fichero.txt", O_RDONLY, O_CREAT);
            for(;i<50;i++){
                read(fileread, buffer, MAXBUF);
                sleep(5);
                if(strcmp(buffer, "FIN")==0) break;
            }
            wait(NULL);
            close(fileread);
        }
    }
    

}
