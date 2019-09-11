
function MostrarCantidadProcesos (){

    
}
Param(
    [Parameter( ParameterSetName ="Procesos" )]
    [switch] $Procesos,
    [Parameter(ParameterSetName ="Peso")]
    [switch] $Peso,
    [Parameter(Mandatory=$true ,ParameterSetName ="Peso")]
    [String] $Directorio

)
if($Procesos -eq $true ){
MostrarCantidadProcesos
}