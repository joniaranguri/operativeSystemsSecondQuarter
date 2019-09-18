#!/bin/bash

# Trabajo práctico N1 Ejercicio 3 (Primera entrega)
# Script: procesoDemonio.sh
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284


directorioAHacerBackup=$1
directorioDondeHacerBackup=$2
cantidadDeBackups=0
nroDeBackup=0
# saque el "echo "">arch " y agregue esta linea para eliminar el archivo si existe
if [ -f ".arch" ]; then
    rm '.arch'
fi

function backUp() {
    destino=$(echo "$directorioDondeHacerBackup/backup-$nroDeBackup")

    while [ -d "$destino" ]; do
        ((nroDeBackup++))
        destino="$directorioDondeHacerBackup/backup-$nroDeBackup"
    done

    cp -r "$directorioAHacerBackup" "$destino"
    echo "$destino" >>'.arch'
    ((cantidadDeBackups++))
    ((nroDeBackup++))
}

function count() {
    ls -1 "$directorioDondeHacerBackup" | wc -l

}

function clear() {
    cantidadDeArchivosAMantener=$(cat '.clear' | tr -d " ")
    rm '.clear'
    nroArchivosABorrar=$(($cantidadDeBackups - $cantidadDeArchivosAMantener))
    echo "numero a mantener: $cantidadDeArchivosAMantener"
    echo "cantidad de backup $cantidadDeBackups"
    cantidadDeBackups=$cantidadDeArchivosAMantener
    echo "cant archivos a borrar :$nroArchivosABorrar"
    if [ "$cantidadDeArchivosAMantener" -eq 0 ]; then
        #echo "" > '.arch'
        touch ".arch"
    fi
    if [ $nroArchivosABorrar -gt 0 ]; then
        archivos=$(head -$nroArchivosABorrar <".arch")

        for i in $archivos; do
            echo $i
            cat ".arch" | grep -v "$i" >".arch2"
            cat ".arch2" >".arch"
            rm -rf "$i"
        done
        rm ".arch2"
    fi

}
################MAIN####################

trap count SIGUSR1
trap backup SIGUSR2
trap clear SIGTERM

while true; do
    backUp
    sleep "$3" &
    wait
done
###################FIN#################