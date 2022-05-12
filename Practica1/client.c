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
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
int pidMonitor, hijo1, hijo2, contarHilos = 0, fin = 0, contSig = 0;

void enviaSig2() {
    kill(pidMonitor, SIGUSR2);
}

void salirWhile() {
    fin = 1;
}

void *funcionHilos() {


    for (int i = 0; i < 4; i++) {
        printf("Soy el hilo %lu\n", pthread_self());
        sleep(5);
    }
    pthread_exit(0);

}

void ejercicio4() {
    pthread_t t;
    pthread_create(&t, NULL, &funcionHilos, NULL);
    pthread_detach(t);
    contarHilos++;
    printf("Se han creado %d hilos\n", contarHilos);
}

void contarSeniales() {
    kill(pidMonitor, SIGUSR2);
    contSig++;

}

/*
 * 
 */
int main(int argc, char** argv) {
    pidMonitor = atoi(argv[1]);
    //Ejercicio 1
    kill(pidMonitor, SIGUSR1);
    sleep(2);
    printf("Fin del Ejercicio 1\n");
    getchar();

    //Ejercicio 2
    kill(pidMonitor, SIGUSR1);
    signal(SIGUSR2, enviaSig2);
    pause();
    printf("Fin del Ejercicio 2\n");
    getchar();

    //Ejercicio 3
    kill(pidMonitor, SIGUSR1);
    sleep(1);

    if (hijo1 == fork()) {
        printf("Soy el hijo 1 con Pid: %i hijo de %i\n", getpid(), getppid());
        //Ejercicio 4
        signal(SIGUSR2, ejercicio4);
        pause();
        kill(pidMonitor, SIGUSR1);
        printf("Fin del Ejercicio 3\n");
        getchar();

    } else if (hijo2 == fork()) {
        printf("Soy el hijo 2 con Pid: %i hijo de %i\n", getpid(), getppid());

        //Ejercicio 5
        signal(SIGUSR1, salirWhile);
        fin = 0;
        while (!fin) {
            pause();
        }
        signal(SIGUSR2, contarSeniales);
        fin = 0;
        while (!fin) {
            pause;
        }
        printf("Número de señales:%d\n", contSig);
    } else {
        printf("Soy el padre con pid:%i\n", getpid());
        kill(pidMonitor, SIGUSR1);
        printf("Fin del Ejercicio 3\n");
        getchar();
        wait(NULL);
        kill(pidMonitor, SIGUSR1); //inicio 5
        getchar();
        wait(NULL);
        //Ejercicio 6
        kill(pidMonitor, SIGUSR1);
        sleep(1);
        printf("Fin del Ejercicio 6\n");
    }


    return (EXIT_SUCCESS);
}

