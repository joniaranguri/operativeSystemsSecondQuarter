
# Trabajo práctico N2 Ejercicio 2 (Primera entrega)
# Script: ejercicio2.sh
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284
###########################AYUDA################################
<#
    .SYNOPSIS

        Informa cuales de los procesos que se encuentran corriendo en el sistema
        tiene mas de una determinada cantidad de instancias.

    .DESCRIPTION

        Informa cuales de los procesos que se encuentran corriendo en el sistema tiene mas de una 
        determinada cantidad de instancias. Recibe un parametro llamado Cantidad, en el cual se indica 
        la cantidad minima de instancias que debe tener un proceso para ser reportado. La salida es un
        listado de los nombres de los procesos que tienen mas de -Cantidad instancias, sin encabezados
        ni otro texto adicional.
    
    .EXAMPLE
    
       <DIRECTORIO_SCRIPT>\ejercicio2.ps1 -Cantidad 5

    .INPUTS
    
        Parametros Obligatorios ["Cantidad"]

     .OUTPUTS

        La salida es una lista de los nombres de aquellos procesos que tienen mas instancias que la cantidad ingresada por parametro

#>

################MAIN####################
Param (
 [Parameter(Mandatory = $true)]
 [ValidateRange(1, [int]::MaxValue)]
 [int] $Cantidad
)

$hash = @{}   
$listaproceso = Get-Process | Select-Object Name
foreach ($item in $listaproceso)
{
    if ($hash.ContainsKey($item.Name)) 
    {
        $hash[$item.Name]++;  
    }
    else
    {
        $hash[$item.Name] = 1;  
    }
}
foreach ($item in $hash.Keys)
{
    if ($hash[$item] -gt $Cantidad) 
    {
        $item
    }
}
