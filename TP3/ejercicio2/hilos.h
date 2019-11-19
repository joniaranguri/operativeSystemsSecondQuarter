#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define NOMBRE_ARCHIVO "vectores.txt"
#define EXISTE_ARCHIVO 1
#define NO_EXISTE_ARCHIVO 0
#define PARAMETROS_OK 1
#define PARAMETROS_ERROR 0

int validarParametros(int cantHilos, char * nombreArchivo);
int validarCantidadHilos(int cantHilos);
int validarExistenciaArchivo(char * nombreArchivo);
void crearHiloSuma(int sumasPorHilo, double * vec1, double * vec2, double * vecRes);
int obtenerTamanoVectoresDeArchivo(char * nombreArchivo);
int obtenerVectoresDeArchivo(double * vec1, double * vec2, char * nombre);
void mostrarVector(double vec[], int c);
void mostrarAyuda();
