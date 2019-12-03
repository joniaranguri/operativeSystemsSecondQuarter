/* Trabajo práctico N3 Ejercicio 5 (Segunda entrega)
    Script: ejercicio5.c
    Integrantes:
         Diaz Adrian Maximiliano     38.167.742
         Rodriguez Gonzalo Martin    39.461.284
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void mostrarAyuda() {
    printf("\n Ejemplo de ejecucion: \n ./ej5 ./archivoProductos ./archivoConf \n");
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

int validarParametros(int arg, char *args[]) {
    //args[0] nombre de script
    //args[1] archivo de productos
    //args[2] archivo de configuracion

    if (arg != 3) {
        printf("\nCANTIDAD DE PARAMETROS INCORRECTOS,VERIFIQUE LA AYUDA\n");
        return 1;
    }

    struct stat myFile;
    if (stat(args[1], &myFile) < 0) {
        printf("\nno se encontro el archivo %s\n", args[1]);
        return 1;
    }

    if (arg == 3) {
        if (stat(args[2], &myFile) < 0) {
            printf("\nno se encontro el archivo %s\n", args[2]);
            return 1;
        }
    }

    return 0;
}

int obtenerCantidadDeRegistros(char *path[]) {
    FILE *pf;
    int cantfilas = 0;
    pf = fopen(*path, "r");
    if (!pf) {
        printf("no se encuentra el archivo\n");
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
    char id[100];
    char articulo[100];

    char producto[100];
    char marca[100];
    char *igual = strchr(filtro, '=');
    char *buscado = igual;

    strcpy(salida, " ");
    buscado++;

    // printf("\nFILTRO: %s\n", filtro);
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
        // printf("id: %s articulo: %s producto:%s marca: %s\n", id, articulo, producto, marca);

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
    strcat(salida, "\0");
    fclose(pf);

    return;
}

int obtenerPuerto(char *archivo) {
    int puerto = 0;
    FILE *fp = fopen(archivo, "r");
    fscanf(fp, "%d", &puerto);

    return puerto;
}

int main(int arg, char *args[]) {
    if (arg == 2 && (strcmp(args[1], "-h") == 0 || strcmp(args[1], "-?") == 0 || strcmp(args[1], "-help") == 0)) {
        mostrarAyuda();
        return 0;
    }
    if (validarParametros(arg, args) == 1)
        return 1;
    int x;
    x = fork();
    if (x > 0)
        return 1;
    struct sockaddr_in direccionServidor;
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = INADDR_ANY;
    direccionServidor.sin_port = htons(obtenerPuerto(args[2]));

    int servidor = socket(AF_INET, SOCK_STREAM, 0);

    int activado = 1;
    setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

    if (bind(servidor, (void *) &direccionServidor, sizeof(direccionServidor)) != 0) {
        perror("Falló el bind");
        return 1;
    }

    //	printf("Estoy escuchando\n");
    listen(servidor, SOMAXCONN);

    //------------------------------

    struct sockaddr_in direccionCliente;
    unsigned int tamanioDireccion;

    while (1) {
        int cliente = accept(servidor, (void *) &direccionCliente, &tamanioDireccion);

//        printf("Recibí una conexión en %d!!\n", cliente);

        if (fork()) {
            char *buffer = malloc(1000);

            while (1) {

                int bytesRecibidos = recv(cliente, buffer, 1000, 0);
                if (bytesRecibidos <= 0) {
                    continue;
                }

                buffer[bytesRecibidos] = '\0';

                int cantCaracteres = strlen(buffer);
                char *aMayuscula = buffer;

                // pongo en mayuscula el filtro
                for (int i = 0; i < cantCaracteres; i++) {
                    *aMayuscula = toupper(*aMayuscula);
                    aMayuscula++;
                }

                int registros = obtenerCantidadDeRegistros(&args[1]);

                char salida[registros * 40];

                filtrarArchivo(&args[1], buffer, registros, salida);

                //  printf("\n******ARCHIVO FILTRADO*******\n");

                int number_to_send = strlen(salida);
                // Put your value
                char salida_[number_to_send];
//                int converted_number = htonl(number_to_send);
                strcpy(salida_, salida);

                // Write the number to the opened socket
                write(cliente, &number_to_send, sizeof(number_to_send));

                send(cliente, salida_, sizeof(salida_), 0);
            }

            free(buffer);
        }
    }

    return 0;
}
