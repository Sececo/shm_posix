 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/ipc.h>
 #include <sys/shm.h>
 #include <unistd.h>

 #define SHM_SIZE 1024
 #define CLAVE 1234


    int main() {
    int shmid;
    char *memoria;

    // 1. Crear segmento de memoria compartida
    shmid = shmget(CLAVE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    printf("[Escritor] Memoria creada, ID: %d\n", shmid);
    // 2. Adjuntar (mapear) a espacio de direcciones
    memoria = shmat(shmid, NULL, 0);
    if (memoria == (char *)-1) {
        perror("shmat");
        exit(1);
    }
    // 3. Escribir directamente en la memoria compartida
    strcpy(memoria, "oye te hablo desde la prisión! wilson manyoma ");
    printf("[Escritor] Datos escritos: '%s'\n", memoria);
    printf("[Escritor] Dirección de memoria: %p\n", (void*)memoria);
    // 4. Esperar a que el lector lea
    printf("[Escritor] Esperando 5 segundos...\n");
    sleep(15);
    // 5. Desadjuntar y liberar
    shmdt(memoria);
    shmctl(shmid, IPC_RMID, NULL);  // Eliminar segmento
    printf("[Escritor] Memoria liberada. Fin.\n");
    return 0;
}
