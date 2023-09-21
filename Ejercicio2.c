#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 10

//Tarea 2
/* EJERCICIO 2
    Cree un proceso que cree a su vez 10 hilos hijos los cuáles deben generar
    un número aleatorio (menor a 255) que imprimirán y luego retornarán al hilo 
    padre como su estado de terminación. El hilo padre también debe imprimir
    los números aleatorios que le retornaron sus hilos hijos al finalizar.
*/
//Funcion ejecutada por los hilos.
void * generarNumeroRandom(void *arg) {
    int numeroAleatorio = rand() % 254+ 1; //Genera un número aleatorio entre 1 y 254
    pthread_exit((void *) numeroAleatorio);//Devuelve al padre el random generado
}

int main() {
    pthread_t hilos[NUM_THREADS];
    srand(time(NULL)); //Semilla para randoms
    // Crear hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        //Al crearlos los manda a ejecutar la funcion generarNumeroRandom
        if (pthread_create(&hilos[i], NULL, generarNumeroRandom, NULL) != 0) {
            perror("Error al crear el hilo");
            return 1;
        }
    }
    // Espera a que los hilos terminen para imprimir el random que generó
    int random;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(hilos[i], (void *)&random) != 0){
            perror("Error al esperar al hilo");
            return 1;
        } else 
            printf("Hilo %d generó el número aleatorio: %d\n", i, random);
        
    }
    return 0;
}
