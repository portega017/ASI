/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: pibol
 *
 * Created on 9 de febrero de 2021, 18:18
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>

int pidMonitor, pidHijo1, pidHijo2, statusH1, statusH2, contarHilos, contSig, fin;

/*
 * 
 */
void *funcionThread() {
    for (int i = 0; i < 4; i++) {
        printf("Soy el hilo %lu\n", pthread_self());
        sleep(5);

    }
    pthread_exit(0);
}

void SalirBucle() {
    fin = 1;
}

void funcion_hilos_ejer4() {
    pthread_t t;
    pthread_create(&t, NULL, &funcionThread, NULL);
    pthread_detach(t);
    contarHilos++;
    printf("Se han creado %d hilos\n", contarHilos);


}

void cuenta() {
    fin = 1;
}

void funcionContarSenales() {
    kill(pidMonitor, SIGUSR2);
    contSig++;
}

void atencionSigusr2() {
    //printf("Rutina de atención a la señal SIGUSR2, enviar SIGUSR2 de vuelta\n");
    kill(pidMonitor, SIGUSR2);
}

int main(int argc, char** argv) {
    pidMonitor = atoi(argv[1]); // obtener el pid del monitor y convertirlo a int
    //Ejer1
    kill(pidMonitor, SIGUSR1); // enviar señal SIGUSR1
    sleep(2); //Esperar al menos 1 segundo
    printf("Fin ejercicio 1\n");
    getchar();

    //Ejer2
    kill(pidMonitor, SIGUSR1); //Enviar SIGUSR1
    signal(SIGUSR2, atencionSigusr2); //Definimos la atención a la señal SIGUSR2
    pause(); //Cuando se desbloquee el proceso al recibir la señal SIGUSR2, se ejecutará la rutina de atención
    printf("Fin del ejercicio 2\n");
    getchar();

    //Ejer3
    kill(pidMonitor, SIGUSR1);
    sleep(1);
    pidHijo1 = fork();
    switch (pidHijo1) {
        case -1:
            printf("error al crear el hijo 1\n");
            getchar();
            break;
        case 0:
            //Ejer4
            printf("Soy el hijo 1\n");
            signal(SIGUSR2, funcion_hilos_ejer4);
            pause();
            printf("Fin del ejercicio 4\n");

            getchar();

            break;
        default:
            pidHijo2 = fork();
            switch (pidHijo2) {
                case -1:
                    printf("error al crear el hijo 2\n");
                    break;
                case 0:
                    printf("Soy el hijo 2\n");

                    contSig = 0;
                    fin = 0;
                    signal(SIGUSR1, cuenta);
                    while (!fin) {
                        pause();
                    }
                    signal(SIGUSR2, funcionContarSenales);
                    signal(SIGUSR1, SalirBucle);
                    fin = 0;
                    while (!fin) {
                        printf("Señal nº %d", contSig);
                        pause();
                    }
                    printf("Fin del ejercicio 5\n");
                    getchar();
                    break;
                default:
                    printf("Soy el padre\n");
                    kill(pidMonitor, SIGUSR1);
                    printf("Fin del ejercicio 3\n");
                    getchar();
                    kill(pidMonitor, SIGUSR1);



                    //Ejer6

                    /*kill(pidHijo1, SIGKILL);
                    printf("Hijo 1: muerto\n");
                    kill(pidHijo2, SIGKILL);
                    printf("Hijo 2: muerto\n");
                    kill(getpid(), SIGKILL);
                    printf("Padre: muerto\n");*/
                    wait(NULL);
                    wait(NULL);
                    kill(pidMonitor, SIGUSR1);
                    sleep(1);
                    printf("Fin ejercicio 6\n");

            }

    }

    return (EXIT_SUCCESS);
}

