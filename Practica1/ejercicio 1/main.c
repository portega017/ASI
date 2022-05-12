/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ejercicio1.c
 * Author: pibol
 *
 * Created on 22 de febrero de 2021, 17:43
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

int hijo1, hijo2, fin = 0, respuesta;

/*
 * 
 */

void salirBucle() {
    fin = 1;
}

void limpiarBuffer() {
    __fpurge(stdin);
    fflush(stdout);
}

void menu() {
    printf("1. Envía señal al hijo 1\n");
    printf("2. Envía señal al padre\n");
    printf("3. Finalizar la comunicación\n");
    printf("4. Construir mensaje\n");
    printf("Selecciona una opción: ");
    fflush(stdin);
    scanf("%d", &respuesta);
    while ((respuesta > 4 || respuesta <= 0)) {
        printf("Selecciona una opción válida: ");
        limpiarBuffer();
        scanf("%d", &respuesta);
    }

}

void crearFich() {
    char mensaje[500];
    FILE *f;
    printf("Escribe el mensaje:  ");
    limpiarBuffer();

    fgets(mensaje, 500, stdin);
    if ((f = fopen("mensaje", "wb")) != (FILE*) NULL) {
        fwrite(mensaje, strlen(mensaje), 1, f);
        fclose(f);
    }
}

void *traduccion() {
    FILE *f;
    char mensaje[500];
    if ((f = fopen("mensaje", "rb")) != (FILE*) NULL) {
        fread(mensaje, 250, 1, f);
        fclose(f);
        for (int i = 0; i < strlen(mensaje); i++) {
            mensaje[i] = toupper(mensaje[i]);
        }
        printf("Mensaje pasado a mayúsculas: %s\n", mensaje);
        limpiarBuffer();
    }
    pthread_exit(0);

}

void crearHilo() {
    pthread_t t;
    pthread_create(&t, NULL, &traduccion, NULL);
    pthread_detach(t);
}

void mensajeRecibido() {
    printf("Soy %d y he recibido una señal\n", getpid());
    limpiarBuffer();
}

int main(int argc, char** argv) {

    if (hijo1 == fork()) {
        printf("Soy el hijo 1 con id:%d mi padre es:%d\n", getpid(), getppid());
        signal(SIGUSR1, mensajeRecibido);

        signal(SIGUSR2, crearHilo);
        while (fin != 1) {
            pause();

        }

    } else if (hijo2 == fork()) {
        printf("Soy el hijo 2 con id:%d mi padre es:%d\n", getpid(), getppid());
        fin = 0;
        do {
            menu();
            switch (respuesta) {
                case 1://señal a hijo 1
                    kill(hijo1, SIGUSR1);

                    fin = 1;
                    limpiarBuffer();
                    break;
                case 2: //señal a padre
                    kill(getppid(), SIGUSR1);

                    fin = 1;
                    limpiarBuffer();
                    break;
                case 3:
                    printf("\nComunicación finalizada\n");
                    limpiarBuffer();
                    kill(hijo1, SIGKILL);
                    break;
                case 4:
                    crearFich();
                    kill(hijo1, SIGUSR2);
                    limpiarBuffer();
                    break;
            }
        } while (respuesta != 3);
        kill(getppid(), SIGUSR2);
        kill(hijo1, SIGKILL);
        raise(SIGKILL);
    } else {
        printf("Soy el padre con id:%d\n", getpid());
        signal(SIGUSR1, mensajeRecibido);
        // signal(SIGUSR2, salirBucle);

        while (fin != 1) {
            pause();

        }
        wait(NULL);
        wait(NULL);

    }

    return (EXIT_SUCCESS);
}

