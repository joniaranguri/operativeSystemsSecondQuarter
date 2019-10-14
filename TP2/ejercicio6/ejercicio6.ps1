﻿
<#
    .SYNOPSIS

    Dado un archivo de entrada correspondiente a una matriz se realiza una de las siguientes acciones:
	- Producto escalar.
	- Sumarle a la matriz dada , otra matriz pasada por parámetro.
    Y se guarda el resultado en un archivo de salida.

    .DESCRIPTION
    Se realiza una suma de de matrices n x n o un producto escalar de la misma segun corresponda y se guarda el resultado en un archivo

    .PARAMETER Entrada
    Indica el path del archivo de entrada, el cual contiene la matriz sobre la cual se va a realizar los procesos.

    .PARAMETER Producto 
    Indica el numero con el que se realizara producto escalar a la matriz que viene en la entrada.

    .PARAMETER Suma
    Indica el path del archivo que contiene la matriz que se quiere sumar

    .EXAMPLE
        .\ejercicio6.ps1 -Entrada unaMatriz -Producto 4
	    .\ejercicio6.ps1 -Entrada unaMatriz -Suma otraMatriz
        
#>

[CmdletBinding()]
Param(
    [Parameter(ParameterSetName='Producto')][Int]$Producto,
     [ValidateScript({$_ | Test-Path})]
        [Parameter(ParameterSetName='Suma')][String]$Suma,

    [ValidateScript({$_ | Test-Path})]
        [string]$Entrada

)

function do_add($a , $b) {
$result = ""
 for($i=0;$i -lt $a.count;$i++) {
$countA = $a[$i].count
$countB = $b[$i].count

if($countA -ne $countB ){
Write-Error "Ha ocurrido un error al sumar las matrices. Verifique que el formato de las mismas sea correcto"
[Environment]::Exit(1)}


for( $j=0;$j -lt $a[$i].count;$j++){
   $suma= $a[$i][$j]+ $b[$i][$j]
  if($j -ne 0){
   $result+= '|'
   }
   $result+=$suma
  }
  $result += "`r`n"
    }

    	
    $result | Out-File -FilePath  .\'Salida.'$Entrada
  

    }

function  scalar_product($a , $scalar){
   $result= 0
    Foreach($i in $a){
    Foreach($val in $i){
       #  Write-Host $val
        $result+= $val*$scalar
         }
   
    }

    $result  | Out-File -FilePath .\'Salida.'$Entrada
}



##leer archivo en una matriz

$matFromFile = @()
foreach($line in Get-Content $Entrada) {

      [double[]] $fila = @($line.split('|'))
     $matFromFile += , $fila
}

if($Suma){
$matToAdd = @()
foreach($line in Get-Content $Suma) {

      [double[]] $fila = @($line.split('|'))
     $matToAdd += , $fila
}
do_add $matFromFile $matToAdd
}else{
scalar_product $matFromFile $Producto
}
Write-Host "El proceso se realizó correctamente"