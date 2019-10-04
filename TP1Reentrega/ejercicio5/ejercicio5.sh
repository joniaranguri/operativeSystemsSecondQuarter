#!/bin/bash

# Trabajo práctico N1 Ejercicio 5 (Primera entrega)
# Script: ejercicio5.sh
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284
pathActual=$(pwd)
cd ~
usuario=$(pwd)
pathPapelera="$usuario/.papelera de reciclaje"
pathListaEliminados="$usuario/.lista de eliminados.txt"
cd $pathActual
function ayuda() {
  echo "Ejemplos de ejecucion:"
  echo "  -Para eliminar un archivo: ./ejercicio5.sh archivo.txt"
  echo "  -Para listar los archivos que se encuentran en la papelera: ./ejercicio5.sh -l"
  echo "  -Para vaciar la papelera: ./ejercucion5.sh -e"
  echo "  -Para restaurar un archivo: ./ejercicio5.sh -r archivo.txt"
}
function listarArchivos() {
  ls -l "$pathPapelera"
}
function agregarLista() {
  echo "$1" >>"$pathListaEliminados" #agrego el archivo a la lista de eliminados
}
function moverArch() {
  #$1 desde $2 #hacia

  pathDestino=$(dirname "$2")
  nombreArchivo=$(basename "$2")
  extension=$([[ "$nombreArchivo" == *.* ]] && echo "${nombreArchivo##*.}")
  nameSinExt="${nombreArchivo%%.*}"

  if [ -f "$2" ]; then # me fijo si existe un archivo con ese nombre a donde quiero mover el primero
    #tengo que cambiar el nombre
    if [ "$entension" != " " ]; then
    echo  "$pathDestino/$nameSinExt(1).$extension"
      moverArch "$1" "$pathDestino/$nameSinExt(1).$extension"
    else
      moverArch "$1" "$pathDestino/$nameSinExt(1)"
    fi
  else
    mv "$1" "$2"
    nombreFinal=$(basename "$2")

  fi

}

function validarParametros() {
  #$3 cantidad de parametros
  #si $3 es 1 , $1 pueder ser "-e" "-l" "pathDeArchivo"
  #sino $3 puede ser 2 entonces, $1 pueder ser "-r"
  #sino error de cantidad de parametros
  if [ $3 -ne 1 -a $3 -ne 2 ]; then
    echo "parametros incorrectos.Puede consultar la ayuda de las siguientes maneres:"
    echo "  ./ejercicio5.sh -?"
    echo "  ./ejercicio5.sh -h"
    echo "  ./ejercicio5.sh -help"
    exit

  elif [ $3 -eq 1 ]; then
    if [ "$1" != "-e" -a "$1" != "-l" -a ! -f "$1" -a "$1" != "-h" -a "$1" != "-?" -a "$1" != "-help" ]; then
      echo "parametros incorrectos.Puede consultar la ayuda de las siguientes maneres:"
      echo "  ./ejercicio5.sh -?"
      echo "  ./ejercicio5.sh -h"
      echo "  ./ejercicio5.sh -help"
      exit
    fi
  elif [ $3 -eq 2 -a "$1" != "-r" ]; then
    echo "parametros incorrectos.Puede consultar la ayuda de las siguientes maneres:"
    echo "  ./ejercicio5.sh -?"
    echo "  ./ejercicio5.sh -h"
    echo "  ./ejercicio5.sh -help"
    exit

  fi

}
function eliminarArchivo() {

  pathCompleto=$(readlink -f "$1")                                #obtengo el path completo del archivo
  path=$(dirname "$pathCompleto")                                 #me quedo con el path
  nombreArchivo=$(basename "$pathCompleto")                       #me quedo con el nombre del archivo
  moverArch "$path/$nombreArchivo" "$pathPapelera/$nombreArchivo" #muevo el archivo a la papelera
  agregarLista "$path/$nombreFinal"                               #agrego el path del archivo a la lista de eliminados

}
function ActualizarArchivo() {
  terminacion="/$1"
  salida=$(grep -v "$terminacion" "$pathListaEliminados") #me quedo con todos menos el archivo que quiero sacar de la lista
  echo "$salida" >"$pathListaEliminados"
}
function recuperarArchivo() {
  #$1 es el archivo a recuperar
  nameArch=$(basename "$1")
  if [ ! -f "$pathPapelera/$nameArch" ]; then
    echo "no se escontro el archivo $nameArch en la papelera"
    exit
  fi
  pathCompleto=$(grep "/$nameArch" "$pathListaEliminados")
  path=$(dirname "$pathCompleto") #me quedo con el path sin el nombre del archivo
  ActualizarArchivo "$nameArch"   #elimino el archivo de la lista de eliminados
  moverArch "$pathPapelera/$nameArch" "$pathCompleto"
}
function vaciarPapelera() {
  rm -f "$pathPapelera/"*
  rm "$pathListaEliminados"
}

################################# MAIN #######################################

#verifico que exista la papelera de reciclaje
if [ ! -d "$pathPapelera" ]; then

  echo "directorio de papelera : $pathPapelera"
  mkdir "$pathPapelera"
fi
#verifico que existe la lista de eliminados
if [ ! -f "$pathListaEliminados" ]; then
  touch "$pathListaEliminados"
fi
validarParametros "$1" "$2" $#

if [ $# -eq 1 -a "$1" == "-e" ]; then
  vaciarPapelera
  exit
elif [ $# -eq 1 -a "$1" == "-l" ]; then
  listarArchivos
  exit
elif [ $# -eq 1 -a "$1" == "-?" ]; then
  ayuda
  exit
elif [ $# -eq 1 -a "$1" == "-h" ]; then
  ayuda
  exit
elif [ $# -eq 1 -a "$1" == "-help" ]; then
  ayuda
  exit
elif [ $# -eq 2 ]; then
  recuperarArchivo "$2"
  exit
else
  eliminarArchivo "$1"
  exit
fi

################################## FIN ####################################
