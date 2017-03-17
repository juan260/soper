/**
* @brief Fuente del ejercicio 4
* Este programa pide al usuario dos matrices y dos factores para multiplicarlas. Multiplica
* cada una por un factor en dos hilos distintos que además mantienen una comunicación para saber por donde
* va el otro
* @file ejercicio4.c
* @author Juan Riera Gomez (juan.riera@estudiante.uam.es) y Carlos Ignacio Isasa Martín (carlos.isasa@estudiante.uam.es)
* @version 1.0
* @date 14-03-2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

typedef struct matrix
{
    int dim;
    int **matrix1;
    int **matrix2;
    int fila1;
    int fila2;
    int mul1;
    int mul2;
}Matrix;

Matrix *matrix_create(int dim, int mul1, int mul2, char *matrix1, char *matrix2);

void matrix_free(Matrix *m);

void *first_matrix (void *matrix);

void *second_matrix (void *matrix);

int main ()
{
    int dim;
    int mul1;
    int mul2;
    char m1[50];
    char m2[50];
    Matrix *m;
    pthread_t T1; 
    pthread_t T2; 

    
    printf ("Introduzca dimension de la matriz cuadrada:\n");
    scanf("%d", &dim);
    printf ("Introduzca multiplicador 1:\n");
    scanf("%d", &mul1);
    printf ("Introduzca multiplicador 2:\n");
    scanf("%d", &mul2);
    getchar();
    printf("Introduzca matriz 1:\n");
    fgets(m1, 50, stdin);
    printf("Introduzca matriz 2:\n");
    fgets(m2, 50, stdin);
    m = matrix_create(dim, mul1, mul2, m1, m2);
    if(m==NULL){
        printf("Error de memoria\n");
        return 0;
    }

    pthread_create(&T1, NULL, first_matrix, (void*) m);
    pthread_create(&T2, NULL, second_matrix, (void*) m);

    pthread_join(T1, NULL);
    pthread_join(T2, NULL);
    
    matrix_free(m);



    return 1;
}

Matrix *matrix_create(int dim, int mul1, int mul2, char *matrix1, char *matrix2){
    int i, j;
    Matrix *m = (Matrix*) malloc(sizeof(Matrix));
    (m->dim)=dim;
    m->mul1=mul1;
    m->mul2=mul2;
    m->fila1=0;
    m->fila2=0;
    m->matrix1 = (int**) malloc (sizeof(int*)*dim);
    if (m->matrix1==NULL){
        return NULL;
    }
    for (i=0; i<dim; i++){ 
        m->matrix1[i]=(int*)malloc(sizeof(int)*dim);
        if (m->matrix1[i]==NULL){
            while(i!=0){
                free(m->matrix1[i]);
                i--;
            }
            free(m->matrix1);
            return NULL;
        }
        for (j=0; j<dim; j++){
            if (i==0 && j == 0){
                m->matrix1[i][j] = atoi(strtok(matrix1, " "));
            }
            else{
                m->matrix1[i][j] = atoi(strtok(NULL, " "));
            }
        }
    }
    m->matrix2 = (int**) malloc (sizeof(int*)*dim);
    if (m->matrix2==NULL){
        for (i=0; i<dim; i++){
            free(m->matrix1[i]);
        }
        free(m->matrix1);
        return NULL;
    }
    for (i=0; i<dim; i++){
        m->matrix2[i]=(int*)malloc(sizeof(int)*dim);
        if (m->matrix2[i]==NULL){
            while(i!=0){
                free(m->matrix2[i]);
                i--;
            }
            free(m->matrix2);
            for (i=0; i<dim; i++){
                free(m->matrix1[i]);
            }
            free(m->matrix1);
            return NULL;
        }
        for (j=0; j<dim; j++){
            if (i==0 && j == 0){
                m->matrix2[i][j] = atoi(strtok(matrix2, " "));
            }
            else{
                m->matrix2[i][j] = atoi(strtok(NULL, " "));
            }
        }
    }
    return m;
}


void matrix_free(Matrix *m){
    int i;
    for (i=0; i<(m->dim); i++){
        free(m->matrix1[i]);
        free(m->matrix2[i]);
    }
    free(m->matrix1);
    free(m->matrix2);
    free(m);
}

void *first_matrix (void *matrix){
    Matrix *m = (Matrix*) matrix;
    int i, j;
    for (i = 0; i<m->dim; i++){
        printf("Hilo 1 multiplicando fila %d resultado: ", i);
        for (j=0; j<m->dim; j++){
            printf("%d ", m->matrix1[i][j]*m->mul1);
        }
        printf("– el Hilo 2 va por la fila %d\n", m->fila2);
        sleep(1);
        m->fila1++;
    }
    return NULL;
}

void *second_matrix (void *matrix){
    Matrix *m = (Matrix*) matrix;
    int i, j;
    for (i = 0; i<m->dim; i++){
        printf("Hilo 2 multiplicando fila %d resultado: ", i);
        for (j=0; j<m->dim; j++){
            printf("%d ", m->matrix2[i][j]*m->mul2);
        }
        printf("– el Hilo 1 va por la fila %d\n", m->fila1);
        sleep(1);
        m->fila2++;
    }
    return NULL;
}