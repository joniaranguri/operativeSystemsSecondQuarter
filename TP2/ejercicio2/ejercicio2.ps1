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
#>

Param (
 [Parameter(Mandatory = $true)]
 [ValidateRange(1, [int]::MaxValue)]
 [int] $Cantidad,
 [string] $help
)
if ($help = 'Get-Help') 
{
    Write-Host "Ayuda jeje"
    Exit-PSHostProcess
}
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