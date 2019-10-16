
# Trabajo práctico N2 Ejercicio 1 (Primera entrega)
# Script: ejercicio1.sh
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284
################MAIN####################
Param (
    [Parameter(Position = 1, Mandatory = $false)]
    [String] $pathsalida = ".\procesos.txt",
    [int] $cantidad = 3
    )
    $existe = Test-Path $pathsalida
    if ($existe -eq $true) 
{
 $listaproceso = Get-Process
 foreach ($proceso in $listaproceso) 
 {
  $proceso | Format-List -Property Id,Name >> $pathsalida
 }
 for ($i = 0; $i -lt $cantidad ; $i++) 
 {
  Write-Host $listaproceso[$i].Name - $listaproceso[$i].Id
 }
} 
else 
{
 Write-Host "El path no existe"
}

<#

Responder:
1. ¿Cuál es el objetivo del script?
2. ¿Agregaría alguna otra validación a los parámetros?
3. ¿Qué sucede si se ejecuta el script sin ningún parámetro?

1. Lista todos los procesos que se estan ejecutando, los imprime en un archivo y muestra por pantalla una determinada cantidad de ellos.
2. Validaria que $pathsalida no sea un string vacio.
3. Si se intenta ejecutar el script sin pasarle ningun parametro, indica que el path no existe.

#>