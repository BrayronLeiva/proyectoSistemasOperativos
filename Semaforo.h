#ifndef PROYECTOSO_SEMAFORO_H
#define PROYECTOSO_SEMAFORO_H

#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define SEM_ID int

void semWait(int semid){
    struct sembuf s;
    s.sem_num = 0; // Primer y unico elemento del semaforo.
    s.sem_op = -1;
    s.sem_flg = SEM_UNDO;

    semop(semid, &s, 1);
    return;
}
void semSignal(int semid){
    struct sembuf s;

    s.sem_num = 0; // Primer y unico elemento del semaforo.
    s.sem_op = 1;
    s.sem_flg = SEM_UNDO;

    semop(semid, &s, 1);
    return;
}
int createSem(int key, int value) {
    int semid;
    semid = semget(key, 1, 0666 | IPC_CREAT); // Solicita un arreglo de Semaforos al SO
    semctl(semid,0, SETVAL, value); // Inicializa el semaforo en 1
    return semid;
}
void eraseSem(int semid){
    semctl(semid, 0, IPC_RMID, 0); // Elimina el arreglo de semaforos.
    return;
}

#endif //PROYECTOSO_SEMAFORO_H
