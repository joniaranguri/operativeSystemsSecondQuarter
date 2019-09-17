#!/bin/bash
IFS=$'\n'

function ayuda() {
    echo "Ejemplos de ejecucion:"
    echo "./ejercicio3.sh start <directorio a realizar backups> <directorio donde guardar backups> <intervalo de tiempo> //Inicia el proceso de backups"
    echo "./ejercicio3.sh stop //Finaliza el proceso de backups"
    echo "./ejercicio3.sh count //Indica la cantidad de archivos de backup en el directorio"
    echo "./ejercicio3.sh clear <cantidad de backups a mantener> //Limpia los backups manteniendo los ultimos N según el parametro. Sin parametro se limpia entero"
    echo "./ejercicio3.sh play //Crea un backup a demanda"
}

function ejemploMostrarAyudas() {
    echo "Puede ver la ayuda con las siguientes opciones:"
    echo "./ejercicio2.sh -?"
    echo "./ejercicio2.sh -h"
    echo "./ejercicio2.sh -help"
}
function validarParametros() {
    if [[ $1 != "start" && $1 != "stop" && $1 != "count" && $1 != "clear" && $1 != "play" && $1 != "-?" && $1 != "-h" && $1 != "-help" ]]; then
        echo "Accion invalida, consulte la ayuda"
        ejemploMostrarAyudas
        exit
    fi
    if [[ "$1" == 'start' && "$5" -ne 4 ]] ||
        [[ "$1" == 'stop' && "$5" -ne 1 ]] ||
        [[ "$1" == 'count' && "$5" -ne 1 ]] ||
        [[ "$1" == 'clear' && "$5" -gt 2 ]] ||
        [[ "$1" == 'play' && "$5" -ne 1 ]]; then #Perdon por la condición gigante
        echo "Error en la cantidad de parametros, consulte la ayuda"
        ejemploMostrarAyudas
        exit
    fi
}

validarParametros "$1" "$2" "$3" "$4" $#
if [[ $1 == "-h" || $1 == "-?" || $1 == "-help" ]]; then
    ayuda
    exit
fi

if [[ $1 == "start" ]]; then
    PID=$(ps -e | grep "./procesoDemonio.sh" | grep -v "grep" | cut -d " " -f1)
    if [[ -z $PID ]]; then
        ./procesoDemonio.sh "$2" "$3" "$4" &
        exit
    else
        echo "Ya existe un proceso en curso"
        exit
    fi
fi

if [[ $1 == "count" ]]; then
 pd=$(ps u | awk '$12== "./procesoDemonio.sh" {print $2}')
    if [ "$pd" == "" ]
    then
        exit
    else
        kill -SIGUSR1 $pd
        exit
    fi
   
fi

if [[ $1 == "stop" ]]; then
 pd=$(ps u | awk '$12== "./procesoDemonio.sh" {print $2}')
    if [ "$pd" == "" ]
    then
        exit
    else
        kill -9 $pd
        exit
    fi
   
fi

if [[ $1 == "clear" ]]; then
    aBorrar=0
    if [[ $# -eq 2 ]]; then
        aBorrar=$2
    fi
    echo "$aBorrar" >'.clear'
     pd=$(ps u | awk '$12== "./procesoDemonio.sh" {print $2}')
    if [ "$pd" == "" ]
    then
        exit
    else
        kill -SIGTERM $pd
        exit
    fi
fi

if [[ $1 == "play" ]]; then
 pd=$(ps u | awk '$12== "./procesoDemonio.sh" {print $2}')
    if [ "$pd" == "" ]
    then
        exit
    else
        kill -SIGUSR2 $pd
        exit
    fi
   fi
