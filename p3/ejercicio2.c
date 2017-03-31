#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#define MAXBUF 500
#define PROJID 1300
/* Este define sirve para limitar la cantidad de tiempo aleatorio 
que esperael proceso hijo (Ya que podria ser demasiado alto) */
#define MAXUSEC 10000

typedef struct _info{
    char nombre[80];
    int id;
}Info;

void handler_sigusr1(){
    
    if(signal(SIGUSR1, handler_sigusr1)==SIG_ERR){
        perror("Error al ejecutar signal");
        while(wait(NULL)>=0){}
        exit(EXIT_FAILURE);
        }
}

int main(int argc, char * argv[]){
    int N, id, sid, key;
    Info *info;  
    char buffer[MAXBUF];
    time_t t;   
    
    /* Cambiamos la semilla de rand() */
    srand(time(&t));

    if(argc!=2){
        printf("Error en los argumentos de entrada."
        "\nLa entrada deberia ser:\n\n\t./ejercicio2 N\n\n"
        "Donde N es el número de procesos hijos a crear.\n\n");
        exit(EXIT_FAILURE);
    }

    N=atoi(argv[1]);
    if(signal(SIGUSR1,handler_sigusr1)==SIG_ERR){
        perror("Error al ejecutar signal()");
        exit(EXIT_FAILURE);
    }

    key = ftok("fichero.txt", PROJID);
    
    if((sid = shmget(key, sizeof(Info), 
        IPC_EXCL | IPC_CREAT | SHM_R | SHM_W))==-1){
        perror("Error al obtener la zona compartida de memoria.\n");
        exit(EXIT_SUCCESS);
    }
    
    info = shmat(sid, NULL, 0);
    
    if(info==NULL){
        perror("Error al reservar la memoria compartida.\n");
        exit(EXIT_FAILURE);
    }

    printf("\nVamos con el fork\n");
    
    id=fork();
        if(signal(SIGUSR1,handler_sigusr1)==SIG_ERR){
                perror("Error al ejecutar signal()");
                        exit(EXIT_FAILURE);
                            }

    if(id<0){
        perror("Error al hacer fork.\n");
        while(wait(NULL)>=0){} 
        exit(EXIT_SUCCESS);
        
    } else if(id>0) {
        /* Estamos en el proceso hijo */
        info = shmat(sid, NULL, 0);
        printf("Esperando\n");
        usleep(rand()%MAXUSEC);
        printf("\nIntroduzca el nombre de un cliente: ");
        scanf("%s", buffer);
            
        info->id++;
        strcpy(info->nombre, buffer);

        kill(getppid(), SIGUSR1);
        shmdt((Info*)info);
        shmctl(sid, IPC_RMID, (struct shmid_ds*)NULL); 
        exit(EXIT_SUCCESS);
    }

    while(wait(NULL)>=0){}
    shmctl(sid, IPC_RMID, (struct shmid_ds*)NULL);
    exit(EXIT_SUCCESS);

    
}
    
