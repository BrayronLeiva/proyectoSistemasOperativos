#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CARS 5
#define BRIDGE_CAPACITY 1 // Solo un automovil puede cruzar el puente a la vez (Malcolm)

pthread_mutex_t bridge_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bridge_capacity = PTHREAD_COND_INITIALIZER;
int num_cars_on_bridge = 0; // Variable que nos permite saber el numero carros en el puente (Malcolm)

#define N 5 // Número de automóviles
pthread_mutex_t mutex; //mutex para manejar acceso de puente
//char flag;
int carrosEnPuente = 0;
int carrosOaE = 0;
int carrosEaO = 0;

// Estructura para representar el puente
struct Puente{
    bool estado;
    char lado_actual[10];
    // Definir otras variables y estructuras de datos necesarias
};
Puente puente; //aca declaro mi puente

// Estructura para representar un automóvil
struct Automovil {
    int id;
    char sentido; // o 'hacia oeste', e hacia 'este'
    int velocidad;
    bool estado; //espera, cruzando, finalizado
    // Definir otras variables necesarias
};

void *cross_bridge(void *);

// Función para que un automóvil cruce el puente
void cruzar_puente(Automovil *automovil) {
    printf("CRUZANDO PUENTE DESDE CARRO %d \n", automovil->id);
    printf("Sentido: %d \n ", automovil->sentido);
    automovil->estado=true; //ya cruzo

}

// Función que simula el comportamiento de un automóvil
void *comportamiento_automovil(void *arg) {
    Automovil *automovil = (Automovil *)arg; //agarra de args su estructura de datos que es como su entidad
    pthread_t id_hilo = pthread_self(); // Obtener el identificador del hilo
    bool cont = true;
    while (cont) {

        Puente* miPuente = &puente;
        if(carrosEnPuente>0){miPuente->estado=true;}
        if(carrosEnPuente==0){miPuente->estado=false;}//habilito el puente

        //|| automovil->estado=='o'&& carrosOaE>1 && carrosEaO==0 || automovil->estado =='e'&& carrosEaO>1 && carrosOaE==0
        if(!miPuente->estado || automovil->sentido=='o'&& carrosOaE>1 && carrosEaO==0 || automovil->estado =='e'&& carrosEaO>1 && carrosOaE==0){ //hay que implementar para que si los carros NO van en el mismo sentido se bloquee
            //ademas lo del carro puede ir en otro sitio ya que como ya cruzo el hilo podria ya ser eliminado o tal vez dependiente si el carro puede regresar???
            printf("El valor del booleano es: %s, ID del carro: %d\n", miPuente->estado ? "ocupado" : "libre",automovil->id, "\n");
            printf("carros en puente: %d, ID del carro: %d\n\n", carrosEnPuente , automovil->id );
            printf("carros en puente de este a oeste: %d, ID del carro: %d\n\n", carrosEaO , automovil->id );
            printf("carros en puente de oeste a este: %d , ID del carro: %d\n\n", carrosOaE , automovil->id );
            if(!automovil->estado) {
                pthread_mutex_lock(&mutex);
                carrosEnPuente++;
                if(automovil->sentido=='o'){carrosOaE++;}
                if(automovil->sentido=='e'){carrosEaO++;}
                pthread_mutex_unlock(&mutex);
                sleep(rand() % 5); // Simula el tiempo entre llegadas de automóviles
                cruzar_puente(automovil);


                printf("2: carros en puente: %d, ID del carro: %d\n \n", carrosEnPuente, automovil->id );
                printf("2: carros en puente de este a oeste: %d , ID del carro: %d\n\n", carrosEaO, automovil->id );
                printf("2: carros en puente de oeste a este: %d, ID del carro: %d\n\n", carrosOaE, automovil->id );
                printf("2: El valor del booleano es: %s, ID del carro: %d\n", miPuente->estado ? "ocupado" : "libre",  automovil->id, "\n");

                pthread_mutex_lock(&mutex);
                carrosEnPuente--;
                if(automovil->sentido=='o'){carrosOaE--;}
                if(automovil->sentido=='e'){carrosEaO--;}
                pthread_mutex_unlock(&mutex);
                printf("TERMINANDO CARRO HILO, desde carro %d\n", automovil->id);
                cont = false;
            }//else{ //VERIFICAR POR QUE ESTO NO SIRVIO
                //printf("CAYENDO A ELSE");
                //cont = false;
            //}//si ya cruzo bye bye

        }else{
            //printf("PUENTE OCUPADO ME REGRESO A ESPERAR \n\n");
        }



        //if(carrosEnPuente==0){
            //if(flag=='o'){flag='e';}
            //if(flag=='e'){flag='o';}
        //}


    }
    printf("\t\t\t\t\t\tSaliendo de metodo\n\n");
    return NULL;
}

int main() {
    /*pthread_t threads[N];
    puente.estado = false; //comienza vacio

    pthread_mutex_init(&mutex, NULL);
    // Inicializar otras variables del puente

    // Crear hilos para los automóviles
    for (int i = 0; i < N; i++) {
        static int ejecutado = 0; // Variable estática como indicador

        Automovil *automovil = (Automovil *)malloc(sizeof(Automovil));
        automovil->id = i;
        automovil->estado = false;
        if(rand() % 2 == 0){
            automovil->sentido = 'e';
        }else{automovil->sentido = 'o';}

        //if (!ejecutado) {
            //para decir el primero que llega al puente
            //flag = automovil->sentido; // Actualiza el indicador para que no se vuelva a ejecutar
        //}

        pthread_create(&threads[i], NULL, comportamiento_automovil, (void *)automovil);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < N; i++) {
        printf("HACIENDO JOIN A EL HILO %d \n\n", i);
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);*/

    pthread_t threads[MAX_CARS];
    struct Automovil cars[MAX_CARS] = {
            {1, 'o', 100000, false},  // Auto 1 esta cruzando en 100000 microsegundos
            {2, 'o',200000, false},  // Auto 2 esta cruzando en 200000 microsegundos
            {3, 'o',150000, false},  // Auto 3 esta cruzando en 150000 microsegundos
            {4, 'o',300000, false},  // Auto 4 esta cruzando en 300000 microsegundos
            {5, 'o',250000, false}   // Auto 5 esta cruzando en 250000 microsegundos
    };

    // Create threads for each car
    for (int i = 0; i < MAX_CARS; i++) {
        pthread_create(&threads[i], NULL, cross_bridge, (void *)&cars[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < MAX_CARS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

void *cross_bridge(void *car_ptr) {
    struct Automovil *car = (struct Automovil *)car_ptr;

    // Los autos esperaran hasta que el puente este libre (Malcolm)
    pthread_mutex_lock(&bridge_mutex);
    while (num_cars_on_bridge >= BRIDGE_CAPACITY) {
        pthread_cond_wait(&bridge_capacity, &bridge_mutex);
    }
    num_cars_on_bridge++;

    // El auto cruza el puente (Malcolm)
    printf("Auto %d esta cruzando el puente...\n", car->id);
    pthread_mutex_unlock(&bridge_mutex);
    usleep(car->velocidad); // Simula la velocidad de cada auto
    pthread_mutex_lock(&bridge_mutex);

    // El puente queda libre para el siguiente auto
    printf("Auto %d ha cruzado el puente.\n", car->id);
    num_cars_on_bridge--;
    pthread_cond_signal(&bridge_capacity);
    pthread_mutex_unlock(&bridge_mutex);

    pthread_exit(NULL);
}