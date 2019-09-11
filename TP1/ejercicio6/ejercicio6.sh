#!/bin/bash
function mustSeeHelpAndExit(){
      echo "Puede ver la ayuda con las siguientes opciones:"
      echo " ./ejercicio6.sh -?"
      echo " ./ejercicio6.sh -h"
      echo " ./ejercicio6.sh -help"
      exit
 }
function help() {
  echo "Ejemplos de ejecucion:
  							 ./ejercicio6.sh directorio //hace el proceso en el directorio pasado por parametro"
 }
function validateParameters() {
  if [ $2 -ne 1 ]; then
    echo "Error en la cantidad de parametros, consulte la ayuda"
    mustSeeHelpAndExit
 fi
if [ $2 -eq 1 ]; then
    if [ ! -d "$1" ]; then
      echo "'$1' no es un directorio,consulte la ayuda"
     mustSeeHelpAndExit
    elif [ ! -d "$1" -a "$1" != "-h" -a "$1" != "-help" -a "$1" != "-?" -a "$1" != "-r" ]; then
      echo "Error ,consulte la ayuda"
      mustSeeHelpAndExit
  	fi
fi

}
 if [ $# -eq 0 ]; then
    echo "No se puede ejecutar el script sin parametros, consulte la ayuda"
    mustSeeHelpAndExit
 fi
 validateParameters"$1" $#
echo "First line"