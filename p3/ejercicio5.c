#include <stdio.h>
#include <stdlib.h>
#include "semaforos.h"
#define PROJID 1301
#define UNIQUEPATH "fichero.txt"
/* Este define sirve para establecer el número de semáforos
   que se van a crear. No debe ser inferior a 3*/
#define SEMNUM 5

int main(){
    
    int error, semid, ret, key, i, activeArray[SEMNUM];
    
    short unsigned int array[SEMNUM];
    union semun {
        int val;
        struct semid_ds *semstat;
        unsigned short *array;
    } arg1, arg2;
    
    if(SEMNUM<3){
        perror("El define SEMNUMno debe ser inferior a 3.\n");
        exit(EXIT_FAILURE);
    }

    arg1.array=(unsigned short*)malloc(sizeof(unsigned short)*SEMNUM);
    
    if(arg1.array==NULL){
        exit(EXIT_FAILURE);
    }
    
    /* Inicializamos los valores del array, los usaremos mas adelante */   for(i=0;i<SEMNUM;i++){
        array[i]=0;
    }

    printf("\nIniciando test de semaforos.\n");
    
    printf("Probando la creación de semaforos.\n");
    key = ftok(UNIQUEPATH, PROJID);
    ret = Crear_Semaforo(key, SEMNUM, &semid);
    if(ret==ERROR){
        perror("Error en la creación del semáforo.\n");
        free(arg1.array);
        exit(EXIT_FAILURE);
    } else if (ret == 1) {
        perror("El semáforo ya estaba creado.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
    }
    
    /* Probamos que el return de esta segunda llamada es 1
    porque el semáforo ya estaba creado */
    ret = Crear_Semaforo(key, SEMNUM, &semid);
    if(ret==ERROR){
        perror("Error en la obtención del semáforo.\n");
        free(arg1.array);
        exit(EXIT_FAILURE);
    } else if (ret == 0) {
        perror("El semáforo no estaba creado, y deberia estarlo.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }


    printf("Semaforo creado.\n");
    if(semctl(semid, 0, GETALL, arg1)==-1){
        perror("Error al obtener valores");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }
    
    printf("Comprobando que el semáforo está bien inicializado.\n"); 
    for(i=0;i<SEMNUM;i++){
        if(arg1.array[i]!=0){
            printf("El semaforo %d no está bien inicializado, tenia el valor %hu\n", i, arg1.array[i]);
            free(arg1.array);
            Borrar_Semaforo(semid);
            exit(EXIT_FAILURE);
        }
    }
    
    printf("Probando la inicializacion de semaforos.\n");    
    /* Vamos a cambiar los valores de los semaforos 0 y 2 */
    /* La variable 'array' es un array de unsigned short inicializado a 0 al principio del programa */
    array[0]=1;
    array[2]=5;
    Inicializar_Semaforo(semid, array);
    
    if(semctl(semid, 0, GETALL, arg1)==-1){
        perror("Error al obtener valores.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }

    printf("Comprobando que el semáforo está bien inicializado.\n");
    /* Empezamos comprobando los dos semáforos que hemos cambiado */
    if(arg1.array[0]!=1||arg1.array[2]!=5){
        printf("Fallo en la inicialización de semáforos\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }
    
    /* Ahora comprobamos el resto */
    for(i=0;i<SEMNUM;i++){
        if(i==0||i==2)
            continue;
        if(arg1.array[i]!=0){
            printf("El semaforo %d no está bien inicializado, tenia el valor %hu\n", i, arg1.array[i]);
            free(arg1.array);
            Borrar_Semaforo(semid);
            exit(EXIT_FAILURE);
        }
    }

    printf("Probando a hacer up de un semáforo.\n");
    if(Up_Semaforo(semid, 0, SEM_UNDO)==ERROR){
        perror("Error al hacer up del semáforo 0 del array.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }
    
    printf("Probando a hacer otro up de el mismo semáforo.\n");
    if(Up_Semaforo(semid, 0, SEM_UNDO)==ERROR){
        perror("Error al hacer otro up del semáforo 0 del array.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }
    
    printf("Probando a hacer up de otro semáforo.\n");

    if(Up_Semaforo(semid, 1, SEM_UNDO)==ERROR){
        perror("Error al hacer up del semáforo 1 del array.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }

    printf("Comprobando que el semáforo tiene los valores correctos.\n");
    if(semctl(semid, 0, GETALL, arg1)==-1){
        perror("Error al obtener valores.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    } 
    
        /* Empezamos comprobando los dos semáforos que hemos cambiado */
    if(arg1.array[0]!=3||arg1.array[1]!=1||arg1.array[2]!=5){
        printf("Fallo al hacer los up's.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }

    /* Ahora comprobamos el resto */
    for(i=3;i<SEMNUM;i++){
        if(arg1.array[i]!=0){
            printf("El semaforo %d no está bien inicializado, tenia el valor %hu\n", i, arg1.array[i]);
            free(arg1.array);
            Borrar_Semaforo(semid);
            exit(EXIT_FAILURE);
        }
    }
    
    /* Probamos ahora Up multiple */
    printf("Probando Up multiple.\n");

    for(i=0;i<SEMNUM;i++){
        activeArray[i]=i;
    }

    if(UpMultiple_Semaforo(semid, SEMNUM, SEM_UNDO, activeArray)==ERROR){
        perror("Error al hacer up múltiple del semáforo.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }
    
 
    printf("Comprobando que el semáforo tiene los valores correctos.\n");
    if(semctl(semid, 0, GETALL, arg1)==-1){
        perror("Error al obtener valores.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    } 
        /* Empezamos comprobando los dos semáforos que hemos cambiado */
    if(arg1.array[0]!=4||arg1.array[1]!=2||arg1.array[2]!=6){
        printf("Fallo al hacer los up's.\n");
        free(arg1.array);
        Borrar_Semaforo(semid);
        exit(EXIT_FAILURE);
    }

    /* Ahora comprobamos el resto */
    for(i=3;i<SEMNUM;i++){
        if(arg1.array[i]!=1){
            printf("El semaforo %d no está bien inicializado, tenia el valor %hu\n", i, arg1.array[i]);
            free(arg1.array);
            Borrar_Semaforo(semid);
            exit(EXIT_FAILURE);
        }
    }
   
    /* Probamos ahora a hacer down multiples semaforos */
    printf("Probando la instrucción downMultiple.\n");
    
    /* Probamos ahora down multiple */
    printf("Probando down multiple");
    /* Probamos el borrado de los semáforos y terminamos la ejecución */
    printf("Probando el borrado de semáforos.\n");
    if(Borrar_Semaforo(semid)==ERROR){
        perror("Fallo en el borrado del semáforo");
        free(arg1.array);
        exit(EXIT_FAILURE);
    }
    
    printf("Test completado satisfactoriamente.\n\n");
    free(arg1.array);
    exit(EXIT_SUCCESS);
}
