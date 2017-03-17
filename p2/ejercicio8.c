/**
* @brief Fuente del ejercicio 8
* Este programa pide al usuario un número de procesos y un número de vueltas, estos procesos se pasarán
* una señal tantas vueltas como pida el usuario. Cada vez que uno la recibe, imprime la hora. Finalmente
* se acaban uno a uno.
* @file ejercicio8.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 1.0
* @date 15-03-2017
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

char *tiempo();

void  manejador_sigusr1(int sig);

void manejador_sigterm(int sig);


int main (int argc, char* argv[])
{
    int i, j, pid, numProcesos, numVueltas, rootpid;

    if (argc!=3){
        printf("\nNumero de argumentos incorrecto. \n\n"
            "El comando de ejecucion deberia ser:\n\n"
            "\t./ejercicio8 numProcesos numVueltas\n\n"
            "Donde numProcesos y numVueltas son enteros\n"
            "que indican, respectivamente el número de \n"
            "procesos a crear y el número de vueltas que darán.\n\n");
        exit(EXIT_FAILURE);
    }


    numProcesos = atoi(argv[1]);
    numVueltas = atoi(argv[2]);
    rootpid = getpid();

    if(signal(SIGUSR1, manejador_sigusr1)==SIG_ERR){
    perror("Error con la señal SIGUSR1");
    exit(EXIT_FAILURE);
    }
    
    if(signal(SIGTERM, manejador_sigterm)==SIG_ERR){
    perror("Error con la señal SIGTERM");
    exit(EXIT_FAILURE);
    }
    for (i=0; i<numProcesos; i++){
        if ((pid=fork()) <0 ){
            printf("Error haciendo fork\n");
            exit(EXIT_FAILURE);
        }else if (pid ==0){
            if(i==numProcesos-1){
                for (j=0; j<numVueltas; j++){
                    kill(rootpid, SIGUSR1);
                    pause();
                    sleep(2);
                }
                kill(rootpid, SIGTERM);
                pause();
                exit(EXIT_SUCCESS);
            }
        }else{
            for (j=0; j<numVueltas; j++){
                pause();
                sleep(2);
                kill(pid, SIGUSR1);
            }
            pause();
            kill(pid, SIGTERM);
            exit(EXIT_SUCCESS);
        }
    }
    exit(EXIT_SUCCESS);
}

char *tiempo() { 
    time_t tiempo = time(0); 
    struct tm *tlocal = localtime(&tiempo); 
    char *output=(char*)malloc(sizeof(char)*128); 
    strftime(output, 128, "%d/%m/%y %H:%M:%S", tlocal); 
    return output; 
}

void  manejador_sigusr1(int sig){
    printf("Hola PID=%d, time=%s\n", getpid(), tiempo());
    if(signal(SIGUSR1, manejador_sigusr1)==SIG_ERR){
        perror("Error con la señal SIGUSR1");
        exit(EXIT_FAILURE);
    }
}

void manejador_sigterm(int sig){
    printf("Muere PID=%d\n", getpid());
    if(signal(SIGTERM, manejador_sigusr1)==SIG_ERR){
        perror("Error con la señal SIGTERM");
        exit(EXIT_FAILURE);
    }
}
