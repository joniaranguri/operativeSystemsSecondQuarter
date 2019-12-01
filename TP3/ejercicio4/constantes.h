#define SEMAFORO_A "semaforo_a"
#define SEMAFORO_B "semaforo_b"
#define SEMAFORO_C "semaforo_c"
#define SEMAFORO_D "semaforo_d"
#define MEMORIA_CONSULTA "/memoria_consulta"
#define MEMORIA_RESULTADOS "/memoria_resultados"

typedef struct {
    int siguiente;
    char valor[100];
}mensaje;