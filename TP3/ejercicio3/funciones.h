int validarPathFifo(char * path  );
int  validarParametros(int arg,char *args []);
void agregarSalida(char out[],char id[],char articulo[],char producto[],char marca[]);
int obtenerCantidadDeRegistros(char *path[]);
void filtrarArchivo(char *path[],char *filtro,int registros,char *salida);
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