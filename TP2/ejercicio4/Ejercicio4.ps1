
# Trabajo práctico N2 Ejercicio 4 (Primera entrega)
# Script: ejercicio4.sh
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284

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
    [Parameter(ParameterSetName = 'Comprimir', Mandatory = $true)] 
    [switch]  $Comprimir,
    
    [Parameter(ParameterSetName = 'Descomprimir', Mandatory = $true)] 
    [switch]  $Descomprimir,

    [Parameter(ParameterSetName = 'Informar', Mandatory = $true)] 
    [switch]  $Informar,

    [Parameter(ParameterSetName = 'Comprimir', Mandatory = $true)]
    [Parameter(ParameterSetName = 'Descomprimir', Mandatory = $true)]
    [Parameter(ParameterSetName = 'Informar', Mandatory = $true)]
    [ValidateNotNullOrEmpty()] 
    [String]  $ArchivoZip,
    
    [Parameter(ParameterSetName = 'Comprimir', Mandatory = $true)]
    [Parameter(ParameterSetName = 'Descomprimir', Mandatory = $true)]
    [ValidateNotNullOrEmpty()] 
    [String]  $Directorio
)



############### Funciones ############################################

function ValidarDirectorio()
{
    Param ( [string] $directorio )
    $Valid = Test-Path -Path $directorio;
    return $Valid
    
}
function ValidarArchivo()
{
    Param ( [string] $archivo )
    $Valid = Test-Path -Path $archivo;
    return $Valid
    
}


function Comprimir()
{
    Param ( [string] $directorio, [string] $archivoZip )

        
    $existe = ValidarArchivo $archivoZip

    if($existe)
    {
        write-output "Ya se encuentra el archivo que desea crear"
        return 
    }

    Add-Type -AssemblyName System.IO.Compression.FileSystem
    [System.IO.Compression.ZipFile]::CreateFromDirectory($directorio,$archivoZip)
    Write-Output "Se ha comprimido el archivo correctamente"
}


function Descomprimir()
{
    Param ( [string] $directorio, [string] $archivoZip )
	Add-Type -AssemblyName System.IO.Compression.FileSystem
    [System.IO.Compression.ZipFile]::ExtractToDirectory($archivoZip,$directorio)
}

function Informar()
{
   Param ( [string] $archivoZip )
   Add-Type -AssemblyName System.IO.Compression.FileSystem
   $zipFile = [System.IO.Compression.ZipFile]::OpenRead($archivoZip).Entries.Name
   
   Write-Output $zipFile
}



#################### MAIN ##################################################


If ($PSBoundParameters.ContainsKey('Informar')) {

	If (!(ValidarArchivo $ArchivoZip) ){
			Write-Output "no se encontro el archivo ingresado: " $ArchivoZip
			return
    }
   Write-Output "el archivoZIP esta compuesto por: " 
   Informar $ArchivoZip
   return 
}
Else
{
    If ( ! (ValidarDirectorio $Directorio) ){
        Write-Output "no se encontro el directorio ingresado: " $Directorio
		return
    }

    If ($PSBoundParameters.ContainsKey('Comprimir')) {
        
       Comprimir $Directorio $ArchivoZip
       return
    }

    If ($PSBoundParameters.ContainsKey('Descomprimir')) {
        
       If (!( ValidarArchivo $ArchivoZip) ){
			Write-Output "no se encontro el archivo ingresado: " $ArchivoZip 
			return
       }

       Descomprimir $Directorio $ArchivoZip
       Write-Output "Se ha descomprimido el archivo correctamente"
       return 
    }
} 


