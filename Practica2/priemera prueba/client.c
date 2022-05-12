/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   client.c
 * Author: pablo
 *
 * Created on February 22, 2021, 8:44 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

/*
 * 
 */

int hijo1,hijo2,f[2],finbucle,i;
char mensaje[200], resp;

void salirDelbucle(){
    finbucle=1;
}





int main(int argc, char** argv) {
    
    pipe(f);
    hijo1=fork();
    switch(hijo1){
        
        case -1:printf("Error\n");break;
        case  0:close(f[0]); // cerramos para poder escribir
            do{
            
            printf("Introduce un mensaje: \n");
             fflush(stdout);
            __fpurge(stdin);
            gets(mensaje);
            write(f[1],mensaje,strlen(mensaje));
            
            
     
            
            printf("¿Quieres traducir mas mensajes?\n");
            fflush(stdout);
            __fpurge(stdin);
            scanf("%c",&resp);
            
            
            
        }while(resp=='s');
        kill(hijo2,SIGUSR1);
                
        
        
        
        break;
                
        default:hijo2=fork();
                switch(hijo2){
                     case -1:printf("Error\n");break;
                     case  0:
                            
                         signal(SIGUSR1,salirDelbucle);
                         
                            close(f[1]); // cerramos para poder leer
                     
                            
                            finbucle=0;
                            
                            while(!finbucle){
                     
                     
                                read(f[0],mensaje,200);
                                for(i=0;i<strlen(mensaje);i++){
                                    mensaje[i]=toupper(mensaje[i]);
                                    
                                    
                                }
                                printf("El mensaje ha sido traducido por el hijo 2 y este es el resultado: %s\n",mensaje);
                                fflush(stdout);
                                sleep(1);

                            }
                         
                               break;
        default:close(f[0]);//cerramos asi los pipes de lectura y escritura 
                close(f[1]); 
            wait(NULL);              
            wait(NULL);
            
        }
                
                
        
        
    }
    
    
    

    return (EXIT_SUCCESS);
}

