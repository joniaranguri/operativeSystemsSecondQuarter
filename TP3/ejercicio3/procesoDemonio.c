#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include<string.h>

int  validarParametros(int arg,char *args []){
//args[0] nombre de script
//args[1] fifo consulta
//args[2] fifo resultado
//args[3] archivo 
if(arg<4){
    printf("CANTIDA DE PARAMETROS INCORRECTOS,VERIFIQUE LA AYUDA\n");
    return 1;
}
FILE *pf;
pf=fopen(args[1],"r");
 if(!pf){
        printf ("no se encuentra el archivo");
        return 1;
    }
   //falta validad los paths para los fifos
    

return  0;


}
void mostrarAyuda(){
    printf("\n Ejemplo de ejecucion:\n");
    printf("\t procesoDemonio.C  ./articulos.txt ./fifoConsulta ./fifoResultado \n");
    return ;
}
void agregarSalida(char out[],char id[],char articulo[],char producto[],char marca[]){
            strcat(out,id);
            strcat(out,";");
            strcat(out,articulo);
            strcat(out,";");
            strcat(out,producto);
            strcat(out,";");
            strcat(out,marca);
            strcat(out,"\n");
            return ;
}

int obtenerCantidadDeRegistros(char *path[]){
    FILE *pf;
    int cantfilas=0;
    pf=fopen(*path,"r");
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
void filtrarArchivo(char *path[],char *filtro,int registros,char *salida)
{
    FILE *pf;
    int esId=strncmp("ID",filtro,2);
    int esProducto=strncmp("PRODUCTO",filtro,8);
    int esMarca=strncmp("MARCA",filtro,5);
    char id[100];
    char articulo[100];
    
    char producto[100];
    char marca[100];
    char *igual=strchr(filtro,'=');
    char *buscado=igual;
    //char salida[registros*500];

    strcpy(salida," ");
    buscado++;
        int cantCadenaOriginal=strlen(filtro);
    int cantBuscado=strlen(igual);
   // printf("\ncant Caracteres: %d\n",cantCadenaOriginal);
    printf("\n buscado: %s\n",buscado);

   // printf("\n lo que se busca:%s\n",igual);
    
    printf("\nFILTRO: %s\n",filtro);
    pf = fopen(*path, "r");
    if (!pf)
    {
       
        printf("no se  encontro el archivo");
        exit(0);
    }
    int j= 1;

    printf("\n esId:%d ,esProducto:%d esMarca:%d \n",esId,esProducto,esMarca);
    while (!feof(pf))
    {
        printf(" iteracion %d \n", j);
        j++;
        fscanf(pf, " %[^;]", id);
        printf("id :%s \n", id);

        fscanf(pf, " ;%[^;]", articulo);
        printf("articulo:%s \n", articulo);

        fscanf(pf, " ;%[^;]", producto);
        printf("producto:%s \n", producto);

        fscanf(pf, " ;%[^\n]", marca);
        printf("marca:%s \n", marca);
      // printf("\n resultado de comparacion: %d\n",  strcmp(id,buscado));  
        if(esId==0 && strcmp(id,buscado)==0 ){   
            printf("\n  entro por ID : %s %s %s %s ",id,articulo,producto,marca); 
         //   printf("\n resultado de comparacion: %d\n",  comparar(id,buscado));   
            agregarSalida(salida,id,articulo,producto,marca);   
           
        }
        else if(esMarca==0 && strcmp(marca,buscado)==0){
            agregarSalida(salida,id,articulo,producto,marca);   
        }
        else if(esProducto==0 && strcmp(producto,buscado)==0){
             agregarSalida(salida,id,articulo,producto,marca);   
        }
        else{
            printf("\nno hay coincidencia\n");
            
        }
        strcpy(id," ");
        strcpy(articulo," ");
        strcpy(producto," ");
        strcpy(marca," ");
        
    }
    fclose(pf);
    //printf("***********SALIDA***********\n%s",salida);
    return;
}
int main(int arg, char *args[])
{
    int x;
    int num = arg;
    
    printf("ANTES DEL IF, llego el %s\n",args[1]);
    if(arg== 2 && (strcmp(args[1],"-h")==0 ||strcmp(args[1],"-?")==0 || strcmp(args[1],"-help")==0)  ){
        printf("entro");
        mostrarAyuda();
        
        return 0;
    }
    int res=validarParametros(arg,args);
    if(res==1){
        return 0;
    }
    // creo un proceso hijo
    x = fork();
    //si es el padre termino, asi el hijo queda como demonio

    printf("X=%d\n", x);
    if (x > 0)
    {
        return 0;
    }
// crear fifos
    mkfifo("fifoConsulta", 0666);
    mkfifo("./fifoResultado", 0666); 
    //si es el hijo se queda ejecutando
     while(1){
        // abrir fifos 
    int fd = open("fifoConsulta", O_RDONLY); 
     
   // printf("*******************SE CREO EL SEGUNDO FIFO*******************");
    int fds = open("./fifoResultado", O_WRONLY);
   // printf("****************************SE ABRIO EL SEGUNDO FIFO********************");
   
    char filtro[100];
    int bytes = -1;
    printf("\n*******************ESPERANDO CONSULTA*******************\n");
    bytes = read(fd, filtro, sizeof(char)*1000); // leer fifo
    printf("\n ****** LLEGO LA CONSULTA********\n");
    
    int cantCaracteres=strlen(filtro);
    char *aMayuscula=filtro;
    printf("caracteres filtro: %d",cantCaracteres);

    // pongo en mayuscula el filtro
    for(int i=0; i<cantCaracteres;i++){
        *aMayuscula=toupper(*aMayuscula);
        aMayuscula++;
    }


  //  printf("filtro:%s",filtro);
    int registros=obtenerCantidadDeRegistros(&args[1]);
   // printf("cant registros: %d",registros);
      char salida[registros*500];

    filtrarArchivo(&args[1],filtro,registros,salida);
    //escribirFifoSalida()
    printf("\n******ARCHIVO FILTRADO*******\n");
  
    //printf("\n *****  size de salida: %ld",sizeof(salida));
    write(fds, salida, sizeof(salida));
    //printf("************************SE ESCRIBIO EN EL SEGUNDO FIFO*******************");
    close(fds);
    close(fd);
    }
   

    return(0);
}