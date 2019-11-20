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
#include<math.h>
#include<pthread.h>

#define NOMBRE_ARCHIVO "vectores.txt"
#define EXISTE_ARCHIVO 1
#define NO_EXISTE_ARCHIVO 0
#define PARAMETROS_OK 1
#define PARAMETROS_ERROR 0

struct info {
	int cantSumas;
	double * vec1;
	double * vec2;
	double * vecRes;
};

int validarParametros(int arg, char *args[]) {
//args[0] nombre de script
//args[1] cantidad de hilos
//args[2] archivo
if (arg != 3) {
        printf("\nCANTIDAD DE PARAMETROS INCORRECTOS,VERIFIQUE LA AYUDA\n");
        return 1;
    }

    struct stat myFile;
    if (stat(args[1], &myFile) < 0) {
        printf("\nNo se encontro el archivo %s\n", args[2]);
        return 1;
	}

return 0;
}

void mostrarAyuda() {
    printf("\n Ejemplo de ejecucion:\n");
    printf("\t ./ejercicio2  4 ./archivo.txt \n");
    return;
}

int obtenerTamanoVectoresDeArchivo(char * nombreArchivo)
{
    int i = 0;
    char linea[20] = "";
    FILE * archivo = fopen(nombreArchivo, "rt");
    {
        if (archivo)
        {
            while (fgets(linea, 20, archivo))
            {
                i++;
            }
        }
    }
    return i;
}

int obtenerVectoresDeArchivo(double * vec1, double * vec2, char * nombre)
{
    int rta = 0;
    char linea[20] = "";
    FILE * archivo = fopen(nombre, "rt");
    if (archivo)
    {
        while (fgets(linea, 20, archivo))
        {
            sscanf(linea, "%lf, %lf", vec1++, vec2++);
            rta ++;
        }
    }
    fclose(archivo);
    return rta;
}

void grabarVectorResultado(double * vecRes, int cantPos)
{
    FILE * arch = fopen("resultados.txt", "wt");

    if (arch)
    {
        while (cantPos--)
        {
            char string[20]="";
            sprintf(string, "%lf", *vecRes);
            fputs(string, arch);
            vecRes++;
        }
    }

    fclose(arch);
}

int mayor(int a, int b)
{
    return a > b ? a : b;
}

void* sumarVec(void * data)
{
	struct info * dato = data;
    while ((dato->cantSumas)--)
    {
        *(dato->vecRes) = *(dato->vec1) + *(dato->vec2);
       	(dato->vecRes)++;
        (dato->vec1)++;
        (dato->vec2)++;
    }
}

void crearHiloSuma(int cantSumas, double * vec1, double * vec2, double * vecRes)
{
    pthread_t hilo;
	struct info * data;
data->cantSumas = cantSumas;
data->vec1 = vec1;
data->vec2 = vec2;
data->vecRes = vecRes;
    pthread_create(&hilo, NULL, *sumarVec, data);
    pthread_join(hilo, NULL);
}

int sumarVectores(int cantHilos, double * vec1, double * vec2, double * vecRes, int cantSumas)
{
    int sumasFaltantes = cantSumas, sumasPorHilo = (int) ceil((float)cantSumas / (float)cantSumas), hilo = 1;

    while (sumasFaltantes > 0)
    {
        if (sumasPorHilo < sumasFaltantes)
        {
            printf("Thread %d: %d sumas\n", hilo++, sumasPorHilo);
            crearHiloSuma(sumasPorHilo, vec1 + cantSumas - sumasFaltantes, vec2 + cantSumas - sumasFaltantes, vecRes + cantSumas - sumasFaltantes);
        }
        else
        {
            printf("Thread %d: %d sumas\n", hilo++, sumasFaltantes);
            crearHiloSuma(sumasFaltantes, vec1 + cantSumas - sumasFaltantes, vec2 + cantSumas - sumasFaltantes, vecRes + cantSumas - sumasFaltantes);
        }

        sumasFaltantes -= sumasPorHilo;
    }

    return 1;
}

void mostrarVector(double vec[], int c)
{
    for (int i = 0; i < c; i++)
    {
        printf("%f", vec[i]);
    }
}

int main(int cantHilos, char * args[])
{
    int exito = 0;
    if (validarParametros(cantHilos, args))
    {
        int cantPos = 0;
        if (cantPos = obtenerTamanoVectoresDeArchivo(args[0]) != 0)
        {
            double vec1[cantPos], vec2[cantPos], vecRes[cantPos];
            if (obtenerVectoresDeArchivo(vec1, vec2, args[0]))
            {
                exito = sumarVectores(cantHilos, vec1, vec2, vecRes, cantPos);
                if (exito)
                {
                    grabarVectorResultado(vecRes, cantPos);
                }
            }
        }
    }
    else
    {
        mostrarAyuda();
return 0;
    }
	return 1;
}