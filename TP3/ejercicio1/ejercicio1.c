/* Trabajo práctico N3 Ejercicio 1 (Segunda entrega)
    Script: ejercicio1.c
    Integrantes:
         Diaz Adrian Maximiliano     38.167.742
         Rodriguez Gonzalo Martin    39.461.284
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#include <string.h>

#define SEMAFORO "semaforo"

pid_t hijo1();

pid_t hijo2();

pid_t nieto1();

pid_t nieto2();

pid_t nieto3();

pid_t bisnieto1();

pid_t bisnieto2();

pid_t bisnieto3();

pid_t bisnieto4();

pid_t bisnieto5();

int soyHijo(int);

void esperarYSalir(int count, ...);

int status;
sem_t *sem;

void mostrarAyuda() {
    printf("\n Ejemplo de ejecucion: \n ./ejercicio1\n");
}

int main(int arg, char *args[]) {

    if (arg == 2 && (strcmp(args[1], "-h") == 0 || strcmp(args[1], "-help") == 0 || strcmp(args[1], "-?") == 0)) {
        mostrarAyuda();
        return 0;
    }
    if (arg != 1) {
        printf("\n Cantidad de parametros incorrecta, verifique la ayuda\n");
        return 1;
    }

    sem_unlink(SEMAFORO);
    sem = sem_open(SEMAFORO, O_CREAT, 0600, 0);

    pid_t pid1 = hijo1();
    pid_t pid2;

    if (!soyHijo(pid1)) {
        pid2 = hijo2();
    }

    printf("ingrese una tecla para finalizar\n");
    getchar();

    sem_post(sem);
    esperarYSalir(2, pid1, pid2);
    sem_close(sem);
    return 0;
}

pid_t hijo1() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [hijo]-[normal]\n", getpid(), getppid());
        pid_t pid2 = nieto1();
        if (pid2 != 0) {
            pid_t pid3 = nieto2();
            esperarYSalir(1, pid3);
        }

        esperarYSalir(1, pid2);
    }

    return pid;
}

pid_t hijo2() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [hijo]-[normal]\n", getpid(), getppid());
        pid_t pid2 = nieto3();
        esperarYSalir(1, pid2);
    }

    return pid;
}

pid_t nieto1() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [nieto]-[normal]\n", getpid(), getppid());
        pid_t pid2 = bisnieto1();
        if (!soyHijo(pid2)) {
            pid_t pid3 = bisnieto2();
            esperarYSalir(1, pid3);
        }
        esperarYSalir(1, pid2);
    }

    return pid;
}

pid_t nieto2() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [nieto]-[zombie]\n", getpid(), getppid());
        pid_t pid2 = bisnieto3();
        if (!soyHijo(pid2)) {
            pid_t pid3 = bisnieto4();
            exit(1); //esperarYSalir(2, pid2, pid3);
        }
    }

    return pid;
}

pid_t nieto3() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [nieto]-[zombie]\n", getpid(), getppid());
        pid_t pid2 = bisnieto5();
        exit(1);
    }

    return pid;
}

pid_t bisnieto1() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [bisnieto]-[normal]\n", getpid(), getppid());
    }

    return pid;
}

pid_t bisnieto2() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [bisnieto]-[normal]\n", getpid(), getppid());
    }

    return pid;
}

pid_t bisnieto3() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [bisnieto]-[demonio]\n", getpid(), getppid());
    }

    return pid;
}

pid_t bisnieto4() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [bisnieto]-[demonio]\n", getpid(), getppid());
    }

    return pid;
}

pid_t bisnieto5() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [bisnieto]-[demonio]\n", getpid(), getppid());
    }

    return pid;
}

void esperarYSalir(int count, ...) {
    sem = sem_open(SEMAFORO, O_CREAT, 0600, 0);

    sem_wait(sem);

    sem_post(sem);

    va_list list;
    va_start(list, count);

    for (int i = 0; i < count; i++) {
        waitpid(va_arg(list, pid_t), &status, 0);
    }

    exit(1);
}

int soyHijo(int pid) {
    return pid == 0;
}
