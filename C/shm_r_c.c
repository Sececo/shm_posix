#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#define SHM_SIZE 1024
#define CLAVE 1234  // Misma clave que el escritor
int main() {
    int shmid;
    char *memoria;
    // 1. Obtener el segmento existente (misma clave)
    shmid = shmget(CLAVE, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget (¿Ejecutaste primero el escritor?)");
        exit(1);
    }
    printf("[Lector] Memoria encontrada, ID: %d\n", shmid);
    // 2. Adjuntar a mi espacio de direcciones
    memoria = shmat(shmid, NULL, 0);
    if (memoria == (char *)-1) {
        perror("shmat");
        exit(1);
    }
    // 3. Leer directamente (misma dirección física)
    printf("[Lector] Dirección de memoria: %p\n", (void*)memoria);
    printf("[Lector] Datos leídos: '%s'\n", memoria);
    // 4. Desadjuntar (no eliminar, el escritor lo hace)

sleep(15);
    shmdt(memoria);
    printf("[Lector] Fin.\n");
    return 0;
}
