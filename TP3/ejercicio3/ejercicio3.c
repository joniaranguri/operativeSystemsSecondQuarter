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
void enviarArchivoFiltrado(int *fds, char *salida, int tam)
{

    write(*fds, salida, tam);
}
int validarPathFifo(char *path)
{
    char finalPath[strlen(path)];
    char *barra = strrchr(path, '/');

    if (barra != NULL)
    {
        int cantidad = (strlen(path) - strlen(barra));
        strncpy(finalPath, path, cantidad);
        finalPath[cantidad] = '\0';
    }
    else
    {
        strcpy(finalPath, path);
    }

    struct stat myFile;
    if (stat(finalPath, &myFile) < 0)
    {
        printf("\nel directorio %s no existe\n ", finalPath);
        return 1;
    }
    else
    {

        return 0;
    }
}

int validarParametros(int arg, char *args[])
{
    //args[0] nombre de script
    //args[1] archivo
    //args[2] fifo consulta
    //args[3] fifo resultado
    if (arg != 4)
    {
        printf("\nCANTIDAD DE PARAMETROS INCORRECTOS,VERIFIQUE LA AYUDA\n");
        return 1;
    }

    struct stat myFile;
    if (stat(args[1], &myFile) < 0)
    {
        printf("\nno se encontro el archivo %s\n", args[1]);
        return 1;
    }

    if (validarPathFifo(args[2]) == 1)
        return 1;
    if (validarPathFifo(args[3]) == 1)
        return 1;

    return 0;
}

void mostrarAyuda()
{
    printf("\n Ejemplo de ejecucion:\n");
    printf("\t ./ej3  ./articulos.txt ./fifoConsulta ./fifoResultado \n");
    return;
}

void agregarSalida(char out[], char id[], char articulo[], char producto[], char marca[])
{
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

int obtenerCantidadDeRegistros(char *path[])
{
    FILE *pf;
    int cantfilas = 0;
    pf = fopen(*path, "r");
    if (!pf)
    {
        printf("no se encuentra el archivo");
        exit(0);
    }
    char fila[100];
    while (!feof(pf))
    {
        fscanf(pf, " %[^\n]", fila);
        cantfilas++;
    }
    fclose(pf);
    return cantfilas;
}
void informarCantidadRegistrosFiltrado(char *path[] , char *filtro ,int registros,int *fds){
      FILE *pf;
    int esId = strncmp("ID", filtro, 2);
    int esProducto = strncmp("PRODUCTO", filtro, 8);
    int esMarca = strncmp("MARCA", filtro, 5);
   
    char id[60];
    char articulo[60];
    int cantidad=0;
    char producto[60];
    char marca[60];
    char *igual = strchr(filtro, '=');
    char *buscado = igual;


    buscado++;
    printf("buscado: %s\n", buscado);

    printf("\nFILTRO: %s\n", filtro);
    pf = fopen(*path, "r");
    if (!pf)
    {

        printf("\nno se  encontro el archivo %s\n", *path);
        exit(0);
    }
    strcpy(id, " ");
    strcpy(articulo, " ");
    strcpy(producto, " ");
    strcpy(marca, " ");

    while (!feof(pf))
    {

        fflush(stdin);
        fscanf(pf, " %[^;]", id);

    

        fflush(stdin);
        fscanf(pf, " ;%[^;]", articulo);
    

        fflush(stdin);
        fscanf(pf, " ;%[^;]", producto);
        

        fflush(stdin);
        fscanf(pf, " ;%[^\r|\n]", marca);
      
        if (esId == 0 && strcmp(id, buscado) == 0)
        {
            cantidad++;
        }
        else if (esMarca == 0 && strcmp(marca, buscado) == 0)
        {
 cantidad++;
        }
        else if (esProducto == 0 && strcmp(producto, buscado) == 0)
        {

             cantidad++;
        }

        strcpy(id, " ");
        strcpy(articulo, " ");
        strcpy(producto, " ");
        strcpy(marca, " ");
    }
    fclose(pf);

    write(*fds, &cantidad,sizeof(int));
    printf("\ncantidad de registros enviados: %d\n", cantidad);


}
void filtrarArchivo(char *path[], char *filtro, int registros, char *salida, int *fds)
{

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
    printf("buscado: %s\n", buscado);

    printf("\nFILTRO: %s\n", filtro);
    pf = fopen(*path, "r");
    if (!pf)
    {

        printf("\nno se  encontro el archivo %s\n", *path);
        exit(0);
    }
    strcpy(id, " ");
    strcpy(articulo, " ");
    strcpy(producto, " ");
    strcpy(marca, " ");

    while (!feof(pf))
    {

        fflush(stdin);
        fscanf(pf, " %[^;]", id);

        fflush(stdin);
        fscanf(pf, " ;%[^;]", articulo);
     

        fflush(stdin);
        fscanf(pf, " ;%[^;]", producto);
      

        fflush(stdin);
        fscanf(pf, " ;%[^\r|\n]", marca);
       
        if (esId == 0 && strcmp(id, buscado) == 0)
        {

            agregarSalida(salida, id, articulo, producto, marca);
            int tam=0;
            tam=strlen(salida);
            enviarArchivoFiltrado(fds, salida,100);
        }
        else if (esMarca == 0 && strcmp(marca, buscado) == 0)
        {
            int tam=0;
            agregarSalida(salida, id, articulo, producto, marca);
            tam=strlen(salida);
            enviarArchivoFiltrado(fds, salida,100);
        }
        else if (esProducto == 0 && strcmp(producto, buscado) == 0)
        {   int tam=0;
            agregarSalida(salida, id, articulo, producto, marca);
            tam=strlen(salida);
            enviarArchivoFiltrado(fds, salida, 100);
        }

        strcpy(id, " ");
        strcpy(articulo, " ");
        strcpy(producto, " ");
        strcpy(marca, " ");
        strcpy(salida," ");
    }
    fclose(pf);

}
void crearFifos(char *fifoConsulta, char *fifoResultado)
{

    mkfifo(fifoConsulta, 0666);  //fifo consulta
    mkfifo(fifoResultado, 0666); //fifo resultado
}
void recibirConsulta(int *fd, char *filtro, int tam)
{
    read(*fd, filtro, tam+1); // leer fifo

}

void abrirFifos(int *fd, char *fifoConsulta, int *fds, char *fifoResultado)
{
    *fd = open(fifoConsulta, O_RDONLY);   //fd
    *fds = open(fifoResultado, O_WRONLY); //fds
}

int main(int arg,char *args[])
{
    int x;
    

    // args[1] archivo
    // args[2] fifoConsulta
    //args[3] fifoResultado

    if (arg == 2 && (strcmp(args[1], "-h") == 0 || strcmp(args[1], "-?") == 0 || strcmp(args[1], "-help") == 0))
    {

        mostrarAyuda();

        return 0;
    }
    int res = validarParametros(arg, args);
    if (res == 1)
    {
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

   
    crearFifos(args[2], args[3]);

    //si es el hijo se queda ejecutando   

    while (1)
    {
        int fd;
        int fds;
        abrirFifos(&fd, args[2], &fds, args[3]);

        printf("\n*******************ESPERANDO CONSULTA*******************\n");
        char filtro[100]="";
        char *aMayuscula = filtro;

        recibirConsulta(&fd, filtro,100);
        printf("\n ****** LLEGO LA CONSULTA********\n");

        int cantCaracteres = strlen(filtro);

        // pongo en mayuscula el filtro
        for (int i = 0; i < cantCaracteres; i++)
        {
            *aMayuscula = toupper(*aMayuscula);
            aMayuscula++;
        }
        int registros = obtenerCantidadDeRegistros(&args[1]);
        char salida[100];
        char *pS = salida;
        informarCantidadRegistrosFiltrado(&args[1],filtro,registros,&fds);
        filtrarArchivo(&args[1], filtro, registros, salida,&fds);
        printf("\n******ARCHIVO FILTRADO*******\n");


        printf("\nse mando el resultado\n");

        close(fds);
        close(fd);
    }

    return (0);
}
