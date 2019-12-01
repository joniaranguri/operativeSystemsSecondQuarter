/* Trabajo práctico N3 Ejercicio 3 (Segunda entrega)
    Script: ejercicio3.c
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


void enviarArchivoFiltrado(char *salida, int siguiente) {
    int fds;
    mensaje *mem;
    sem_t *sem3 = sem_open(SEMAFORO_C, O_CREAT, 0600, 0); // crear semaforo
    sem_t *sem4 = sem_open(SEMAFORO_D, O_CREAT, 0600, 0); // crea segundo semaforo
    size_t tam = sizeof(mensaje);
    fds = shm_open(MEMORIA_RESULTADOS, O_CREAT | O_RDWR, 0600);                              // crear memoria compartida
    ftruncate(fds, tam);                                                        // definir tamaño
    mem = mmap(NULL, tam, PROT_READ | PROT_WRITE, MAP_SHARED, fds, 0); // asociar espacio a variable
    mem->siguiente = siguiente;
    strcpy(mem->valor, salida);

    sem_post(sem3);                                                              // V();
    sem_wait(sem4);                                                              //P()

    close(fds);
    munmap(mem, tam);
    sem_close(sem3);
    sem_close(sem4);
}

int validarParametros(int arg, char *args[]) {
    //args[0] nombre de script
    //args[1] archivo
    if (arg != 2) {
        printf("\nCANTIDAD DE PARAMETROS INCORRECTOS,VERIFIQUE LA AYUDA\n");
        return 1;
    }

    struct stat myFile;
    if (stat(args[1], &myFile) < 0) {
        printf("\nno se encontro el archivo %s\n", args[1]);
        return 1;
    }

    return 0;
}

void mostrarAyuda() {
    printf("\n Ejemplo de ejecucion:\n");
    printf("\t ./ej3  ./articulos.txt ./fifoConsulta ./fifoResultado \n");
    return;
}

void agregarSalida(char out[], char id[], char articulo[], char producto[], char marca[]) {
    strcat(out, id);
    strcat(out, ";");
    strcat(out, articulo);
    strcat(out, ";");
    strcat(out, producto);
    strcat(out, ";");
    strcat(out, marca);
    strcat(out, "\n");
    return;
}

int obtenerCantidadDeRegistros(char *path[]) {
    FILE *pf;
    int cantfilas = 0;
    pf = fopen(*path, "r");
    if (!pf) {
        printf("no se encuentra el archivo");
        exit(0);
    }
    char fila[100];
    while (!feof(pf)) {
        fscanf(pf, " %[^\n]", fila);
        cantfilas++;
    }
    fclose(pf);
    return cantfilas;
}

void filtrarArchivo(char *path[], char *filtro, char *salida) {

    FILE *pf;
    int esId = strncmp("ID", filtro, 2);
    int esProducto = strncmp("PRODUCTO", filtro, 8);
    int esMarca = strncmp("MARCA", filtro, 5);

    char id[60];
    char articulo[60];

    char producto[60];
    char marca[60];
    char *igual = strchr(filtro, '=');
    char *buscado = igual;

    strcpy(salida, " ");
    buscado++;

    printf("\nFILTRO: %s\n", filtro);
    pf = fopen(*path, "r");
    if (!pf) {
        printf("\nno se  encontro el archivo %s\n", *path);
        exit(0);
    }

    strcpy(id, " ");
    strcpy(articulo, " ");
    strcpy(producto, " ");
    strcpy(marca, " ");

    while (!feof(pf)) {

        fflush(stdin);
        fscanf(pf, " %[^;]", id);

        fflush(stdin);
        fscanf(pf, " ;%[^;]", articulo);


        fflush(stdin);
        fscanf(pf, " ;%[^;]", producto);


        fflush(stdin);
        fscanf(pf, " ;%[^\r|\n]", marca);

        if (esId == 0 && strcmp(id, buscado) == 0) {
            agregarSalida(salida, id, articulo, producto, marca);
            enviarArchivoFiltrado(salida, 1);
        } else if (esMarca == 0 && strcmp(marca, buscado) == 0) {
            agregarSalida(salida, id, articulo, producto, marca);
            enviarArchivoFiltrado(salida, 1);
        } else if (esProducto == 0 && strcmp(producto, buscado) == 0) {
            agregarSalida(salida, id, articulo, producto, marca);
            enviarArchivoFiltrado(salida, 1);
        }

        strcpy(id, " ");
        strcpy(articulo, " ");
        strcpy(producto, " ");
        strcpy(marca, " ");
        strcpy(salida, " ");
    }

    enviarArchivoFiltrado("", 0);
    fclose(pf);
}

void recibirConsulta(char *filtro) {
    int fd;
    char *mem;
    sem_t *sem = sem_open(SEMAFORO_A, O_CREAT, 0600, 0); // crear semaforo
    sem_t *sem2 = sem_open(SEMAFORO_B, O_CREAT, 0600, 0);
    size_t tam = sizeof(char) * 100;
    fd = shm_open(MEMORIA_CONSULTA, O_CREAT | O_RDWR, 0600);                    // crear memoria compartida
    ftruncate(fd, tam);
    mem = (char *) mmap(NULL, tam, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // asociar espacio a variable
    sem_wait(sem); // P() // me quedo bloquedo hasta poder leer de la memoria
    printf("%s\n", mem);
    strcpy(filtro, mem);
    sem_post(sem2); // V()
    close(fd);
    munmap(mem, tam);
    sem_close(sem);
    sem_close(sem2);
}

int main(int arg, char *args[]) {
    shm_unlink(MEMORIA_RESULTADOS);
    sem_unlink(SEMAFORO_A);
    sem_unlink(SEMAFORO_B);
    sem_unlink(SEMAFORO_C);
    sem_unlink(SEMAFORO_D);
    int x;
    // args[1] archivo

    if (arg == 2 && (strcmp(args[1], "-h") == 0 || strcmp(args[1], "-?") == 0 || strcmp(args[1], "-help") == 0)) {
        mostrarAyuda();
        return 0;
    }

    int res = validarParametros(arg, args);
    if (res == 1) {
        return 0;
    }
    //creo un proceso hijo
     x = fork();
    //si es el padre termino, asi el hijo queda como demonio


    if (x > 0)
    {
        return 0;
    }

    //si es el hijo se queda ejecutando
    while (1) {
        int fd;
        int fds;

        char filtro[100] = "";
        char *aMayuscula = filtro;

        recibirConsulta(filtro);

        int cantCaracteres = strlen(filtro);

        // pongo en mayuscula el filtro
        for (int i = 0; i < cantCaracteres; i++) {
            *aMayuscula = toupper(*aMayuscula);
            aMayuscula++;
        }
        char salida[100];
        filtrarArchivo(&args[1], filtro, salida);

        close(fds);
        close(fd);
    }

    return (0);
}
