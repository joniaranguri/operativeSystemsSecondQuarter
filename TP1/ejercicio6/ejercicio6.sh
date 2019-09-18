#!/bin/bash

# Trabajo práctico N1 Ejercicio 6 (Primera entrega)
# Script: ejercicio6.sh
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284

IFS='
'
function mustSeeHelpAndExit(){
      echo "Puede ver la ayuda con las siguientes opciones:"
      echo " ./ejercicio6.sh -?"
      echo " ./ejercicio6.sh -h"
      echo " ./ejercicio6.sh -help"
      exit
 }
function help() {
  echo "Este script permite listar los 10 Subdirectorios hoja de mayor peso, dado un directorio base"
  echo "
        Ejemplos de ejecucion:
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
################MAIN####################
 if [ $# -eq 0 ]; then
    echo "No se puede ejecutar el script sin parametros, consulte la ayuda"
    mustSeeHelpAndExit
fi
if [ "$1" == "-h" -o "$1" == "-help" -o "$1" == "-?" ]; then
  help
    exit
  fi
 validateParameters "$1" $#
  direc="$1"

  declare -a arrDirectories

 for SUBDIR in $(find "$direc" -type d   -links 2 -exec du -Sh {} + | sort -rh | head -n 10 | cut -f2); do

if [ ${#arrDirectories[@]} -lt 10 ]; then
    arrDirectories=(${arrDirectories[*]} "$SUBDIR")
fi
  done

echo -e '\n----------------------------------- Top 10 Subdirectories on path passed by parameter -----------------------------------'
for item in "${arrDirectories[@]}"
do
  countFiles=$(ls $item -1 | grep -v ^l | wc -l)
   size=$(du -h $item | cut -f1)
    echo "$item $size $countFiles arch."
done
########################FIN#################