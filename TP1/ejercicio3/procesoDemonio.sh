#!/bin/bash

directorioAHacerBackup=$1
directorioDondeHacerBackup=$2
cantidadDeBackups=0
nroDeBackup=0
rm '.arch'
echo "" > '.arch'

function backUp() {
    destino="$directorioDondeHacerBackup/backup-$nroDeBackup"
    
    while [ -d "$destino" ]; do
        (( nroDeBackup++ ))
        destino="$directorioDondeHacerBackup/backup-$nroDeBackup"
    done
    
    cp -r "$directorioAHacerBackup" "$destino"
    echo "$destino" >> '.arch'
    (( cantidadDeBackups++ ))
    (( nroDeBackup++ ))
}

function count() {
    echo ls -1 "$directorioDondeHacerBackup" | wc -l
}

function clear() {
    cantidadDeArchivosAMantener=$(cat '.clear' | tr -d " ")
    rm '.clear'
    nroArchivosABorrar=$(( $cantidadDeBackups - $cantidadDeArchivosAMantener ))
    cantidadDeBackups=$cantidadDeArchivosAMantener
    
    if [ $nroArchivosABorrar -ge 0 ]; then
        archivos=$(head -$nroArchivosABorrar < ".arch")
        
        for i in $archivos
        do
            echo $i
            rm -rf "$i"
        done
        
        echo "" > '.arch'
        sed "1,$nroArchivosABorrar""d" >> ".arch"
    fi
    
    if [ "$cantidadDeArchivosAMantener" -eq 0 ]; then
        echo "" > '.arch'
    fi
}

trap count SIGUSR1
trap backup SIGUSR2
trap clear SIGTERM

while true
do
    backUp
    sleep "$3" &
    wait
done
