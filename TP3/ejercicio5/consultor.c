
/* Trabajo práctico N3 Ejercicio 5 (Primera entrega)
    Script: consultor.c
    Integrantes:
         Aranguri Jonathan Enrique   40.672.991	
         Diaz Adrian Maximiliano     38.167.742
         Rodriguez Gonzalo Martin    39.461.284
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


int validarParametros(int arg, char *args[]) {
    if (arg != 3) {
        return 1;
    }

    return 0;
}

void mostrarAyuda() {
    printf("\n Ejemplo de ejecucion: \n ./consultar archivoDeConfig");
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
    const char *END_REQUEST = "QUIT";
    struct sockaddr_in direcciónServidor;
    direcciónServidor.sin_family = AF_INET;
    direcciónServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    direcciónServidor.sin_port = htons(obtenerPuerto(args[1]));


    int cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(cliente, (void *) &direcciónServidor, sizeof(direcciónServidor)) != 0) {
        perror("No se pudo conectar");
        return 1;
    }
    char mensaje[1000];
    printf("Escriba su consulta: ");
    scanf("%s", mensaje);
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
    }

    return 0;
}
