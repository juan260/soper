#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#define MAXBUF 1024


int main()
{
    int ret, file, i=0;
    char buffer[MAXBUF];
    char * word[]={"EL\n","PROCESO\n", "A\n", "ESCRIBE\n", "EN\n", "UN\n", "FICHERO\n", 
        "HASTA\n", "QUE\n", "LEE\n", "LA\n", "CADENA\n", "FIN\n"};
    
    file=open("fichero.txt", O_RDWR);
    if(file<0){
        perror("Error een la apertura del fichero\n");
        exit(EXIT_FAILURE);
        }

    while(i<50){
        /*Cambiamos la semilla cada vez que iniciamos un nuevo proceso*/
        srand(time(NULL));

        ret=fork();
        
        if(ret<0){
            perror("Error al hacer fork\n");
            while(wait(NULL)<0){}
            exit(EXIT_FAILURE);
        
        } else if (ret==0) {
            
            
            do{
                /*Escribimos al principio del fichero colocándonos al principio para borrar
                lo que hubiera*/

                lseek(file, 0, SEEK_SET);
                i=(int)(rand()%13);
                
                write(file, word[i], strlen(word[i])+1);
            }while(strcmp(word[i],"FIN\n"));
            
            close(file);
            exit(EXIT_SUCCESS);
        
        } else {
        
            
            while(i<50){
                lseek(file, 0, SEEK_SET);
                read(file, buffer, MAXBUF);
                i++;
                sleep(1);
                if(strcmp(buffer, "FIN\n")==0) break;
            }
            
            wait(NULL);
            close(file);
        }
    }
    
    exit(EXIT_SUCCESS);
}
