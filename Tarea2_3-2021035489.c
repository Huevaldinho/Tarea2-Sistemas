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
int array[fiboAcalcular];//Almacena los valores de Fibonacci
pthread_mutex_t arrayMutex[fiboAcalcular];//Sincroniadores

//Calcula fibonacci del num.
//Tiene mucha repeticion de calculo por usar recursion.
void * fibonacci(void * num){
    //Saca el numero que envia el padre al crear el hilo
    int n = *((int *)num);
    int siguienteFibo = n+1;

    if (n>fiboAcalcular)//Termina porque ya calculó el Fibo(n)
        pthread_exit(NULL);
    if (n <= 1) {//Fibo(0)=0 & Fibo(1)=1
        array[n] = n;
    } else {//Resto de Fibo
        int a, b;
        //Se bloquean las posiciones para sincronizar los procesos 
        //y asi no se modifique al mismo tiempo
        //Fibo n = Fibo n-1 + Fibo n-2 
        pthread_mutex_lock(&arrayMutex[n - 1]);//Bloquea pos de Fibo n-1
        a = array[n - 1];//Toma fibo de n-1
        pthread_mutex_unlock(&arrayMutex[n - 1]);//Desbloquea pos de Fibo n-1
        pthread_mutex_lock(&arrayMutex[n - 2]);//Bloquea pos de Fibo n-2
        b = array[n - 2];//Toma fibo de n-2
        pthread_mutex_unlock(&arrayMutex[n - 2]);//Desbloquea pos de Fibo n-2
        array[n] = a + b;//Calcula Fibo de n
    }
    //* Descomentar para ver el calculo de Fibo hecho por cada hilo.
    //printf("Hilo: %lu calculó Fibo(%d) = %d\n", (unsigned long)pthread_self(),n,array[n]);
    fibonacci(&(siguienteFibo));
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    //Inicializa el array de mutex 
    for (int i = 0; i <= fiboAcalcular; i++) {
        pthread_mutex_init(&arrayMutex[i], NULL);
    }
    //Crea los 5 hilos, manda a calcular Fibo de 0 a 4, los demas 
    //se mandan a calcular dentro de la ejcucion de la funcion fibonacci.
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, fibonacci, &thread_args[i]);
    }
    //Espera a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    //Elimina los mutexes inicializados
    for (int i = 0; i <= fiboAcalcular; i++) {
        pthread_mutex_destroy(&arrayMutex[i]);
    }
    printf("Fibonacci(%d) = %d\n", fiboAcalcular, array[fiboAcalcular]);
    return 0;
}