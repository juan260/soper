#include "monitor.h"

/**
* Esta funcion ejecuta un fork, y el hijo ejecuta el codigo del monitor.
* El padre devuelve el pid del hijo, o -1 en caso de error.
*/

int monitor(int nCaballos, int nApostadores, int * sid, int mutex,
    int* matrizApuestasId){

  int * tiempo;
  int * posicionCaballo;
  double * matrizApuestas[10];
  int tiempoAux, i, j, ret;
  double sumaTotal, sumaParcial;

  if((ret=fork())<0){
    return -1;
  } else if(ret==0){
    tiempo = (int *)shmat(sid[0], NULL, 0);
    posicionCaballo = (int *)shmat(sid[1], NULL, 0);
    for(i=0;i<nApostadores;i++){
      matrizApuestas[i] = (double *)shmat(matrizApuestasId[i], NULL, 0);
    }

    while(1){
      tiempoAux = *tiempo;
      printf("Estado de la carrera:\n");
      if(tiempoAux>1){
        printf("\tFaltan %d segundos para el comienzo de la carrera.\n", tiempoAux);
      } else if(tiempoAux==1){
        printf("\tFalta 1 segundo para el comienzo de la carrera.\n");
      } else if(tiempoAux<0){
        break;
      } else {
        printf("\tCarrera comenzada.\n");
      }
      printf("\tEstado de las apuestas: (valores totales apostados por cada apostador a\n\t"
          "  cada caballo, en, orden creciente segun el numero del caballo)\n");
      sumaTotal = 0;
      Down_Semaforo(mutex, 2, SEM_UNDO);
      for(i=0;i<nApostadores;i++){
        printf("\t\t- El Apostador-%d ha apostado: ", i);
        for(j=0;j<nCaballos;j++){
          printf("%f ",matrizApuestas[i][j]);
          sumaTotal+=matrizApuestas[i][j];
        }
        printf("\n");
      }
      printf("\tCotizaciones de los caballos:\n");
      for(i=0;i<nApostadores;i++){
        printf("\t\t- Caballo %d: ", i);
        sumaParcial=0;
        for(j=0;j<nCaballos;j++){
          sumaParcial+=matrizApuestas[i][j];
        }
        if(sumaParcial==0){
          printf("no hay apuestas a este caballo.\n");
        } else {
          printf("%f\n",(double)sumaTotal/(double)sumaParcial);
        }
      }
      Up_Semaforo(mutex,2,SEM_UNDO);
      printf("\n");
      sleep(1);
    }
    printf("Carrera finalizada.\n");
    while(1){
      if(*tiempo<=-15){
        break;
      }
      printf("Segundos restantes para mostrar resultados: %d", *tiempo);
      sleep(1);
    }

    printf("Mostrando resultados de la carrera: \n");
    Down_Semaforo(mutex, 1, SEM_UNDO);
    



    Up_Semaforo(mutex,1,SEM_UNDO);
    printf("\t");

    shmdt(tiempo);
    shmdt(posicionCaballo);
    shmdt(matrizApuestas);
  } else {
    return ret;
  }
}
