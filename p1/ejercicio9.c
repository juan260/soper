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
    int i, arg1, arg2, status, resultado, bytesLeidos;
    int pid[4];
    char op;
    char buffer[MAXBUF];

    /*Abrimos la infraestructura de pipes:*/

    /*Primero los pipes con dirección de hijo a padre*/
    for(i=0;i<4;i++){
        if(pipe(pipeHijoAPadre[i])==-1){
            perror("Error al hacer pipe\n");
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
            perror("Error al hacer pipe\n");
            i--;
            for(;i>=0;i--){
                close(pipePadreAHijo[i][LEER]);
                close(pipePadreAHijo[i][ESCRIBIR]);
            }
            exit(EXIT_FAILURE);
            }
    }

    if((pid[suma]=fork())==-1){
        perror("Error al hacer fork\n");
        for(i=0;i<4;i++){
            close(pipePadreAHijo[i][LEER]);
            close(pipePadreAHijo[i][ESCRIBIR]);
            close(pipeHijoAPadre[i][LEER]);
            close(pipeHijoAPadre[i][ESCRIBIR]);

        }
        exit(EXIT_FAILURE);
    }

    else if((pid[suma])==0){
/*        close(pipeHijoAPadre[suma][LEER]);
        close(pipePadreAHijo[suma][ESCRIBIR]);
  */      
        bytesLeidos = read(pipePadreAHijo[suma][LEER], buffer, MAXBUF);
        buffer[strlen(buffer)]=0;
        sscanf(buffer, "%d,%d\n", &arg1, &arg2);
        resultado=arg1+arg2;
        sprintf(buffer, "Datos enviados a través de la tubería por el proceso PID=%d\n"
        "Operando 1: %d. Operando 2: %d. Suma: %d\n", getpid(), arg1, arg2, resultado);
        buffer[strlen(buffer)]=0;

        write(pipeHijoAPadre[suma][ESCRIBIR], buffer, strlen(buffer));
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
        
                
        exit(EXIT_SUCCESS);
        }
    
   if((pid[divi]=fork())==0){
        close(pipeHijoAPadre[divi][LEER]);
        close(pipePadreAHijo[divi][ESCRIBIR]);
 
        exit(EXIT_SUCCESS);
    }

    /*
    for(i=0;i<4;i++){
        close(pipeHijoAPadre[i][ESCRIBIR]);
        close(pipePadreAHijo[i][LEER]);
    }
*/
    printf("Escriba una operacion en el formato a op b (por ejemplo: '1 + 3' sin las comillas):\n\t");
    scanf("%d %c %d", &arg1, &op, &arg2);
    sprintf(buffer, "%d,%d\n", arg1, arg2);
    buffer[strlen(buffer)]=0;
    
    if(op=='/'){
        if(arg2==0){
            perror("Error: división por cero");
            exit(EXIT_FAILURE);
        }
    }

    write(pipePadreAHijo[suma][ESCRIBIR], buffer, 5);
    
    for(i=0;i<4;i++){
        waitpid(pid[i], &status, 0);
    }
    
    
    if(op=='+')
        bytesLeidos = read(pipeHijoAPadre[suma][LEER], buffer, MAXBUF);

    else if(op=='-')
        bytesLeidos = read(pipeHijoAPadre[resta][LEER], buffer, MAXBUF);

    else if(op=='*')
        bytesLeidos = read(pipeHijoAPadre[multi][LEER], buffer, MAXBUF);

    else if(op=='/'){
       
        bytesLeidos = read(pipeHijoAPadre[divi][LEER], buffer, MAXBUF);
    }
    else{
        perror("Error en la entrada de argumentos");
        exit(EXIT_FAILURE);

    }
    buffer[strlen(buffer)]=0;

    printf("\n%s\n%d %d\n", buffer, bytesLeidos, (int)strlen(buffer));
    exit(EXIT_SUCCESS);
}
