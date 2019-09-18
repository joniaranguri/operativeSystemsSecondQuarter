#!/bin/bash

#funciones

function ayuda()
{
    echo "Script que realiza la contabilidad de lineas de còdigo de archivos dentro de un directorio indicado como parametro"
    echo "[Parametro 1 ]: Directorio utilizado para contabilizar lineas de còdigo"
    echo "[Parametro 2 ]: extensiòn de archivos buscados dentro de directorio"
}

#directorios funcionan con espacios
IFS='
'

 # valida si se envia como parametro 1 para ayuda.
if test $# -eq 1
then
    if test $1 = "-h" || test $1 = "-?" || test $1 = "-help"
        then ayuda
    else 
        echo "error de parametros, consulte la ayuda"
        exit 
    fi
fi

# Validaciones

if [ $# != 2 ]
then
    echo "La cantidad de parametros ingresados no es la correcta"
    echo "Para mas informaciòn ingrese como parametro -h"
    exit
fi

if ! [ -d $1 ]
then 
 echo "El directorio ingresado no es correcto"
fi

# MAIN #
cantidadArchivosAnalizados=0
cantidadDeLineasTotales=0
cantidadDeLineasComentadasTotales=0
contadorDeArchivos=0
cantidadLineasDeCodigo=0;
cantidadLineasDeCodigoTotales=0;
porcentajeLineasDeCodigoTotales=0;
porcentajeLineasComentadasTotales=0;


listaDeArchivos=$( find -type f -name "*.$2" )

#echo $listaDeArchivo

for archivo in $listaDeArchivos
{
    resultadoArchivo=$( awk -f 'ContadorDeLineasDeComentario.awk' $archivo )

    arrayResultado=($resultadoArchivo)
    cantidadDeLineas="${arrayResultado[0]}"
    cantidadDeLineasComentadas="${arrayResultado[1]}"
    cantidadLineasDeCodigo="${arrayResultado[2]}"
     
    echo "--------------------------------------"
    echo "Archivo Analizado:"$archivo
    echo "--------------------------------------"
    echo "cantidadDeLineas:" $cantidadDeLineas
    echo "cantidadLineasDeCodigo="$cantidadLineasDeCodigo
    echo "cantidadDeLineasComentadas: "$cantidadDeLineasComentadas 

    #calculo de totales
    contadorDeArchivos=$((contadorDeArchivos+1))
    cantidadDeLineasTotales=$(expr $cantidadDeLineasTotales + $cantidadDeLineas)
    cantidadDeLineasComentadasTotales=$(expr $cantidadDeLineasComentadasTotales + $cantidadDeLineasComentadas)
    cantidadLineasDeCodigoTotales=$(expr $cantidadLineasDeCodigoTotales + $cantidadLineasDeCodigo)
    

}
#calculo de porcentaje
porcentajeLineasDeCodigoTotales=$((100*cantidadLineasDeCodigoTotales/cantidadDeLineasTotales))
porcentajeLineasComentadasTotales=$((100*cantidadDeLineasComentadasTotales/cantidadDeLineasTotales))

#mostrar en pantalla.
echo "=== TOTALES ======================================================"
echo "cantidad de archivos totales analizados:"  $contadorDeArchivos

echo "cantidad de lineas totales:"  $cantidadDeLineasTotales
echo "cantidad de lineas còdigo totales:"  $cantidadLineasDeCodigoTotales
echo "cantidad de lineas comentadas totales:"  $cantidadDeLineasComentadasTotales
echo "% Lineas de codigo: " $porcentajeLineasDeCodigoTotales "%"
echo "% Lineas de comentadas: "$porcentajeLineasComentadasTotales "%"
