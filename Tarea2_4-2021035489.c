#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h> 


#define NUM_THREADS 10
#define CANTIDAD_NECESARIA_BARRERA 5
pthread_barrier_t barrera; // Declaración de la barrera

//                              TAREA 2
/*  EJERCICIO 4
    Una barrera es una forma de mantener los miembros de un grupo juntos.
    Una barrera es generalmente empleada para asegurar que todos los hilos 
    cooperantes en algún algoritmo paralelo lleguen a un punto específico 
    antes que puedan continuar. 
    Escriba un programa que cree 10 hilos que esperan un tiempo aleatorio,
    y luego se reunirán en una barrera hasta que completen 5 de ellos. 
    Cuando los hilos pasen la barrera imprimirán su identificador de hilo.
*/
void *ejeucionHilo(void * threadid){
    sleep( rand() % 5); // Espera un tiempo aleatorio
    // Esperar en la barrera
    printf("Hilo %lu está esperando que se abra la barrera.\n",pthread_self());
    pthread_barrier_wait(&barrera);//hasta que lleguen los 5 necesarios se puede continuar
    printf("El hilo %lu ya pasó la barrera.\n", pthread_self());
    pthread_exit(NULL);
}
int main() {
    pthread_t threads[NUM_THREADS];
    pthread_barrier_init(&barrera, NULL, CANTIDAD_NECESARIA_BARRERA);
    srand(time(NULL)); 

    // Crear hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, ejeucionHilo, NULL) != 0) {//Crea hilo, el hilo se va a ejecutar la funcion
            perror("Error al crear el hilo");
            return 1;
        }
    }
    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error al esperar al hilo");
            return 1;
        }
    }
    pthread_barrier_destroy(&barrera);//Destruye la barrera
    return 0;
}