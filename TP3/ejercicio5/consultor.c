
/* Trabajo práctico N3 Ejercicio 5 (Segunda entrega)
    Script: consultor.c
    Integrantes:
         Diaz Adrian Maximiliano     38.167.742
         Rodriguez Gonzalo Martin    39.461.284
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>

int validarConsulta(char *consulta) {
    char *igual = strchr(consulta, '=');
    if (!igual)
        return 1;
    return 0;
}

int validarParametros(int arg, char *args[]) {
    if (arg != 2) {
        printf("\n cantidad de paramentros incorrecta , verifique la ayuda\n");
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
    printf("\n Ejemplo de ejecucion: \n ./consultar archivoDeConfig \n Se pediran las consultas interactivamente la terminal \n Para finaliza ingrese QUIT\n");
}

int main(int arg, char *args[]) {
    if (arg == 2 && (strcmp(args[1], "-h") == 0 || strcmp(args[1], "-?") == 0 || strcmp(args[1], "-help") == 0)) {
        mostrarAyuda();
        return 0;
    }
    if (validarParametros(arg, args) == 1)
        return 1;
    const char *END_REQUEST = "QUIT";
    struct sockaddr_in direccionServidor;
    direccionServidor.sin_family = AF_INET;

    char ip[16];
    int puerto = 0;
    FILE *fp = fopen(args[1], "r");

    strcpy(ip, "");

    fflush(stdin);
    fscanf(fp, "%d", &puerto);
    fflush(stdin);
    fscanf(fp, "%s", ip);

    fclose(fp);

    direccionServidor.sin_addr.s_addr = inet_addr(ip);
    direccionServidor.sin_port = htons(puerto);

    int cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(cliente, (void *) &direccionServidor, sizeof(direccionServidor)) != 0) {
        perror("\nNo se pudo conectar\n");
        return 1;
    }
    char mensaje[1000];
    printf("\nEscriba su consulta: ");
    scanf("%s", mensaje);
    while (validarConsulta(mensaje) == 1) {
        printf("\nConsulta invalida no se encuenta el = \nEscriba su consulta: ");
        scanf("%s", mensaje);
    }

    while (strcmp(mensaje, END_REQUEST) != 0) {

        send(cliente, mensaje, strlen(mensaje), 0);

        printf("\nSE ENVIO LA CONSULTA\n");

        int received_int = 0;

        read(cliente, &received_int, sizeof(received_int));

        char respuesta[ntohl(received_int)];

        int bytesRecibidos = recv(cliente, respuesta, ntohl(received_int), 0);
        respuesta[bytesRecibidos] = '\0';
        printf("%s\n", respuesta);

        printf("\nEscriba su consulta: ");

        scanf("%s", mensaje);
        while (validarConsulta(mensaje) == 1) {
            printf("\nConsulta invalida no se encuenta el = \nEscriba su consulta: ");
            scanf("%s", mensaje);
        }
    }

    return 0;
}
