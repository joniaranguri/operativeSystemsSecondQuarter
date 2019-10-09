
################  Ayuda ############################# 

<#
    .SYNOPSIS
    
	    Infomar, comprimir y descomprimir Zip. 
    
    .DESCRIPTION
    
	    Este script permite comprimir directorios, informar y descomprimir archivos zip. 
    
    .EXAMPLE
    
        Opcion 1) <DIRECTORIO_SCRIPT>\Ejercicio4.ps1 -Informar -ArchivoZip C:\directorio\archivoZip.zip
        
        Opcion 2) <DIRECTORIO_SCRIPT>\Ejercicio4.ps1 -Comprimir -ArchivoZip C:\directorio\archivoZip.zip -Directorio  C:\directorio\DirectorioAComprimir
        
        Opcion 3) <DIRECTORIO_SCRIPT>\Ejercicio4.ps1 -Descomprimir -ArchivoZip C:\directorio\archivoZip.zip -Directorio C:\directorio\DirectorioADescomprimir
    
    .PARAMETER Informar

        Parametro Switch. Indica que el modo de operación es “Información”.

    .PARAMETER Comprimir

        Parametro Switch. Indica que el modo de operación es “Compresión”.

    .PARAMETER Descomprimir

        Parametro Switch. Indica que el modo de operación es “Descompresión”.

    .PARAMETER ArchivoZip
    
        Parametro obligatorio. Path del archivo ZIP. Este parámetro se usará para cualquiera de los tres modos de operación del script.

    .PARAMETER Directorio
        Parametro obligatorio para Comprimir y descomprimir. Indica el directorio a comprimir o el destino de la descompresión del archivo ZIP.
   
#>



################  Declaracion de parametros ############################# 



Param(
    [Parameter(ParameterSetName = 'Comprimir', Position = 0, Mandatory = $true)] 
    [switch]  $Comprimir,
    
    [Parameter(ParameterSetName = 'Descomprimir', Position = 0, Mandatory = $true)] 
    [switch]  $Descomprimir,

    [Parameter(ParameterSetName = 'Informar', Position = 0, Mandatory = $true)] 
    [switch]  $Informar,

    [Parameter(ParameterSetName = 'Comprimir', Position = 1, Mandatory = $true)]
    [Parameter(ParameterSetName = 'Descomprimir', Position = 1, Mandatory = $true)]
    [Parameter(ParameterSetName = 'Informar', Position = 1, Mandatory = $true)]
    [ValidateNotNullOrEmpty()] 
    [String]  $ArchivoZip,
    
    [Parameter(ParameterSetName = 'Comprimir', Position = 2, Mandatory = $true)]
    [Parameter(ParameterSetName = 'Descomprimir', Position = 2, Mandatory = $true)]
    [ValidateNotNullOrEmpty()] 
    [String]  $Directorio
)



############### Funciones ############################################

function Validar-Directorio()
{
    Param ( [string] $directorio )
    $Valid = Test-Path -Path $directorio;
    return $Valid
    
}
function Validar-Archivo()
{
    Param ( [string] $archivo )
    $Valid = Test-Path -Path $archivo;
    return $Valid
    
}


function Comprimir()
{
    Param ( [string] $directorio, [string] $archivoZip )
    [System.IO.Compression.ZipFile]::CreateFromDirectory($directorio,$archivoZip)
}


function Descomprimir()
{
    Param ( [string] $directorio, [string] $archivoZip )
    [System.IO.Compression.ZipFile]::ExtractToDirectory($archivoZip,$directorio)
}


#probar...

function Informar()
{
   Param ( [string] $archivoZip )
   $zipFile = [System.IO.Compression.ZipFile]::OpenRead($archivoZip).Entries.Name
   
   Write-Output $zipFile
}



#################### MAIN ##################################################


If ($PSBoundParameters.ContainsKey('Informar')) {

   Informar $ArchivoZip
   return 
}
Else
{
    If (!Validar-Directorio $Directorio){
        return
    }

    If ($PSBoundParameters.ContainsKey('Comprimir')) {
        
       Comprimir $Directorio $ArchivoZip
       return
    }

    If ($PSBoundParameters.ContainsKey('Descomprimir')) {
        
       If (!Validar-Archivo $ArchivoZip){
            return
       }

       Descomprimir $Directorio $ArchivoZip
       return 
    }
} 


