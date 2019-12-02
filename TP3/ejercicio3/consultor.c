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
#include <sys/stat.h>
#include <string.h>


int validarParametros(int arg, char *args[])
{
       //args[1] consulta
    //args[2] fifoConsulta
    //args[3] fifoResultado
    if (arg != 4)
    {
        printf("\n cantidad de parametros incorrecta, verifique la ayuda");
        return 1;
    }
    struct stat myFile;
    if (stat(args[2], &myFile) < 0)
    {
        printf("\nno se encontro el fifo: %s \n", args[2]);
        printf("\n Debe ejecutar el proceso ej3 primero\n");
        return 1;
    }
    if (stat(args[3], &myFile) < 0)
    {
        printf("\nno se encontro el fifo: %s \n", args[3]);
        printf("\n Debe ejecutar el proceso ej3 primero\n");
        return 1;
    }

    return 0;
}

void mostrarAyuda()
{
    printf("\n Ejemplo de ejecucion: \n");
    printf(" \t ./consultar producto=P.DULCE ./fifoConsulta ./fifoResultado \n");
    printf("\t ./consultar marca=MAROLIO ./fifoConsulta ./fifoResultado \n");
    printf("\t ./consultar id=16008 ./fifoConsulta ./fifoResultado \n");
    
    //args[1] consulta
    //args[2] fifoConsulta
    //args[3] fifoResultado
  
}
void recibirResultado(int *fds, char *resultado, int tam){
    read(*fds, resultado, tam); // leer fifo
}
void enviarConsulta(int *fd, char *salida, int tam)
{
    printf("\nconsultaenviada:%s\n", salida);
    write(*fd, salida, tam);
}
void abrirFifos(int *fd, char *fifoConsulta, int *fds, char *fifoResultado)
{
    *fd = open(fifoConsulta, O_WRONLY);   //fd
    *fds = open(fifoResultado, O_RDONLY); //fds
}
void recibirCantidadFiltrados(int *fds,int *cantidad){
    read(*fds,cantidad,sizeof(int));
    printf("\ncantidad filtrado: %d\n", *cantidad);
}
int main(int arg, char *args[])
{
    //args[1] consulta
    //args[2] fifoConsulta
    //args[3] fifoResultado

 
    if (arg == 2 && (strcmp(args[1], "-h") == 0 || strcmp(args[1], "-help") == 0 || strcmp(args[1], "-?") == 0))
    {
        mostrarAyuda();
        return 0;
    }
    if (validarParametros(arg, args) == 1)
        return 1;
    char *entrada = args[1];
    int cantCaracteres = strlen(entrada);
    int fd;
    int fds;
   
    abrirFifos(&fd, args[2], &fds, args[3]);

    enviarConsulta(&fd, args[1], cantCaracteres+1);
    int i=0;
    int cantidadFiltrados;
  recibirCantidadFiltrados(&fds,&cantidadFiltrados);
    char resultado[cantidadFiltrados*100];
  printf("\n****************SALIDAAAAA*******************\n");
    //for(i=0;i<cantidadFiltrados;i++){
    recibirResultado(&fds, resultado, cantidadFiltrados*100);
     printf("\n%s\n", resultado);

    //}
  
    return 0;
}
