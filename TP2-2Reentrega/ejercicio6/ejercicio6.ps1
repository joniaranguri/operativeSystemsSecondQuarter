
# Trabajo práctico N2 Ejercicio 6 (Tercera entrega)
# Script: ejercicio6.ps1
# Integrantes:
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Rodriguez Gonzalo Martin    39.461.284
################AYUDA###################



<#
    .SYNOPSIS
    
 	Dado un archivo de entrada correspondiente a una matriz se realiza una de las siguientes acciones:
	- Producto escalar.
	- Sumarle a la matriz dada , otra matriz pasada por parámetro.
   	 Y se guarda el resultado en un archivo de salida.   
	 
    .DESCRIPTION
    
    	Se realiza una suma de de matrices n x n o un producto escalar de la misma segun corresponda y se guarda el resultado en un archivo
    
    .EXAMPLE
    
        Opcion 1)  .\ejercicio6.ps1 -Entrada unaMatriz -Producto 4
        
        Opcion 2)   .\ejercicio6.ps1 -Entrada unaMatriz -Suma otraMatriz
   .PARAMETER Entrada
    	Indica el path del archivo de entrada, el cual contiene la matriz sobre la cual se va a realizar los procesos.
    .PARAMETER Producto 
    	Indica el numero con el que se realizara producto escalar a la matriz que viene en la entrada.
    .PARAMETER Suma
   	 Indica el path del archivo que contiene la matriz que se quiere sumar
#>


[CmdletBinding()]
Param(
    [Parameter(ParameterSetName='Producto')][Int]$Producto,
     [ValidateScript({$_ | Test-Path})]
        [Parameter(ParameterSetName='Suma')][String]$Suma,

    [ValidateScript({$_ | Test-Path})]
        [string]$Entrada

)
#########################################MAIN############################################
function do_add($a , $b) {
$result = ""
 for($i=0;$i -lt $a.count;$i++) {
$countA = $a[$i].count
$countB = $b[$i].count

if($countA -ne $countB ){
Write-Error "Ha ocurrido un error al sumar las matrices. Verifique que el formato de las mismas sea correcto"
return 1
}


for( $j=0;$j -lt $a[$i].count;$j++){
   $suma= $a[$i][$j]+ $b[$i][$j]
  if($j -ne 0){
   $result+= '|'
   }
   $result+=$suma
  }
  $result += "`r`n"
    }
    $Entrada = $Entrada.Split("/")[-1]
    $salida = "Salida."+ $Entrada
 $result | Out-File -FilePath  $salida
 return 0
 }
 
function  scalar_product($a , $scalar) {

$result = ""
 for($i=0;$i -lt $a.count;$i++) {
for( $j=0;$j -lt $a[$i].count;$j++){
   $suma= $a[$i][$j] * $scalar
  if($j -ne 0){
   $result+= '|'
   }
   $result+=$suma
  }
  $result += "`r`n"
 }
 $Entrada = $Entrada.Split("/")[-1]
 $salida = "Salida."+ $Entrada
$result | Out-File -FilePath  $salida
return 0
}
function ObtenerPathAbsoluto()
{
    Param ( [string] $pathIn )

    if ([System.IO.Path]::IsPathRooted($pathIn)) {
        $pathOut = $pathIn
    } else{
        $pathOut = Join-Path (Get-Location) $pathIn
    }

   return $pathOut
}



##leer archivo en una matriz

$Entrada = ObtenerPathAbsoluto $Entrada
$matFromFile = @()  
foreach($line in Get-Content $Entrada) {

      [double[]] $fila = @($line.split('|'))
     $matFromFile += , $fila
}
if($matFromFile.count -lt 1){
Write-Error "Ha ocurrido un error al obtener la matriz. Verifique que el archivo no esté vacío"
return
}
$resultCode = 0
if($Suma){
$Suma = ObtenerPathAbsoluto $Suma

$matToAdd = @()
foreach($line in Get-Content $Suma) {

      [double[]] $fila = @($line.split('|'))
     $matToAdd += , $fila
}
$resultCode = do_add $matFromFile $matToAdd
}else{
$resultCode = scalar_product $matFromFile $Producto
}
if($resultCode -eq 0){
Write-Host "El proceso se realizó correctamente"
}
