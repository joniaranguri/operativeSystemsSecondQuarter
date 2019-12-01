/* Trabajo práctico N3 Ejercicio 2 (Primera entrega)
    Script: funciones.c
    Integrantes:
         Aranguri Jonathan Enrique   40.672.991	
         Diaz Adrian Maximiliano     38.167.742
         Rodriguez Gonzalo Martin    39.461.284
*/

#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include<math.h>
#include<pthread.h>
#include <stdlib.h>

#define NOMBRE_ARCHIVO "vectores.txt"
#define EXISTE_ARCHIVO 1
#define NO_EXISTE_ARCHIVO 0
#define PARAMETROS_OK 1
#define PARAMETROS_ERROR 0

struct info {
    int desde;
    int hasta;
    double *vec1;
    double *vec2;
    double *vecRes;
};

int validarParametros(int cantHilos, char *nombre);

void mostrarAyuda();

int obtenerTamanoVectoresDeArchivo(char *nombreArchivo);

int obtenerVectoresDeArchivo(double *vec1, double *vec2, char *nombre);

void grabarVectorResultado(double *vecRes, int cantPos);

void *sumarVec(void *data);

void crearHiloSuma(int desde, int hasta, double *vec1, double *vec2, double *vecRes);

int sumarVectores(int cantHilos, double *vec1, double *vec2, double *vecRes, int cantSumas);

int main(int params, char *args[]) {
    int exito = 0;
    if (params == 3 && validarParametros(atoi(args[1]), args[2])) {
        int cantHilos = atoi(args[1]);
        char *nombre = args[2];
        int cantPos = obtenerTamanoVectoresDeArchivo(nombre);
        if (cantPos > 0) {
            double vec1[cantPos], vec2[cantPos], vecRes[cantPos];
            if (obtenerVectoresDeArchivo(vec1, vec2, nombre)) {
                exito = sumarVectores(cantHilos, vec1, vec2, vecRes, cantPos);
                if (exito) {
                    grabarVectorResultado(vecRes, cantPos);
                }
            }
        }
    } else {
        mostrarAyuda();
        return 0;
    }
    return 1;
}

int validarParametros(int cantHilos, char *nombre) {
    //args[0] nombre de script
    //args[1] cantidad de hilos
    //args[2] archivo

    if (cantHilos < 1) {
        printf("\nEl numero de threads debe ser mayor a cero");
        return 0;
    }

    struct stat myFile;
    if (stat(nombre, &myFile) < 0) {
        printf("\nNo se encontro el archivo %s\n", nombre);
        return 0;
    }

    int cantidadDePosiciones = obtenerTamanoVectoresDeArchivo(nombre);

    if (cantHilos > cantidadDePosiciones) {
        printf("\nOperación invalida: Tengo más threads que posiciones en el vector: threads: %d, posiciones: %d\n",
               (int) cantHilos, cantidadDePosiciones);
        return 0;
    }

    return 1;
}

void mostrarAyuda() {
    printf("\n Ejemplo de ejecucion:\n");
    printf("\t ./ejercicio2  4 ./archivo.txt \n");
    return;
}

int obtenerTamanoVectoresDeArchivo(char *nombreArchivo) {
    int i = 0;
    char linea[20] = "";
    FILE *archivo = fopen(nombreArchivo, "rt");
    {
        if (archivo) {
            while (fgets(linea, 20, archivo)) {
                i++;
            }
        }
    }
    return i;
}

int obtenerVectoresDeArchivo(double *vec1, double *vec2, char *nombre) {
    int rta = 0;
    char linea[20] = "";
    FILE *archivo = fopen(nombre, "rt");
    if (archivo) {
        while (fgets(linea, 20, archivo)) {
            sscanf(linea, "%lf, %lf", vec1++, vec2++);
            rta++;
        }
    }
    fclose(archivo);
    return rta;
}

void grabarVectorResultado(double *vecRes, int cantPos) {
    FILE *arch = fopen("resultados.txt", "wt");

    if (arch) {
        while (cantPos--) {
            char string[20] = "";
            sprintf(string, "%.4lf\n", *vecRes);
            fputs(string, arch);
            vecRes++;
        }
    }

    fclose(arch);
}

int mayor(int a, int b) {
    return a > b ? a : b;
}

void *sumarVec(void *data) {
    struct info *dato = data;
    int desde = dato->desde;
    int hasta = dato->hasta;
    for (; desde < hasta; desde++) {
        dato->vecRes[desde] = dato->vec1[desde] + dato->vec2[desde];
    }
    return 0;
}

//void crearHiloSuma(int desde, int hasta, double *vec1, double *vec2, double *vecRes) {
//    pthread_t hilo;
//    struct info *data;
//    data->cantSumas = cantSumas;
//    data->vec1 = vec1;
//    data->vec2 = vec2;
//    data->vecRes = vecRes;
//    pthread_create(&hilo, NULL, *sumarVec, data);
//    pthread_join(hilo, NULL);
//}

int sumarVectores(int cantHilos, double *vec1, double *vec2, double *vecRes, int cantSumas) {
    int sumasPorHilo[cantHilos];
    int sumasInicialesPorHilo = cantSumas / cantHilos;
    pthread_t hilos[cantHilos];

    for (int i = 0; i < cantHilos; i++) {
        sumasPorHilo[i] = sumasInicialesPorHilo;
    }

    int sumasRestantes = cantSumas % cantHilos;
    for (int i = 0; i < sumasRestantes; i++) {
        sumasPorHilo[i]++;
    }

    int desde = 0;
    int hasta;
    struct info data[cantHilos];
    for (int nroHilo = 0; nroHilo < cantHilos; nroHilo++) {
        printf("Thread %d: %d sumas\n", nroHilo, sumasPorHilo[nroHilo]);
        hasta = desde + sumasPorHilo[nroHilo];

        data[nroHilo].desde = desde;
        data[nroHilo].hasta = hasta;
        data[nroHilo].vec1 = vec1;
        data[nroHilo].vec2 = vec2;
        data[nroHilo].vecRes = vecRes;
        pthread_create(&hilos[nroHilo], NULL, *sumarVec, &data[nroHilo]);

        desde = hasta;
    }

    for (int nroHilo = 0; nroHilo < cantHilos; nroHilo++) {
        pthread_join(hilos[nroHilo], NULL);
    }

    return 1;
}

void mostrarVector(double vec[], int c) {
    for (int i = 0; i < c; i++) {
        printf("%f", vec[i]);
    }
}

