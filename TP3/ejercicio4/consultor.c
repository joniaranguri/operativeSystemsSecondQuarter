/* Trabajo práctico N3 Ejercicio 3 (Primera entrega)
    Script: ejercicio3.c
    Integrantes:
         Aranguri Jonathan Enrique   40.672.991	
         Diaz Adrian Maximiliano     38.167.742
         Rodriguez Gonzalo Martin    39.461.284
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/mman.h>

int validarParametros(int arg, char *args[])
{
    if (arg != 2)
    {
        return 1;
    }

    return 0;
}

void mostrarAyuda()
{
    printf("\n Ejemplo de ejecucion: \n ./consultar producto=P.DULCE");
}

int main(int arg, char *args[])
{

    char *entrada = args[1];

    int tam = sizeof(char) * 30;

    if (arg == 2 && (strcmp(args[1], "-h") == 0 || strcmp(args[1], "-help") == 0 || strcmp(args[1], "-?") == 0))
    {
        mostrarAyuda();
        return 0;
    }
    // if (validarParametros(arg, args) == 1)
    //   return 1;
    int fd;
    char *mem;
    sem_t *sem = sem_open("semaforo", O_CREAT, 0600, 0);                      // crear semaforo
    sem_t *sem2 = sem_open("semaforo2", O_CREAT, 0600, 0);                    // crea segundo semaforo
    fd = shm_open("/nombre1", O_CREAT | O_RDWR, 0600);                        // crear memoria compartida
    ftruncate(fd, tam);                                                       // definir tamaño
    mem = (char *)mmap(NULL, tam, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // asociar espacio a variable
    strcpy(mem, entrada); // escribo la consulta en la memoria compartida
    // una vez que envio el mensaje y recibo la confirmacion cierro todo
    sem_post(sem);  // V(); // libero el semaforo para que el proceso "ejercicio3" pueda leer de la memoria
    sem_wait(sem2); //P() // me quedo bloqueado hasta que se lea de la memoria
    close(fd);
    munmap(mem, tam); // elino la asociacion de la variable con la memoria
    shm_unlink("/nombre1");// elimino la memoria compartida
    sem_close(sem);
    sem_unlink("semaforo"); // elimino el semaforo
    sem_close(sem2);
    sem_unlink("semaforo2");

    return 0;
}
