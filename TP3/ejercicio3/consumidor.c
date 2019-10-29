#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
void mostrarAyuda(){
    printf("\n Ejemplo de Ejecucion: ./consumir ./articulos.txt ./fifoResultado \n");
    return ;
}
int obtenerCantidadDeRegistros(char path[]){
    FILE *pf;
    int cantfilas=0;
    pf=fopen(path,"r");
    if(!pf){
        printf ("no se encuentra el archivo");
        exit(0);
    }
    char fila[100];
    while(!feof(pf)){
        fscanf(pf," %[^\n]",fila);
      	cantfilas++;
	}
    fclose(pf);
    return cantfilas;

}
int main (int arg , char * args[]){
 if(arg== 2 && (strcmp(args[1],"-h")==0 ||strcmp(args[1],"-?")==0 || strcmp(args[1],"-help")==0)  ){
      
        mostrarAyuda();
        
        return 0;
    }

    
    int fd= open("fifoResultado",O_RDONLY); // abrir fifo para lectura
        printf("\n*****************SE ABRIO EL FIFO*********\n");
        
        int registros=obtenerCantidadDeRegistros(&args[1]);
       
    char entrada[registros*500];
    int bytes=-1;
     bytes= read(fd,entrada,sizeof(entrada));   // leer fifo
        printf("\n************SALIDA****\n");
  //  close(fd);
            //  printf("soy el hijo y  es %d ", num);
         printf("\n %s\n ",entrada); 

return 0;
}