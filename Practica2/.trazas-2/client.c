/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: pibol
 *  PRACTICA 2
 * Created on 17 de febrero de 2021, 8:51
 */
#include <stdio_ext.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/msg.h>

#define NOM_FIFOR "/tmp/fifo_monitor_1"
#define NOM_FIFOW "/tmp/fifo_monitor_2"
#define TAM 50

typedef struct Mensajes {
    long idMensaje;
    char cadena[TAM];
} Mensajes;
/*
 * 
 */
int pidMonitor;
int fifoR, fifoW;
char sec1[TAM], sec2[TAM], sec3[TAM], sec4[TAM];
int numbytes;
int IdColaMensaje1, MsgRet;
key_t Clave1 = 0x16094053L;
Mensajes m1, m2;
long pidCliente;
int numejer = 1;

void limpiarBuffer() {
    __fpurge(stdin);
    fflush(stdout);
}

void cambioEjer() {
    numejer++;
    printf("Pulsa una tecla para empezar el siguiente ejercicio (%i)\n", numejer);
    getchar();
    limpiarBuffer();
}

int main(int argc, char** argv) {
    pidMonitor = atoi(argv[1]);
    //Ejercicio 1
    fifoR = open(NOM_FIFOR, O_RDWR);
    if (fifoR == -1) {
        perror("Error al abrir fifo\n");
    } else {
        printf("Leyendo Fifo\n");
        numbytes = read(fifoR, sec1, TAM);
        printf("Bytes leídos: %d\n", numbytes);
        if (numbytes <= 0) {
            perror("Error de lectura\n");
        } else {
            printf("Secreto1: %s\n", sec1);
        }
    }
    printf("Fin del Ejercicio 1!!!!!\n");
    //Ejercicio 2
    cambioEjer();
    int MkFifoError = mkfifo(NOM_FIFOW, S_IRWXU);
    if (MkFifoError) {
        perror("Error creando fifo\n");
    } else {

    }
    fifoW = open(NOM_FIFOW, O_RDWR);
    if (fifoW == -1) {
        perror("Error al abrir el fifo 2\n");
    } else {
        numbytes = write(fifoW, sec1, strlen((char*) sec1) + 1);
        if (numbytes != strlen((char*) sec1) + 1) {
            perror("Error escribiendo secreto\n");
        } else {
            printf("%d bytes escritos correctamente\n", numbytes);
        }
        //Recibimos el secreto2 por FIFO1

        printf("Leyendo FIFO...\n");
        numbytes = read(fifoR, sec2, TAM);
        printf("NumBytes leidos = %d\n", numbytes);
        if (numbytes <= 0) {
            perror("Error de lectura\n");

        } else {
            printf("Leído el secreto 2: %s\n", sec2);
        }
        numbytes = write(fifoW, sec2, strlen((char*) sec2) + 1);
        if (numbytes != strlen((char*) sec2) + 1) {
            perror("Error escribiendo secreto 2\n");
        } else {
            printf("%d bytes escritos correctamente\n", numbytes);
        }
    }

    //Ejercicio 3
    cambioEjer();
    //Creamos cola de mensajes
    IdColaMensaje1 = msgget(Clave1, 0666);
    if (IdColaMensaje1 == -1) {
        perror("Error al abrir cola de mensaje\n");
        exit(-1);
    } else {
        printf("Se abre una cola de mensajes\n");
    }
    MsgRet = msgrcv(IdColaMensaje1, &m1, sizeof (Mensajes) - sizeof (long), 0, 0);
    if (MsgRet == -1) {
        perror("Error al leer el mensaje de la cola\n");
    } else {
        printf("Mensaje leído, secreto 3; %ld, secreto 4: %s\n", m1.idMensaje, m1.cadena);
    }


    //Ejercicio 4
    cambioEjer();
    //Crear cola con clave 1
    IdColaMensaje1 = msgget(Clave1, IPC_CREAT | 0666);
    if (IdColaMensaje1 == -1) {
        perror("Error al crear la cola de mensajes\n");
        exit(-1);
    } else {
        printf("vvvv---Se obtiene la cola de mensajes correctamente ----vvvv\n");
    }
    //Escribir secreto 3 y 4 en el Mensaje y enviarlo por la cola que acabamos de crear
    m2.idMensaje = 1L;
    sprintf(m2.cadena, "<%ld>%s", m1.idMensaje, m1.cadena);
    MsgRet = msgsnd(IdColaMensaje1, &m2, sizeof (Mensajes) - sizeof (long), 0);
    if (MsgRet == -1) {
        perror("Error al enviar el mensaje.\n");
    } else {
        printf("Mensaje enviado correctamente\n");
    }
    
    //Ejercicio 5
    cambioEjer();
    MsgRet = msgrcv(IdColaMensaje1, &m1, sizeof (Mensajes) - sizeof (long), getpid(), 0);
    if (MsgRet == -1) {
        perror("Error al leer mensaje en la cola\n");
    } else {
        printf("Mensaje leido, secreto 6: %s, PID del cliente: %ld\n", m1.cadena, m1.idMensaje);

    }
    //EJERCICIO6    

    cambioEjer();

    m1.idMensaje = pidMonitor;
   // sprintf(m1.cadena, "%s", m1.cadena);
    MsgRet = msgsnd(IdColaMensaje1, &m1, sizeof (Mensajes) - sizeof (long), 0);
    if (MsgRet == -1) {
        perror("Error al enviar el mensaje");
    } else {
        printf("Mensaje enviado\n");
    }

    //Ejercicio7
    cambioEjer();
    //Le enviamos un mensaje cualquiera con tipo pid del monitor
    MsgRet = msgsnd(IdColaMensaje1, &m1, sizeof (Mensajes) - sizeof (long), 0);
    if (MsgRet == -1) {
        perror("Error al enviar el mensaje");
    } else {
        printf("Mensaje enviado\n");
    }

    //Eliminamos los recursos de las colas FIFO 1 y 2
    close(fifoR);
    MkFifoError = unlink(NOM_FIFOR);
    if (MkFifoError) {
        perror("Error borrando FIFO1\n");
    } else {
        printf("FIFO1 borrada correctamente\n");
    }
    //Eliminar FIFO2
    close(fifoW);
    MkFifoError = unlink(NOM_FIFOW);
    if (MkFifoError) {
        perror("Error borrando FIFO2\n");
    } else {
        printf("FIFO2 borrada correctamente\n");
    }
    //Eliminamos la cola de mensajes1
    close(IdColaMensaje1);
    MsgRet = msgctl(IdColaMensaje1, IPC_RMID, (struct msqid_ds*) NULL);
    if (MsgRet == -1) {
        perror("Error al destruir la cola de mensajes\n");
    } else {
        printf("La cola de mensajes se destruye correctamente\n");
    }
    return (EXIT_SUCCESS);
}

