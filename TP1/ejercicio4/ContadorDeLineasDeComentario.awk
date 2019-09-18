#!/bin/awk
# Trabajo práctico N1 Ejercicio 4 (Primera entrega)
# Script: contadorDeLineasDeComentario.awk
# Integrantes:
# Annacondia Federico Agustin 38.435.945
# Aranguri Jonathan Enrique   40.672.991	
# Diaz Adrian Maximiliano     38.167.742
# Icardi Fernando Javier      34.412.142
# Rodriguez Gonzalo Martin    39.461.284
################MAIN####################
BEGIN {
    contadorLineasComentadas=0;
    contadorLineasCodigo=0;
    contadorDeLineaComentadasMultiples=0;
    comentarioMultilineaAbierto=0;
    esComentarioMultilinea=0;
}
{
    esLineaComentario=0;
    esLineaCodigo=0;

    for(campoActual=1;campoActual<=NF;campoActual++)
    {
        
       if ( match($campoActual,"//") && esComentarioMultilinea == 0 )
        {
            contadorLineasComentadas++;
            esLineaComentario=1;
        }
        
        {
        if ( match($campoActual,"/\*") )
           esComentarioMultilinea=1;
        }

        if ( match($campoActual,"\*/") )
        {
           contadorLineasComentadas = contadorLineasComentadas+1
           esComentarioMultilinea=0
            
        }else if ( esLineaComentario == 0 && esComentarioMultilinea == 0)
        {
            esLineaCodigo=1;
        }

       
    }
    if (esLineaCodigo == 1)
    {
        contadorLineasCodigo= contadorLineasCodigo+1
    }
    if (esComentarioMultilinea == 1)
    {
        contadorLineasComentadas = contadorLineasComentadas+1
    }
    
}
END{
    print NR; 
    print contadorLineasComentadas;
    print contadorLineasCodigo;
}
################FIN##############################