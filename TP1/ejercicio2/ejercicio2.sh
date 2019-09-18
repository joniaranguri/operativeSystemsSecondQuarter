#!/bin/bash

# Trabajo práctico N1 Ejercicio 2 (Primera entrega)
# Script: ejercicio2.sh
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284
################MAIN####################

IFS='
'
function ayuda() {
  echo "Ejemplos de ejecucion:"
  echo "  -sin parametros:
                        ./ejercicio2.sh //hace el proceso en el directorio actual"
  echo "  -con un solo parametro: 
                        ./ejercicio2.sh -r //hace el proceso en el directorio actual de forma recursiva "
  echo "                        
                        ./ejercicio2.sh directorio //hace el proceso en el directorio pasado por parametro"
  echo "  -con dos parametros:    
                        ./ejecicio2.sh directorio -r //hace el proceso en el directorio pasado por parametro de forma recursiva"

}
function ValidarParametros() {
  if [ $3 -gt 2 ]; then
    echo "Error en la cantidad de parametros, consulte la ayuda"
    echo "Puede ver la ayuda con las siguientes opciones:"
    echo " ./ejercicio2.sh -?"
    echo "./ejercicio2.sh -h"
    echo "./ejercicio2.sh -help"
    exit
  elif [ $3 -eq 2 ]; then
    if [ ! -d "$1" ]; then
      echo "'$1' no es un directorio,consulte la ayuda"
      echo "Puede ver la ayuda con las siguientes opciones:"
      echo " ./ejercicio2.sh -?"
      echo "./ejercicio2.sh -h"
      echo "./ejercicio2.sh -help"
      exit
    elif [ "$2" != "-r" ]; then
      echo "El segundo parametro debe ser '-r' ,consulte la ayuda"
      echo "Puede ver la ayuda con las siguientes opciones:"
      echo " ./ejercicio2.sh -?"
      echo "./ejercicio2.sh -h"
      echo "./ejercicio2.sh -help"
      exit
    fi

  elif [ $3 -eq 1 ]; then
    if [ ! -d "$1" -a "$1" != "-h" -a "$1" != "-help" -a "$1" != "-?" -a "$1" != "-r" ]; then
      echo "Error ,consulte la ayuda"
      echo "Puede ver la ayuda con las siguientes opciones:"
      echo " ./ejercicio2.sh -?"
      echo "./ejercicio2.sh -h"
      echo "./ejercicio2.sh -help"
      exit
    fi
  fi

}
function cambiarNombre() {
  nameNuevo=$(echo "$1" | tr -s " " | tr " " "_")
  echo "nombre nuevo: $nameNuevo"
  if [ -f "$path/$nameNuevo" ]; then
    extension=$([[ "$nameNuevo" == *.* ]] && echo "${nameNuevo##*.}")
    nameSolo="${nameNuevo%%.*}"
    if [ "$entension" != " " ]; then
      cambiarNombre "$nameSolo(1).$extension"
    else
      cambiarNombre "$nameSolo(1)"
    fi
  else
    if [ "$1" != "$nameNuevo" ]; then
      echo "Se cambio el nombre del  archivo: '$name' por '$nameNuevo'"
    fi
      mv "$path/$name" "$path/$nameNuevo"
  fi
}
#######################MAIN########################
ValidarParametros "$1" "$2" $#
if [ $# -eq 2 ]; then
  direc="$1"
  opcion="$2"
elif [ $# -eq 1 ]; then
  if [ "$1" == "-h" -o "$1" == "-help" -o "$1" == "-?" ]; then
    ayuda
    exit

  elif [ "$1" == "-r" ]; then
    opcion="-r"
    direc=$(pwd)
  else
    direc="$1"
    opcion=" "
  fi
elif [ $# -eq 0 ]; then
  direc=$(pwd)
  opcion=" "
fi
echo "el directorio que recibi:  $direc"
if [ "$opcion" == "-r" ]; then
  for j in $(find "$direc" -type f); do
    path=$(dirname "$j")
    name=$(basename "$j")
    cambiarNombre "$name"

  done
else
  for j in $(find "$direc" -type f); do
    path=$(dirname "$j")
    name=$(basename "$j")
    echo " path encontrado: $path"
    echo "nombre encontrado: $name"
    if [ "$path" == "$direc" -o  "$path/" == "$direc" ]; then
      cambiarNombre "$name"
    fi
  done
fi
##########################FIN#########################