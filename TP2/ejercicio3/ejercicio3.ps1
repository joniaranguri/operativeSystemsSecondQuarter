

# Trabajo práctico N2 Ejercicio 3 (Primera entrega)
# Script: ejercicio3.sh
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284
###########################AYUDA################################


<# 
    .SYNOPSIS
        Mueve archivos de un directorio a otro
    .DESCRIPTION
        Dado un archivo CSV mueve archivos del directorio de origen al directorio de destino 
        y escriben en un log la fecha en la que se hizo el movimiento.
        Recibe como parametro la ruta del archivo CSV a procesar.
#>
#########################################MAIN############################################
Param (
 [Parameter(Mandatory = $true)]
 [ValidatePattern(".*.csv")] # el parametro tiene que ser un archivo
 [ValidateScript({
            if(!(Test-Path $_)) {
                throw "El archivo csv de entrada no existe"
            }

            return $true
        })]
 [string] $entrada,
 [Parameter(Mandatory = $true)]
 [ValidatePattern(".*.csv")] # el parametro tiene que ser un archivo
 [string] $salida,
 [string] $help
)

if ($help = 'Get-Help') {
    Exit-PSHostProcess
}

$csvEntrada = Import-CSV -Delimiter "," -Path $entrada

foreach ($fila in $csvEntrada) {
    if ((Test-Path $fila.origen)) {
        Move-Item -Path $fila.origen -Destination $fila.destino
        [PSCustomObject]@{archivo = $fila.destino; fecha = (Get-Date).ToString('MM-dd-yyyy')} | Export-Csv -Path $salida -Append
    } else {
        Add-Content "errores.txt" ("El archivo {0} no fue encontrado por lo tanto no se movió" -f $fila.origen)
    }
}

