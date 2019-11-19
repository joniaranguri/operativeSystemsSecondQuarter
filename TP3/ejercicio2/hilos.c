#include "hilos.h"

/// Validaciones

int validarParametros(int cantHilos, char * nombreArchivo)
{
    int rta = 0;

    rta += validarCantidadHilos(cantHilos);
    rta += validarExistenciaArchivo(nombreArchivo);

    return rta == 2 ? PARAMETROS_OK : PARAMETROS_ERROR;
}

int validarCantidadHilos(int cantHilos)
{
    return (cantHilos >= 0) ? PARAMETROS_OK : PARAMETROS_ERROR;
}

int validarExistenciaArchivo(char * nombreArchivo)
{
    FILE * arch = fopen(nombreArchivo, "rt");
    if (arch)
    {
        fclose (arch);
        return EXISTE_ARCHIVO;
    }
    return NO_EXISTE_ARCHIVO;
}

/// Manejo de archivos

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

/// Resolucion del problema

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

int mayor(int a, int b)
{
    return a > b ? a : b;
}

void sumarVec(int cantSumas, double * vec1, double * vec2, double * vecRes)
{
    while (cantSumas--)
    {
        (*vecRes) = (*vec1) + (*vec2);
        vecRes++;
        vec1++;
        vec2++;
    }
}

void crearHiloSuma(int cantSumas, double * vec1, double * vec2, double * vecRes)
{
    pthread_t hilo;
    pthread_create(&hilo, NULL, &sumarVec, cantSumas, vec1, vec2, vecRes);
    pthread_join();
}

void mostrarVector(double vec[], int c)
{
    for (int i = 0; i < c; i++)
    {
        printf("%f", vec[i]);
    }
}

void mostrarAyuda()
{
    printf("\n Ejemplo de ejecucion: \n");
    printf("\t ./ej2 4 vectores.txt \n");
}

