#include "hilos.h"

int main(int cantHilos, char * args[])
{
    int exito = 0;

    if (validarParametros(cantHilos, args[0]))
    {
        int cantPos = 0;

        if (cantPos = obtenerTamañoVectoresDeArchivo(args[0]) != 0)
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
    }

}
