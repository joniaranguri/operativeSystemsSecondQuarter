
# Trabajo práctico N2 Ejercicio 5 (Primera entrega)
# Script: ejercicio5.sh
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284
################MAIN####################

   
       
<#
    .SYNOPSIS
    
        Este script obtiene cantidad de procesos en ejecución o el tamaño de un directorio indicado. 
    
    .DESCRIPTION
    
        Este script se encarga de obtener cada cierta cantidad de tiempo la cantidad de procesos o el tamaño de un directorio segun los parametros ingresados.
    
    .EXAMPLE
    
        Opcion 1) <DIRECTORIO_SCRIPT>\ejercicio5.ps1 -Procesos
        Opcion 2) <DIRECTORIO_SCRIPT>\ejercicio5.ps1 -Peso -Directorio [Path]

    .INPUTS
    
        Parametros Obligatorios ["Procesos" o "Peso"] ( si la opción es "Peso" el parametro "Directorio" es obligatorio) 

     .OUTPUTS
        La salida es numerica que define la cantidad de procesos ejecutandose en ese momento en caso de ingresar como parametro -Procesos o
        el tamaño de un directorio ingresado en caso de ingresar como parametro -Peso.
#>

Param(
    [Parameter( ParameterSetName ="Proceso" )]
    [switch] $Procesos,
    [Parameter(ParameterSetName ="Peso")]
    [switch] $Peso,
    [Parameter(Mandatory=$true ,ParameterSetName ="Peso")]
    [String] $Directorio)
    


function Validar-Directorio()
{
    Param ( [string] $directorio )
    $Valid = Test-Path -Path $directorio;
    return $Valid
    
}
function Mostrar-MensajeErrorDirectorio()
{
    Write-Output "Error de Validación: El directorio ingresado no existe"
}

################################################ main ################################################


$timer = New-Object  -type System.Timers.Timer
$timer.Interval = 10000
$timer.AutoReset = $true

if ($PSBoundParameters.ContainsKey('Peso')) {
    if(!(Validar-Directorio $Directorio)){
        Mostrar-MensajeErrorDirectorio
        return
    }
}

Register-ObjectEvent -InputObject $timer -MessageData $Directorio  -EventName Elapsed -SourceIdentifier eventoEjercicio5 -Action {
    
    $directorio = $event.MessageData
    
    if($directorio){ # si es Opcion -Peso
        $salida=(Get-ChildItem -path "$Directorio" -File -recurse | Measure-Object -Property Length -sum).Sum
        
    
    }else{ # si es Opcion -Procesos 
        $salida=(Get-Process).count
    }
    Write-Host $salida
    
}

$timer.Enabled = $True