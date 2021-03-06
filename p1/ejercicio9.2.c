#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define LEER 0
#define ESCRIBIR 1
#define MAXBUF 250

enum OPERACION
{
    suma=0,
    resta=1,
    multi=2,
    divi=3
    };

int main(){
    int pipeHijoAPadre[4][2];
    int pipePadreAHijo[2];
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
    if(pipe(pipePadreAHijo)==-1){
        perror("Error al hacer pipe\n");
       for(i=0;i<4;i++){
                close(pipeHijoAPadre[i][LEER]);
                close(pipeHijoAPadre[i][ESCRIBIR]);
            }
        
        exit(EXIT_FAILURE);
    }



    /*FORK DE LA SUMA*/
    if((pid[suma]=fork())==-1){
        perror("Error al hacer fork\n");
        for(i=0;i<4;i++){
            close(pipeHijoAPadre[i][LEER]);
            close(pipeHijoAPadre[i][ESCRIBIR]);

        }
        close(pipePadreAHijo[LEER]);
        close(pipePadreAHijo[ESCRIBIR]);
        exit(EXIT_FAILURE);
    }

    else if((pid[suma])==0){
        close(pipeHijoAPadre[suma][LEER]);
        close(pipePadreAHijo[ESCRIBIR]);
        
        bytesLeidos = read(pipePadreAHijo[LEER], buffer, MAXBUF);
        sscanf(buffer, "%d,%d\n", &arg1, &arg2);
        resultado=arg1+arg2;
        sprintf(buffer, "Datos enviados a través de la tubería por el proceso PID=%d\n"
        "Operando 1: %d. Operando 2: %d. Suma: %d\n", getpid(), arg1, arg2, resultado);

        write(pipeHijoAPadre[suma][ESCRIBIR], buffer, (strlen(buffer)+1));
        exit(EXIT_SUCCESS);
    }




    /*FORK DE LA RESTA*/
/*    
    if((pid[resta]=fork())==-1){
        perror("Error al hacer fork\n");
        for(i=0;i<4;i++){
            close(pipeHijoAPadre[i][LEER]);
            close(pipeHijoAPadre[i][ESCRIBIR]);
        }
        close(pipePadreAHijo[LEER]);
        close(pipePadreAHijo[ESCRIBIR]);
        exit(EXIT_FAILURE);
    }
    else if((pid[resta])==0){
        close(pipeHijoAPadre[resta][LEER]);
        close(pipePadreAHijo[ESCRIBIR]);
        bytesLeidos = read(pipePadreAHijo[LEER], buffer, MAXBUF);
        sscanf(buffer, "%d,%d\n", &arg1, &arg2);
        
        resultado=arg1-arg2;
        sprintf(buffer, "Datos enviados a través de la tubería por el proceso PID=%d\n"
        "Operando 1: %d. Operando 2: %d. Resta: %d\n", getpid(), arg1, arg2, resultado);
        write(pipeHijoAPadre[resta][ESCRIBIR], buffer, (strlen(buffer)+1));
        exit(EXIT_SUCCESS);
    }
*/


    /*FORK DE LA MULTIPLICACION*/
/*
    if((pid[multi]=fork())==-1){
        perror("Error al hacer fork\n");
        for(i=0;i<4;i++){
            close(pipeHijoAPadre[i][LEER]);
            close(pipeHijoAPadre[i][ESCRIBIR]);
        }
        close(pipePadreAHijo[LEER]);
        close(pipePadreAHijo[ESCRIBIR]);
        exit(EXIT_FAILURE);
    }
    else if((pid[multi])==0){
        close(pipeHijoAPadre[multi][LEER]);
        close(pipePadreAHijo[ESCRIBIR]);
        bytesLeidos = read(pipePadreAHijo[LEER], buffer, MAXBUF);
        sscanf(buffer, "%d,%d\n", &arg1, &arg2);
        resultado=arg1*arg2;
        sprintf(buffer, "Datos enviados a través de la tubería por el proceso PID=%d\n"
        "Operando 1: %d. Operando 2: %d. Resta: %d\n", getpid(), arg1, arg2, resultado);
        write(pipeHijoAPadre[multi][ESCRIBIR], buffer, (strlen(buffer)+1));
        exit(EXIT_SUCCESS);
    }
*/

    /*FORK DE LA DIVISION*/
/*
    if((pid[divi]=fork())==-1){
        perror("Error al hacer fork\n");
        for(i=0;i<4;i++){
            close(pipeHijoAPadre[i][LEER]);
            close(pipeHijoAPadre[i][ESCRIBIR]);
        }
        close(pipePadreAHijo[LEER]);
        close(pipePadreAHijo[ESCRIBIR]);
        exit(EXIT_FAILURE);
    }
    else if((pid[divi])==0){
        close(pipeHijoAPadre[divi][LEER]);
        close(pipePadreAHijo[ESCRIBIR]);
        bytesLeidos = read(pipePadreAHijo[LEER], buffer, MAXBUF);
        sscanf(buffer, "%d,%d\n", &arg1, &arg2);
        resultado=arg1/arg2;
        sprintf(buffer, "Datos enviados a través de la tubería por el proceso PID=%d\n"
        "Operando 1: %d. Operando 2: %d. Resta: %d\n", getpid(), arg1, arg2, resultado);
        write(pipeHijoAPadre[divi][ESCRIBIR], buffer, (strlen(buffer)+1));
        exit(EXIT_SUCCESS);
    }
 */
    for(i=0;i<4;i++){
        close(pipeHijoAPadre[i][ESCRIBIR]);
    }
    
    close(pipePadreAHijo[LEER]);
    printf("Escriba una operacion en el formato a op b (por ejemplo: '1 + 3' sin las comillas):\n\t");
    scanf("%d %c %d", &arg1, &op, &arg2);


    sprintf(buffer, "%d,%d", arg1, arg2);
    
    if(op=='/' && arg2==0){
            perror("Error: división por cero");
            exit(EXIT_FAILURE);
    }

    write(pipePadreAHijo[ESCRIBIR], buffer, (strlen(buffer)+1));
    
    for(i=0;i<4;i++){
        wait(&status);
    }
    
    
    if(op=='+')
        bytesLeidos = read(pipeHijoAPadre[suma][LEER], buffer, MAXBUF-1);

    else if(op=='-')
        bytesLeidos = read(pipeHijoAPadre[resta][LEER], buffer, MAXBUF-1);

    else if(op=='*')
        bytesLeidos = read(pipeHijoAPadre[multi][LEER], buffer, MAXBUF-1);

    else if(op=='/'){
       
        bytesLeidos = read(pipeHijoAPadre[divi][LEER], buffer, MAXBUF-1);
    }
    else{
        perror("Error en la entrada de argumentos");
        exit(EXIT_FAILURE);

    }

    printf("\n%s\n", buffer);
    exit(EXIT_SUCCESS);
}
