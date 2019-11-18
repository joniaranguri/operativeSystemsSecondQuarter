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
#include <ctype.h>
#include <sys/stat.h>
#include<string.h>


int validarPathFifo(char *path) {
    char finalPath[strlen(path)];
    char *barra = strrchr(path, '/');

    if (barra != NULL) {
        int cantidad = (strlen(path) - strlen(barra));
        strncpy(finalPath, path, cantidad);
        finalPath[cantidad] = '\0';
    } else {
        strcpy(finalPath, path);

    }

    struct stat myFile;
    if (stat(finalPath, &myFile) < 0) {
        printf("\nel directorio %s no existe\n ", finalPath);
        return 1;
    } else {

        return 0;
    }

}

int validarParametros(int arg, char *args[]) {
//args[0] nombre de script
//args[1] archivo
//args[2] fifo consulta
//args[3] fifo resultado
    if (arg != 4) {
        printf("\nCANTIDAD DE PARAMETROS INCORRECTOS,VERIFIQUE LA AYUDA\n");
        return 1;
    }

    struct stat myFile;
    if (stat(args[1], &myFile) < 0) {
        printf("\nno se encontro el archivo %s\n", args[1]);
        return 1;
    }


    if (validarPathFifo(args[2]) == 1)
        return 1;
    if (validarPathFifo(args[3]) == 1)
        return 1;


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

void filtrarArchivo(char *path[], char *filtro, int registros, char *salida) {

    FILE *pf;
    int esId = strncmp("ID", filtro, 2);
    int esProducto = strncmp("PRODUCTO", filtro, 8);
    int esMarca = strncmp("MARCA", filtro, 5);
    // printf("esMarca: %d",esMarca);
    int j = 0;
    printf("valor j=%d\n", j);
    char id[100];
    char articulo[100];

    char producto[100];
    char marca[100];
    char *igual = strchr(filtro, '=');
    char *buscado = igual;

    strcpy(salida, " ");
    buscado++;
    printf("buscado: %s\n", buscado);

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

        j++;
        fflush(stdin);
        fscanf(pf, " %[^;]", id);

        //  printf("id: %s",id);

        fflush(stdin);
        fscanf(pf, " ;%[^;]", articulo);
        //  printf("articulo: %s",articulo);

        fflush(stdin);
        fscanf(pf, " ;%[^;]", producto);
        // printf("producto: %s",producto);

        fflush(stdin);
        fscanf(pf, " ;%[^\r|\n]", marca);
        printf("id: %s articulo: %s producto:%s marca: %s\n", id, articulo, producto, marca);

        // printf(" registro:%d marca leida: %s",j,marca);

        // printf("marcaLeida:%s marcaBuscada:%s resultadoComparacion:%d\n",marca,buscado,valorComparacion);
        if (esId == 0 && strcmp(id, buscado) == 0) {

            agregarSalida(salida, id, articulo, producto, marca);

        } else if (esMarca == 0 && strcmp(marca, buscado) == 0) {

            agregarSalida(salida, id, articulo, producto, marca);
        } else if (esProducto == 0 && strcmp(producto, buscado) == 0) {

            agregarSalida(salida, id, articulo, producto, marca);
        }

        strcpy(id, " ");
        strcpy(articulo, " ");
        strcpy(producto, " ");
        strcpy(marca, " ");

    }
    fclose(pf);

    return;
}

int main(int arg, char *args[]) {
    int x;
    int num = arg;

    // args[1] archivo
    // args[2] fifoConsulta
    //args[3] fifoResultado

    if (arg == 2 && (strcmp(args[1], "-h") == 0 || strcmp(args[1], "-?") == 0 || strcmp(args[1], "-help") == 0)) {

        mostrarAyuda();

        return 0;
    }
    int res = validarParametros(arg, args);
    if (res == 1) {
        return 0;
    }
    /*
    // creo un proceso hijo
    x = fork();
    //si es el padre termino, asi el hijo queda como demonio

   
    if (x > 0)
    {
        return 0;
    }*/


// crear fifos
    mkfifo(args[2], 0666); //fifo consulta
    mkfifo(args[3], 0666); //fifo resultado

    //si es el hijo se queda ejecutando
    while (1) {

        // abrir fifos 
        int fd = open(args[2], O_RDONLY);


        int fds = open(args[3], O_WRONLY);


        char filtro[100];
        int bytes = -1;
        printf("\n*******************ESPERANDO CONSULTA*******************\n");
        bytes = read(fd, filtro, sizeof(filtro)); // leer fifo
        printf("\n ****** LLEGO LA CONSULTA********\n");

        int cantCaracteres = strlen(filtro);
        char *aMayuscula = filtro;


        // pongo en mayuscula el filtro
        for (int i = 0; i < cantCaracteres; i++) {
            *aMayuscula = toupper(*aMayuscula);
            aMayuscula++;
        }


        int registros = obtenerCantidadDeRegistros(&args[1]);

        char salida[registros * 40];
        printf("sizeofSalida: %ld", sizeof(salida));

        filtrarArchivo(&args[1], filtro, registros, salida);
        printf("\n******ARCHIVO FILTRADO*******\n");


        write(fds, salida, sizeof(salida) + 1);

        close(fds);
        close(fd);
    }


    return (0);
}