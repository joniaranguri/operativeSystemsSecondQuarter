#!/bin/awk

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
        
       if ( match($campoActual,"//"))
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
            
        }

        if ( esLineaComentario == 0 && esComentarioMultilinea == 0)
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
