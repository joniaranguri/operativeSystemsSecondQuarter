#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
int main (int arg , char * args[]){
    char *entrada=args[1];
    printf("\n%s \n",args[1]);
    int cantCaracteres= strlen(entrada);
    printf("caracteres %d\n",cantCaracteres);

   
    int fd=open("fifoConsulta",O_WRONLY);
    write(fd,args[1],sizeof(char)*(cantCaracteres+1));
   
    return 0;
    }
