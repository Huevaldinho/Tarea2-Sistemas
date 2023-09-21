#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 

#define NUM_THREADS 10


// TAREA 2
/* Ejercicio 1
    Cree un hilo padre que cree a su vez 10 hilos hijos a los cuáles les pasará 
    por parámetros un identificador y un número aleatorio de segundos
    que cada hilo hijo debe dormir (sleep). 
    Al despertar cada hilo imprimirá su identificador y la cantidad de segundos que esperó.
*/
//Estructura para pasarle los datos por parametro a los hilos.
struct ThreadArgs {
    int identificador;
    int tiempoAdormir;
};
//Funcion ejecutada por los hilos
void *ejeucionHilo(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    int id = args->identificador;
    int tiempoAdormir = args->tiempoAdormir;
    printf("Hilo hijo %d: Va a dormir %d segundos.\n", id, tiempoAdormir);
    sleep(tiempoAdormir);
    printf("Hilo hijo %d: Despierta, despues de %d segundos.\n", id, tiempoAdormir);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    struct ThreadArgs args[NUM_THREADS];

    srand(time(NULL)); //Para el random de dormir

    // Crear hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].identificador = i + 1; 
        args[i].tiempoAdormir = rand() % 5 + 1; //Entre 1 y 5.
        if (pthread_create(&threads[i], NULL, ejeucionHilo, &args[i]) != 0) {//Crea hilo, el hilo se va a ejecutar la funcion
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

    return 0;
}
