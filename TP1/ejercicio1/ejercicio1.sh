#!/bin/bash
# Trabajo práctico N1 Ejercicio 1 (Primera entrega)
# Script: ejercicio1.sh
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284

# Declaracion de funciones de errores
ErrorS()
{
 echo "Error. La sintaxis del script es la siguiente:"
 echo "Cantidad de lineas del archivo: $0 nombre_archivo L"
 echo "Cantidad de caracteres del archivo: $0 nombre_archivo C"
 echo "Longitud de la linea mas larga del archivo: $0 nombre_archivo M"
}
ErrorP()
{
 echo "Error. $1 no es un archivo legible"
}
################MAIN####################
# Si la cantidad de parametros es menor a 2, muestro el error S
if test $# -lt 2; then
 ErrorS
 exit
fi
# Si el parametro 1 no es legible, muestro el error P
if ! test -r $1; then
 ErrorP
 exit
# Si es legible y el parametro 2 es una de esas letras, realizo las acciones
elif test -f $1 && (test $2 = "L" || test $2 = "C" || test $2 = "M"); then
     if test $2 = "L"; then
         res=`wc $1 -l`
         echo "La cantidad de lineas del archivo es: $res"
     elif test $2 = "C"; then
         res=`wc $1 -m`
         echo "La cantidad de caracteres del archivo es: $res"
     elif test $2 = "M"; then
         res=`wc $1 -L`
         echo "La longitud de la linea mas larga del archivo es: $res"
    fi
else
# Si es legible pero el parametro 2 no es valido, muestro el error S
     ErrorS
     exit
fi

# Responda:
# a) ¿Cuál es el objetivo de este script?
# b) ¿Qué parámetros recibe?
# c) Comentar el código según la funcionalidad (no describa los
# comandos, indique la lógica)
# d) Completar los “echo” con el mensaje correspondiente.
# e) ¿Qué información brinda la variable “$#”? ¿Qué otras variables
# similares conocen?
# Explíquelas.
# f) Explique las diferencias entre los distintos tipos de comillas que
# se pueden utilizar en Shell
# scripts.

# a) El objetivo de este script es procesar un archivo de texto y
# obtener informacion acerca de 3 aspectos del mismo
# b) Recibe 2 parámetros. El nombre del archivo a aplicar el script y
# un segundo parametro para elegir que accion se realizará.
# e) $# indica la cantidad de parametros que recibe el script. Otras
# variables de ese estilo son:
#    $0: nombre de nuestro script
#    $n: cada parametro
#    $$: PID de nuestro proceso
#    $*: todos los parámetros menos $0
# f) En shell se pueden utilizar dos tipos de comillas: simples y dobles.
# Su comportamiento variará depende el uso que se le quiera dar.
# Las comillas dobles deben usarse cuando se deseen utilizar variables
# o expandir comandos dentro de una cadena (con el caracter `).
# Las comillas simples, en cambio, deben usarse cuando se quiere que
# la cadena encerrada se tome como un texto literal.
