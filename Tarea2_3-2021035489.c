#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 5
#define fiboAcalcular 30


//                              TAREA 2
/*  EJERCICIO 3
    Escriba un programa que calcule la función Fibonacci(30) utilizando 
    un arreglo de 30 posiciones. Utilice 5 hilos para ir calculando cada 
    una de las posiciones del arreglo, y un arreglo de mutexes 
    para sincronizar los diferentes hilos.
*/
int array[fiboAcalcular];
pthread_mutex_t arrayMutex[fiboAcalcular];

void * fibonacci(void *arg) {
    int n = *((int *)arg);

    if (n <= 1) {
        array[n] = n;
    } else {
        int a, b;
        
        pthread_mutex_lock(&arrayMutex[n - 1]);
        a = array[n - 1];
        pthread_mutex_unlock(&arrayMutex[n - 1]);

        pthread_mutex_lock(&arrayMutex[n - 2]);
        b = array[n - 2];
        pthread_mutex_unlock(&arrayMutex[n - 2]);

        array[n] = a + b;
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    for (int i = 0; i < 30; i++) {
        pthread_mutex_init(&arrayMutex[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i + 2;  // Iniciar los hilos desde la posición 2
        pthread_create(&threads[i], NULL, fibonacci, &thread_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Fibonacci(%d) = %d\n", fiboAcalcular, array[fiboAcalcular]);

    for (int i = 0; i < 30; i++) {
        pthread_mutex_destroy(&arrayMutex[i]);
    }

    return 0;
}