/* Trabajo práctico N3 Ejercicio 3 (Segunda entrega)
    Script: consultor.c
    Integrantes:
         Diaz Adrian Maximiliano     38.167.742
         Rodriguez Gonzalo Martin    39.461.284
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "constantes.h"


int validarParametros(int arg) {
    //args[1] consulta
    if (arg != 2) {
        printf("\n cantidad de parametros incorrecta, verifique la ayuda");
        return 1;
    }

    return 0;
}

void mostrarAyuda() {
    printf("\n Ejemplo de ejecucion: \n ./consultar producto=P.DULCE ./fifoConsulta ./fifoResultado \n");
    //args[1] consulta
    //args[2] fifoConsulta
    //args[3] fifoResultado

}

void recibirResultado(mensaje *resultado) {
    int fds;
    mensaje *mem;
    sem_t *sem3 = sem_open(SEMAFORO_C, O_CREAT, 0600, 0); // crear semaforo
    sem_t *sem4 = sem_open(SEMAFORO_D, O_CREAT, 0600, 0);
    fds = shm_open(MEMORIA_RESULTADOS, O_CREAT | O_RDWR, 0600); // crear memoria compartida
    size_t tam = sizeof(mensaje);
    ftruncate(fds, tam);                                                       // definir tamaño
    mem =  mmap(NULL, tam, PROT_READ | PROT_WRITE, MAP_SHARED, fds, 0); // asociar espacio a variable

    sem_wait(sem3); // P()

    resultado->siguiente = mem->siguiente;
    strcpy(resultado->valor, mem->valor);

    sem_post(sem4);

    //printf("%s\n", mem);
    close(fds);
    munmap(mem, tam);
    sem_close(sem3);
    sem_close(sem4);
}

void enviarConsulta(char *consulta, int tam) {
    int fd;
    printf("\nconsulta enviada:%s\n", consulta);
    char *mem;
    sem_t *sem = sem_open(SEMAFORO_A, O_CREAT, 0600, 0);                       // crear semaforo
    sem_t *sem2 = sem_open(SEMAFORO_B, O_CREAT, 0600, 0);                     // crea segundo semaforo
    fd = shm_open(MEMORIA_CONSULTA, O_CREAT | O_RDWR, 0600);                         // crear memoria compartida
    ftruncate(fd, tam);                                                        // definir tamaño
    mem = (char *) mmap(NULL, tam, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // asociar espacio a variable
    strcpy(mem, consulta);    // escribo la consulta en la memoria compartida
    // una vez que envio el mensaje y recibo la confirmacion cierro todo
    sem_post(sem);  // V(); // libero el semaforo para que el proceso "ejercicio4" pueda leer de la memoria
    sem_wait(sem2); //P() // me quedo bloqueado hasta que se lea de la memoria
    close(fd);
    munmap(mem, tam);       // elino la asociacion de la variable con la memoria
    shm_unlink(MEMORIA_CONSULTA); // elimino la memoria compartida
    sem_close(sem);
    sem_unlink(SEMAFORO_A); // elimino el semaforo
    sem_close(sem2);
    sem_unlink(SEMAFORO_B);

}

int main(int arg, char *args[]) {
    //args[1] consulta

    char *entrada = args[1];
    int cantCaracteres = strlen(entrada);
    if (arg == 2 && (strcmp(args[1], "-h") == 0 || strcmp(args[1], "-help") == 0 || strcmp(args[1], "-?") == 0)) {
        mostrarAyuda();
        return 0;
    }
    if (validarParametros(arg) == 1) {
        return 1;
    }

    enviarConsulta(args[1], cantCaracteres + 1);
    mensaje men;
    men.siguiente = 1;
    while (men.siguiente) {
        recibirResultado(&men);
        printf("\n%s\n", men.valor);
    }

    return 0;
}
