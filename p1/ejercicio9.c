/**
* @brief Fuente del ejercicio 9
*
* Este programa pide al usuario una serie de argumentos por pantalla, después crea cuatro procesos hijo y establece
* una red de pipes con ellos. Cada uno de estos hijos toma argumentos a través de sus pipes, ejecuta una operación diferente,
* y devuelve el resultado a través de los pipes. Por último, el proceso padre lee estos resultados y los imprime por pantalla
* de la forma que haya indicado el usuario por parámetros.
* @file ejercicio9.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 1.0
* @date 02-03-2017
*/


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
    int pipePadreAHijo[4][2];
    int i, arg1, arg2, status, resultado, bytesLeidos;
    int pid[4];
    char op, mode;
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
            for(i=0;i<4;i++){
                close(pipeHijoAPadre[i][LEER]);
                close(pipeHijoAPadre[i][ESCRIBIR]);
            }
            exit(EXIT_FAILURE);
            }
    }

    for(i=0;i<4;i++){
        
        if((pid[i]=fork())==-1){
            perror("Error al hacer fork en la operación suma\n");
                for(i=0;i>=0;i--){
                    close(pipePadreAHijo[i][LEER]);
                    close(pipePadreAHijo[i][ESCRIBIR]);
            }
                for(i=0;i<4;i++){
                    close(pipeHijoAPadre[i][LEER]);
                    close(pipeHijoAPadre[i][ESCRIBIR]);
                }
                exit(EXIT_FAILURE);
                }
 
    

        else if((pid[i])==0){
            close(pipeHijoAPadre[i][LEER]);
            close(pipePadreAHijo[i][ESCRIBIR]);

            bytesLeidos = read(pipePadreAHijo[i][LEER], buffer, MAXBUF);
            sscanf(buffer, "%d,%d\n", &arg1, &arg2);

            if(bytesLeidos<3){
                sprintf(buffer, "Error en la lectura del pipe por parte de los hijos\n");
           } else if(i==suma){
                /*Caso de la suma*/
                resultado=arg1+arg2;
                sprintf(buffer, "Datos enviados a través de la tubería por el proceso PID=%d\n"
                "Operando 1: %d. Operando 2: %d. Suma: %d\n", getpid(), arg1, arg2, resultado);
            }else if(i==resta){
                /*Caso de la resta*/
                resultado=arg1-arg2;
                sprintf(buffer, "Datos enviados a través de la tubería por el proceso PID=%d\n"
                "Operando 1: %d. Operando 2: %d. Resta: %d\n", getpid(), arg1, arg2, resultado);

            }else if(i==multi){
                /*Caso de lamultiplicación*/
                resultado=arg1*arg2;
                sprintf(buffer, "Datos enviados a través de la tubería por el proceso PID=%d\n"
                "Operando 1: %d. Operando 2: %d. Multi: %d\n", getpid(), arg1, arg2, resultado);

            }else{
                /*Caso de la división*/
                if(arg2==0){
                    sprintf(buffer, "Error: division por cero");
                }else{
                    resultado=arg1/arg2;
                    sprintf(buffer, "Datos enviados a través de la tubería por el proceso PID=%d\n"
                    "Operando 1: %d. Operando 2: %d. Divi: %d\n", getpid(), arg1, arg2, resultado);
                }
            }
            write(pipeHijoAPadre[i][ESCRIBIR], buffer, (strlen(buffer)+1));
            exit(EXIT_SUCCESS);
        }
    }


 
    for(i=0;i<4;i++){
        close(pipeHijoAPadre[i][ESCRIBIR]);
    }
    

	/*Proceso padre*/

    close(pipePadreAHijo[suma][LEER]);
    printf("Desea:\n\ta) Escribir una única operación\n\tb) Escribir operandos y recibir todos los"
        " resultados de operación.\n\n\t[a, b]:  ");

    scanf("%c", &mode);
    if(mode=='a'||mode=='A'){
        printf("Escriba una operacion en el formato a op b (por ejemplo: '1 + 3' sin las comillas):\n\t");
        scanf("%d %c %d", &arg1, &op, &arg2);

    }else if(mode=='b'||mode=='B'){
        printf("Introduzca el primer operando: ");
        scanf("%d", &arg1);
        printf("Introduzca el segundo operando: ");
        scanf("%d", &arg2);
        printf("\n");
    }else{
        printf("Error en la entrada de argumentos, debe escoger entre introducir a o b\n");
        exit(EXIT_FAILURE);
    }
    sprintf(buffer, "%d,%d", arg1, arg2);
    
   
    for(i=0;i<4;i++){
        write(pipePadreAHijo[i][ESCRIBIR], buffer, (strlen(buffer)+1));
    }

   while(wait(&status)>0){} 
    
    if(mode=='a'||mode=='A'){
        if(op=='+')
            bytesLeidos = read(pipeHijoAPadre[suma][LEER], buffer, MAXBUF-1);

        else if(op=='-')
            bytesLeidos = read(pipeHijoAPadre[resta][LEER], buffer, MAXBUF-1);

        else if(op=='*')
            bytesLeidos = read(pipeHijoAPadre[multi][LEER], buffer, MAXBUF-1);

        else if(op=='/')
            bytesLeidos = read(pipeHijoAPadre[divi][LEER], buffer, MAXBUF-1);
        
        else{
            perror("Error en la entrada de argumentos");
            exit(EXIT_FAILURE);
        }

        printf("\n%s\n", buffer);
    }else {
        for(i=0;i<4;i++){
            read(pipeHijoAPadre[suma][LEER], buffer, MAXBUF-1);
            printf("%s\n", buffer);
        }
    }
    
    exit(EXIT_SUCCESS);
}
