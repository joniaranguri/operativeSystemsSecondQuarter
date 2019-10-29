#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
void mostrarAyuda(){
    printf("\n Ejemplo de ejecucion: ./consultor producto=P.DULCE ./fifoConsulta");
}

int main (int arg , char * args[]){
    
    char *entrada=args[1];
    printf("\n%s \n",args[1]);
    int cantCaracteres= strlen(entrada);
    printf("caracteres %d\n",cantCaracteres);
    if(arg==2 && (strcmp(args[1],"-h")==0 || strcmp(args[1],"-help")==0 || strcmp(args[1],"-?")==0)){
        mostrarAyuda();
        return 0;
    }
   
    int fd=open(args[2],O_WRONLY);
    write(fd,args[1],sizeof(char)*(cantCaracteres+1));
   
    return 0;
    }
