 /**
 * @brief Fuente del ejercicio 2
 *
 * Este programa reserva una zona dememoria compartida, y crea una serie de hijos,
 * tantos como se le indique por parámetros. Cada uno de los hijos altera la memoria 
 * compartida, envia una señal al padre y termina. El padre por su parte cada vez
 * que recibe esa señal, lee de la memoria compartida, imprime el contenido por 
 * pantalla y termina. Cabe destacar que este ejercicio tiene un fallo en su
 * planteamiento, al no controlar la concurrencia de ninguna manera.
 *
 * Para ejecutarse, se debe utilizar el comando:
 *
 * ./ejercicio2 N
 *
 * Donde N será el número de hijos a crear.
 *
 * @file ejercicio2.c
 * @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
 * @version 1.0
 * @date 06-abril-2017
 */

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

        
void  handler_sigusr1(){
    int key, sid;
    Info * info;
     
    key = ftok("fichero.txt", PROJID);
    
    if((sid = shmget(key, sizeof(Info), 
        IPC_CREAT | SHM_R | SHM_W))==-1){
        perror("Error al obtener la zona compartida de memoria.\n");
        exit(EXIT_SUCCESS);
    }
    
    info = shmat(sid, NULL, 0);
    
    if(info == (void*)-1){
        perror("Error al obtener la variable de memoria compartida.\n");
        shmctl(sid, IPC_RMID, (struct shmid_ds*)NULL);            
        exit(EXIT_FAILURE);
    }
    
    printf("%d, %s\n", info->id, info->nombre);
    
    if(shmdt((Info*)info)==-1){
        perror("Error al liberar la variable de memoria compartida");
        shmctl(sid, IPC_RMID, (struct shmid_ds*)NULL);
        exit(EXIT_FAILURE);
    }
 
    if(signal(SIGUSR1, handler_sigusr1)==SIG_ERR){
        perror("Error al ejecutar signal");
        while(wait(NULL)>=0){}
        exit(EXIT_FAILURE);
        }
}

int main(int argc, char * argv[]){
    int N, id, sid, key, i;
    Info *info;  
    char buffer[MAXBUF];
    time_t t;   
    

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

    for(i=0;i<N;i++){
        id=fork();
       if(id<0){
            perror("Error al hacer fork.\n");
            while(wait(NULL)>=0){} 
            exit(EXIT_SUCCESS);
        
        } else if(id==0) {
            /* Estamos en el proceso hijo */
            /* Le damos una semilla distinta a cada proceso hijo */
            srand(time(&t)*getpid());
             
            if((sid = shmget(key, sizeof(Info), 
                IPC_CREAT | SHM_R | SHM_W))==-1){
                perror("Error al obtener la zona compartida de memoria.\n");
                exit(EXIT_SUCCESS);
            }
    
            info = shmat(sid, NULL, 0);
            if(info == (void *)-1){
                perror("Error  al obtener la variable de memoria compartida.\n");
                exit(EXIT_FAILURE);
            }
            usleep(rand()%MAXUSEC);
            printf("\nIntroduzca el nombre de un cliente: ");
            scanf("%s", buffer);
            
            info->id++;
            strcpy(info->nombre, buffer);

            if(kill(getppid(), SIGUSR1)==-1){
                perror("Error al enviar la señal SIGUSR1");
            }

            if(shmdt((Info*)info)==-1){
                perror("Error al liberar la variable de memoria compartida");
                shmctl(sid, IPC_RMID, (struct shmid_ds*)NULL);
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        }
    }
    while(wait(NULL)>=0){}
   
    if((sid = shmget(key, sizeof(Info), 
        IPC_CREAT | SHM_R | SHM_W))==-1){
        perror("Error al obtener la zona compartida de memoria.\n");
        exit(EXIT_SUCCESS);
    }

    if(shmctl(sid, IPC_RMID, (struct shmid_ds*)NULL)==-1){
        perror("Error al eliminar la memoria compartida.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);

    
}
    
