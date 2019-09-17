#!/bin/bash
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
# Si la cantidad de parametros es menor a 2, muestro el error S
if test $# -lt 2; then
 ErrorS
 exit
fi
# Si el parametro 1 no es legible, muestro el error P
if ! test $1 -r; then
 ErrorP
 exit
# Si es legible y el parametro 2 es una de esas letras, realizo las acciones
elif test -f $1 && (test $2 = "L" || test $2 = "C" || test $2 = "M"); then
 if test $2 = "L"; then
 res=`wc –l $1`
 echo "La cantidad de lineas del archivo es: $res"
 exit
 elif test $2 = "C"; then
 res=`wc –m $1`
 echo "La cantidad de caracteres del archivo es: $res"
 exit
 elif test $2 = "M"; then
 res=`wc –L $1`
 echo "La longitud de la linea mas larga del archivo es: $res"
 exit
fi
else
# Si es legible pero el parametro 2 no es valido, muestro el error S
 ErrorS
 exit
fi