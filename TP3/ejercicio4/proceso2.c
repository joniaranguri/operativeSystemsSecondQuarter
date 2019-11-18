#include <stdio.h>
#include <stddef.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <zconf.h>
#include <stdint.h>

int main(int arg, char *args[]) {
    int fd;
    size_t tam = sizeof(int);
    sem_t *sem = sem_open("semaforo", O_CREAT, 0600, 0);

    fd = shm_open("/nombre", O_CREAT, 0600);
    ftruncate(fd, tam);
    //mem es la memoria compartida, si cambio su valor estoy escribiendo en
    //la mem compartida, si veo su valor estoy leyendo de memoria compartida
    uint8_t *mem = mmap(NULL, tam, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd, 0);
    sem_wait(sem); // P()
    printf("valor: %d", *mem);
    sem_close(sem);
    sem_unlink("semaforo");
    close(fd);

}