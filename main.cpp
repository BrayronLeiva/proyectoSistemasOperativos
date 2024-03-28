#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5 // Número de automóviles
pthread_mutex_t mutex; //mutex para manejar acceso

// Estructura para representar el puente
struct Puente{
    bool estado;
    char lado_actual[10];
    // Definir otras variables y estructuras de datos necesarias
};

// Estructura para representar un automóvil
struct Automovil {
    int id;
    char sentido; // o 'hacia oeste', e hacia 'este'
    Puente *puente;
    bool estado; //espera, cruzando, finalizado
    // Definir otras variables necesarias
};

// Función para que un automóvil cruce el puente
void cruzar_puente(Automovil *automovil) {
    printf("CRUZANDO PUENTE DESDE CARRO %d \n", automovil->id);
    automovil->estado=true; //ya cruzo

}

// Función que simula el comportamiento de un automóvil
void *comportamiento_automovil(void *arg) {
    Automovil *automovil = (Automovil *)arg; //agarra de args su estructura de datos que es como su entidad
    while (1) {

        Puente* miPuente = automovil->puente;
        if(!miPuente->estado && !automovil->estado){ //hay que implementar para que si los carros NO van en el mismo sentido se bloquee
            //ademas lo del carro puede ir en otro sitio ya que como ya cruzo el hilo podria ya ser eliminado o tal vez dependiente si el carro puede regresar???
            pthread_mutex_lock(&mutex);
                sleep(rand() % 5); // Simula el tiempo entre llegadas de automóviles
                cruzar_puente(automovil);
            pthread_mutex_unlock(&mutex);
        }

    }
    return NULL;
}

int main() {
    pthread_t threads[N];
    Puente puente;
    puente.estado = false; //comienza vacio
    pthread_mutex_init(&mutex, NULL);
    // Inicializar otras variables del puente

    // Crear hilos para los automóviles
    for (int i = 0; i < N; i++) {
        Automovil *automovil = (Automovil *)malloc(sizeof(Automovil));
        automovil->id = i;
        automovil->puente = &puente;
        automovil->estado = false;
        //poner sentido al carro !---------------
        pthread_create(&threads[i], NULL, comportamiento_automovil, (void *)automovil);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
