#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define LEER 0
#define ESCRIBIR 1
#define MAXBUF 100

enum OPERACION
{
    suma=0,
    resta=1,
    multi=2,
    divi=3
    };

int main(){
    int pipeHijoAPadre[4][2];
    int pipePadreAHijo[4][2];
    int i;
    int pid[4];
    int status;
    char buffer1[MAXBUF];
    char buffer2[MAXBUF];

    /*Abrimos la infraestructura de pipes:*/

    /*Primero los pipes con dirección de hijo a padre*/
    for(i=0;i<4;i++){
        if(pipe(pipeHijoAPadre[i])==-1){
            perror("Error al hacer pipe");
            i--;
            for(;i>=0;i--){
                close(pipeHijoAPadre[i][LEER]);
                close(pipeHijoAPadre[i][ESCRIBIR]);
            }
            exit(EXIT_FAILURE);
            }  
    }

    /*Ahora los pipes con dirección de padre a hijo*/
    for(i=0;i<4;i++){
        if(pipe(pipePadreAHijo[i])==-1){
            perror("Error al hacer pipe");
            i--;
            for(;i>=0;i--){
                close(pipePadreAHijo[i][LEER]);
                close(pipePadreAHijo[i][ESCRIBIR]);
            }
            exit(EXIT_FAILURE);
            }
    }

    
    if((pid[suma]=fork())==0){
        close(pipeHijoAPadre[suma][LEER]);
        close(pipePadreAHijo[suma][ESCRIBIR]);
        read(pipePadreAHijo[suma][LEER], buffer2, MAXBUF);

        printf("LEIDO : %s", buffer2);
        write(pipeHijoAPadre[suma][ESCRIBIR], buffer2, 5);
        exit(EXIT_SUCCESS);
    }
    
    if((pid[resta]=fork())==0){
        close(pipeHijoAPadre[resta][LEER]);
        close(pipePadreAHijo[resta][ESCRIBIR]);


        exit(EXIT_SUCCESS);
                        }

    if((pid[multi]=fork())==0){
        close(pipeHijoAPadre[multi][LEER]);
        close(pipePadreAHijo[multi][ESCRIBIR]);
        
        close(pipeHijoAPadre[multi][ESCRIBIR]);
        close(pipePadreAHijo[multi][LEER]);
        
        exit(EXIT_SUCCESS);
        }
    
   if((pid[divi]=fork())==0){
        close(pipeHijoAPadre[suma][LEER]);
        close(pipePadreAHijo[suma][ESCRIBIR]);
 
        exit(EXIT_SUCCESS);
    }

    
    for(i=0;i<4;close(pipeHijoAPadre[i][ESCRIBIR]), i++);
    for(i=0;i<4;close(pipePadreAHijo[i][LEER]), i++);
 

    
    scanf("%s", buffer1);

    write(pipePadreAHijo[suma][ESCRIBIR], buffer1, 5);
    waitpid(pid[suma], &status, 0);
    read(pipeHijoAPadre[suma][LEER], buffer2, MAXBUF);
    printf("\n%s\n", buffer2);
    exit(EXIT_SUCCESS);
}
