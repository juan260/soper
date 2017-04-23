#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef struct Mensaje{
	long mtype;
	char mensaje[4096]; /*4kB como máximo a un byte por caracter*/
}Mensaje;

int main(int argc, char* argv[]) {
    key_t keyMen, keyMem;
    int msqid, memid, ret1, ret2, *contadores, i, size;
    char buffer[4096];
    
    if(argc!=3){
        perror("Error en los argumentos de entrada: Arg1 tiene que ser fichero origen y Arg2 tiene que ser fichero de destino");
        exit(EXIT_FAILURE);
    }
    
    keyMen = ftok ("/bin/ls", 21);
    if (keyMen == (key_t) -1){
        perror("Error al obtener clave para cola mensajes\n");
        exit(EXIT_FAILURE);
    }

    keyMem = ftok ("/bin/ls", 45);
    if (keyMem == (key_t) -1){
        perror("Error al obtener clave para memoria compartida\n");
        exit(EXIT_FAILURE);
    }
    
    msqid = msgget(keyMen, 0600|IPC_CREAT);
    if(msqid == -1){
    	perror("Error al obtener identificador de la cola de mensajes");
    	exit(EXIT_FAILURE);
    }

    memid = shmget(keyMem, sizeof(int)*3, IPC_CREAT | SHM_R | SHM_W);
    if(memid==-1){
    	perror("Error en la creación de memoria");
    	msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
    	shmctl (memid, IPC_RMID, (struct shmid_ds *)NULL);
    	exit(EXIT_FAILURE);
    }

    contadores = (int*) shmat(memid, NULL, 0);
    if(contadores==NULL){
    	perror("Error en la asignación de memoria");
    	msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
    	shmctl (memid, IPC_RMID, (struct shmid_ds *)NULL);
    	exit(EXIT_FAILURE);
    }

    contadores[0] = 0;
    contadores[1] = 0;
    contadores[2] = 0;


    if((ret1 = fork()) < 0){
    	perror("Error en el primer fork");
    	msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
    	exit(EXIT_FAILURE);
    }
    if(ret1==0){
    	if((ret2 = fork()) < 0){
    		perror("Error en el segundo fork");
    		msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
    		exit(EXIT_FAILURE);
    	}
    	if (ret2 == 0){
    		Mensaje send;
    		FILE *f1 = fopen(argv[1], "r");
    		if (f1 == NULL){
    			perror("Error abriendo archivo de origen");
    			contadores[0]=1;
    			exit(EXIT_FAILURE);
    		}
    		send.mtype=1;
    		while ((size = fread(buffer, 1, 4096, f1)) !=0){
    			if (size!=4096){
    				buffer[size]=4;
    			}
    			strcpy(send.mensaje, buffer);
    			msgsnd(msqid, (struct msgbuf*) &send, sizeof(Mensaje)-sizeof(long), 0);
    			contadores[1]++;
    		}
    		contadores[0]=1;
    		fclose(f1);
    		exit(EXIT_SUCCESS);
    	}
    	else{
    		while(contadores[0]!=1 || contadores[1]!=0){
    			Mensaje send, rcv;
    			msgrcv(msqid, (struct msgbuf*)&rcv, sizeof(Mensaje)-sizeof(long), 1, 0);
    			for(i=0; i<4096; i++){
    				if (rcv.mensaje[i]>96 && rcv.mensaje[i]<123){
    					buffer[i] = rcv.mensaje[i]-32;
    				}
    				else{
    					buffer[i] = rcv.mensaje[i];
    				}
    			} 
    			send.mtype=2;
    			strcpy(send.mensaje, buffer);
    			msgsnd(msqid, (struct msgbuf*) &send, sizeof(Mensaje)-sizeof(long), 0);
    			contadores[1]--;
    			contadores[2]++;
    		}
    		wait(NULL);
    		exit(EXIT_SUCCESS);
    	}
    }
    else{
    	FILE *f2=fopen(argv[2], "w");
    	if(f2 == NULL){
    		perror("Error abriendo segundo archivo");
    		wait(NULL);
    		shmdt ((void *)contadores);
			shmctl (memid, IPC_RMID, (struct shmid_ds *)NULL);
			msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
			fclose(f2);
    		exit(EXIT_FAILURE);		
    	}
    	while(contadores[0]!=1 || contadores[2]!=0){
    		Mensaje rcv;    		
    		msgrcv(msqid, (struct msgbuf*)&rcv, sizeof(Mensaje)-sizeof(long), 2, 0);
    		fflush(f2);
    		for(i=0; i<4096; i++){
    			if(rcv.mensaje[i]==4){
    				break;
    			}
    			fprintf(f2, "%c", rcv.mensaje[i]);
    		}
    		contadores[2]--;
    	}
    	wait(NULL);
    	shmdt ((void *)contadores);
		shmctl (memid, IPC_RMID, (struct shmid_ds *)NULL);
		msgctl (msqid, IPC_RMID, (struct msqid_ds *)NULL);
		fclose(f2);
    	exit(EXIT_SUCCESS);
    }
}

