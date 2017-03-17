/**
* @brief Fuente del ejercicio 10, versión2
*
* Este es el ejercicio 10, pero una segunda versión que imprime
* cada lectura y cada escritura cada vez que se ejecuta, 
* ya que no se especifica ninguna salida por pantalla.
* Por lo demás no se diferencia en nada del fuente "ejercicio10.c"
* @file ejercicio10b.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 2.0
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
#define MAXBUF 256

/**
* Este es el manejador de la señal de alarma de reloj (SIGALRM)i
* que no tiene nada, ya que nos interesa que simplemente
* espere a la llegada de la señal y continúe su ejecución.
*/

void manejador_sigalrm(int sig){
}


int main()
{
    int ret, i=0;
    FILE * read, * write;
    char buffer[MAXBUF];
    char * word[]={"EL","PROCESO", "A", "ESCRIBE", "EN", "UN", "FICHERO", 
        "HASTA", "QUE", "LEE", "LA", "CADENA", "FIN"};
    sigset_t * set; 
    while(i<50){
        /*Cambiamos la semilla cada vez que iniciamos un nuevo proceso*/
        srand(time(NULL));

        ret=fork();
        
        if(ret<0){
            perror("Error al hacer fork\n");
            while(wait(NULL)<0){}
            exit(EXIT_FAILURE);
        
        } else if (ret==0) {
            /* Si estamos en el caso hijo */    
            write=fopen("fichero.txt", "w");
            if(write==NULL){
                perror("Error en la apertura del fichero\n");
                exit(EXIT_FAILURE);
            }


            do{
                i=(int)(rand()%13);
                fprintf(write, "%s ", word[i]);
                printf("Escrito: '%s'\n", word[i]);
            }while(strcmp(word[i],"FIN")!=0);
            
            fclose(write);
            exit(EXIT_SUCCESS);
        
        } else {
            
            /* Estamos en el caso del padre */
            read=fopen("fichero.txt", "r");
            if(read==NULL){
                perror("Error en la apertura del fichero\n");
                exit(EXIT_FAILURE);
            }

            while(i<50){
                if(signal(SIGALRM, manejador_sigalrm)==SIG_ERR){
                    perror("Error al capturar la señal SIGALRM\n");
                    exit(EXIT_FAILURE);
                }
                
                /* Creamos una máscara que ignore cualquier señal
                excepto SIGINT por si el usuario quiere terminar
                la ejecucuión con ctrl+c, y excepto SIGALRM por si
                recibimos la alarma que estábamos esperando. 
                
                Esto lo hemos hecho para usar máscaras en el ejercicio,
                pero realmente, las proximas 5 líneas de código se podrían 
                sustituir por:
                    
                    alarm(5);
                    pause();

                y tendrían un efecto similar, con la diferencia de que en el caso
                de usar pause(), podríamos recibir cualquier señal durante
                nuestra espera lo cual, según bajo qué punto de vista lo miremos
                sería mejor.    
                */

                sigfillset(set);
                sigdelset(set, SIGINT);
                sigdelset(set, SIGALRM);
                alarm(5); 
                sigsuspend(set);

                /* Comprobamos que no estamos el final del fichero para no hacer
                la suposición de que el hijo ha escrito en los 5 segundos de espera,
                aunque podemos estar prácticamente seguros de que lo ha hecho */
                
                if(feof(read)){
                    printf("Final de fichero");
                    continue;
                }

                fscanf(read, "%s ", buffer);
                printf("\tLEIDO: %s\n", buffer);
                i++;
  
                if(strcmp(buffer, "FIN")==0) break;
            }
            
            wait(NULL);
            fclose(read);
        }
    }
    
    exit(EXIT_SUCCESS);
}
