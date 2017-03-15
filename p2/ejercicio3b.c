/**
* @brief Fuente del ejercicio 3a
*
* Este programa crea un numero de hilos hijo (definido por NUMTHREAD)
* y cada uno de éstos llama a la funcion calculaPrimos, que recibe
* como argumento el número de primos a calcular, y calcula ese número 
* de primos.
* @file ejercicio3b.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 1.0
* @date 15-03-2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <pthread.h>

#define NUMTHREAD 100

/**
* Función calcula primos:
* Calcula un número entero de números primos que recibe por parámetros.
* @param (void*)(int arg): número de primos a calular
*/


void * calculaPrimos(void * arg){
    int N = *((int *)arg);

    int *primos, j, k, flag;
    
    primos=(int *)malloc(sizeof(int)*(N+1));
    primos[0]=2;
    for(j=1;j<N;j++){
        for(primos[j]=primos[j-1]+1;primos[j]<INT_MAX;primos[j]++){
            for(k=2;k<primos[j];k++){
                flag=0;
                if(primos[j]%k==0){
                    flag=1;
                    break;
                }
                if(flag==0){
                    break;
                }

            }
            if(flag==0){
                break;
            }
        }
    }
    free(primos);
 
    pthread_exit(NULL);
}

int main(int argc, char ** argv)
{	
	struct timeval ti, tf;
	double miliseconds; 
	int i, N;
    pthread_t thread[NUMTHREAD];    
    if(argc!=2){
        perror("Error en la entrada de argumentos. Se debe de escribir"
        "\n en linea de comandos:\n\n\t./ejercicio3b N\n\nDonde N es"
        " la cantidad de numeros primos a calcular");
        exit(EXIT_FAILURE);
        }
    N=atoi(argv[1]);
	gettimeofday(&ti, NULL);

	for(i=0;i<NUMTHREAD;i++)
	{
        if(pthread_create(&thread[i], NULL, calculaPrimos, (void*)(&N))){
            perror("Error al crear el thread\n");
            for(i--;i>0;i--){
                pthread_join(thread[i], NULL);
            }

            exit(EXIT_FAILURE);
        }
	}
	
	for(i=0;i<NUMTHREAD;i++){
        pthread_join(thread[i], NULL);
    }
	gettimeofday(&tf, NULL);
	miliseconds = (tf.tv_sec-ti.tv_sec)*1000+(tf.tv_usec-ti.tv_usec)/1000.0;
	printf("Milisegundos transcurridos: %f\n", miliseconds);
    exit(EXIT_SUCCESS);
}
