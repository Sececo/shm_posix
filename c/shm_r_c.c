#include <stdio.h>    // Biblioteca para printf y perror (mensajes de error).
#include <stdlib.h>   // Biblioteca para exit() (finalización del programa).
#include <unistd.h>   // Biblioteca para sleep() y close().
#include <fcntl.h>    // Constantes de control de archivos como O_RDONLY.
#include <sys/mman.h> // Funciones de memoria compartida POSIX: shm_open, mmap, munmap.

int main() {
    // El nombre debe coincidir exactamente con el del Escritor ("/Mi_memoria_compartida").
    const char *nombre = "/Mi_memoria_compartida";
    // El tamaño debe ser suficiente para leer lo que el escritor reservó.
    const int SIZE = 1024;

    // 1. Obtener el segmento existente (misma clave/nombre)
    // shm_open abre el objeto de memoria ya creado por el escritor.
    // O_RDONLY: Abre el segmento en modo "Solo Lectura".
    // 0666: Permisos de acceso (deben ser compatibles con los del creador).
    int fd = shm_open(nombre, O_RDONLY, 0666);
    
    // Si fd es -1, significa que el objeto no existe o no tenemos permisos.
    if (fd == -1) {
        perror("[Lector] shm_open (¿Ejecutaste primero el escritor?)");
        exit(1);
    }
    
    // Imprime el descriptor de archivo (identificador interno del proceso).
    printf("[Lector] Memoria encontrada, ID: %d\n", fd);

    // 2. Adjuntar a mi espacio de direcciones
    // mmap proyecta el objeto de memoria en el espacio virtual de este proceso.
    // 0: El kernel elige la dirección de memoria.
    // PROT_READ: Solo queremos leer (coincide con O_RDONLY).
    // MAP_SHARED: Compartimos la misma memoria física que el escritor.
    // fd: El descriptor que acabamos de abrir.
    // 0: Empezamos a leer desde el inicio del bloque (offset 0).
    char *ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    
    // MAP_FAILED es una macro que indica que el mapeo de memoria falló.
    if (ptr == MAP_FAILED) {
        perror("[Lector] mmap");
        exit(1);
    }

    // 3. Leer directamente (misma dirección física)
    // Muestra la dirección virtual donde este proceso ve la memoria compartida.
    printf("[Lector] Dirección de memoria: %p\n", (void*)ptr);
    
    // Muestra el nombre del objeto que estamos consultando.
    printf("[Lector] Direccion de Memoria compartida: '%s'\n", nombre);
    
    // Accede al contenido apuntado por 'ptr'. Al ser una cadena de texto, lo lee con %s.
    printf("[Lector] Datos leídos: '%s'\n", ptr);

    // 4. Desadjuntar (no eliminar, el escritor lo hace)

    // Pausa de 15 segundos para asegurar que el escritor no borre la memoria 
    // mientras el lector aún está activo o para inspección del usuario.
    sleep(15);
    
    // munmap libera el mapeo de memoria virtual del proceso lector.
    munmap(ptr, SIZE);
    
    // close cierra el descriptor de archivo, liberando el recurso en este proceso.
    close(fd);

    // El Lector NO usa shm_unlink porque el Escritor es el dueño del ciclo de vida.
    printf("[Lector] Fin.\n");
    
    return 0;
}
