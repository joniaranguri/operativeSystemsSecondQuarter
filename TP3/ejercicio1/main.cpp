#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <cstring>

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
bool soyHijo(int);

void esperarYSalir(int count, ...);

int status;
sem_t* sem;

int main(int args, char * argv[]) {

    sem = sem_open("sem", O_CREAT, 0600, 0);

    pid_t pid1 = hijo1();
    pid_t pid2;

    if (!soyHijo(pid1)) {
        pid2 = hijo2();
    }

    printf("ingrese una tecla para finalizar\n");
    getchar();
    sem_post(sem);

    esperarYSalir(2, pid1, pid2);

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
            printf("PID: %d PPID: %d Parentesco-Tipo: [nieto]-[normal]\n", getpid(), getppid());
            pid_t pid2 = bisnieto3();
            if (!soyHijo(pid2)) {
                pid_t pid3 = bisnieto4();
                esperarYSalir(1, pid3);
            }
            esperarYSalir(1, pid2);
        }

    return pid;
}

pid_t nieto3() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [nieto]-[normal]\n", getpid(), getppid());
        pid_t pid2 = bisnieto5();
        esperarYSalir(1, pid2);
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
        printf("PID: %d PPID: %d Parentesco-Tipo: [bisnieto]-[normal]\n", getpid(), getppid());
    }

    return pid;
}

pid_t bisnieto4() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [bisnieto]-[normal]\n", getpid(), getppid());
    }

    return pid;
}

pid_t bisnieto5() {
    pid_t pid = fork();

    if (soyHijo(pid)) {
        printf("PID: %d PPID: %d Parentesco-Tipo: [bisnieto]-[normal]\n", getpid(), getppid());
    }

    return pid;
}

void esperarYSalir(int count, ...) {
    sem = sem_open("sem", O_CREAT, 0600, 0);

    sem_wait(sem);

    sem_post(sem);

    va_list list;
    va_start(list, count);

    for (int i = 0; i < count; i++) {
        waitpid(va_arg(list, pid_t), &status, 0);
    }

    exit(1);
}

bool soyHijo(int pid) {
    return pid == 0;
}
