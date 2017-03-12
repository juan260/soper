#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#define NUMPROC 1

int main(int argc, char ** argv)
{	
	struct timeval ti, tf;
	double miliseconds; 
	int i, j, k, flag, ret, N, *primos;
    
    if(argc!=2){
        perror("Error en la entrada de argumentos. Se debe de escribir"
        "\n en linea de comandos:\n\n\t./ejercicio3a N\n\nDonde N es"
        " la cantidad de numeros primos a calcular");
        exit(EXIT_FAILURE);
        }
    N=atoi(argv[1]);
    primos=(int *)malloc(sizeof(int)*N);
	gettimeofday(&ti, NULL);

	for(i=0;i<NUMPROC;i++)
	{
		if((ret=fork())<0)
		{
			perror("Error al hacer fork");
			while(wait(NULL)>0){}
			exit(EXIT_FAILURE);
		} else if(ret) {
            primos[0]=1;
			for(j=1;j<N;j++){
                for(primos[j]=primos[j-1]+1;primos[j]<INT_MAX;primos[j]++){
                    flag=0;
                    for(k=1;k<primos[j];k++){
                        if(primos[j]%k){
                            flag=1;
                            break;
                    }
                    if(flag==0){
                        break;
                    }

                    }
                }
            }
            printf("\n\n");
            for(j=0;j<N;j++){
                  printf("%d ", primos[j]);         
            }
			exit(EXIT_SUCCESS);
		}
	}
	
	while(wait(NULL)>0){}
	gettimeofday(&tf, NULL);
	miliseconds = (tf.tv_sec-ti.tv_sec)*1000+(tf.tv_usec-ti.tv_usec)/1000.0;
	printf("Milisegundos transcurridos: %f\n", miliseconds);
	exit(EXIT_SUCCESS);
}
