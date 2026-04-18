#include <stdio.h>    // Proporciona printf (salida a pantalla) y perror (mensajes de error).
#include <stdlib.h>   // Proporciona exit() para finalizar el programa en caso de error.
#include <string.h>   // Proporciona funciones de manipulación de texto (aunque aquí usas sprintf).
#include <fcntl.h>    // Define las constantes de control: O_CREAT (crear) y O_RDWR (lectura/escritura).
#include <sys/mman.h> // Define las funciones de mapeo de memoria: shm_open, mmap y munmap.
#include <unistd.h>   // Define ftruncate (ajustar tamaño), sleep (pausa) y close (cerrar archivo).

int main() {
    // Define el nombre del objeto de memoria. En POSIX debe empezar con '/' (se crea en /dev/shm/).
    const char *nombre = "/Mi_memoria_compartida"; 
    
    // Define el tamaño total de la memoria que vamos a reservar (1024 bytes = 1 KB).
    const int SIZE = 1024;

    // 1. Crear el objeto de memoria compartida.
    // shm_open devuelve un "File Descriptor" (fd), un entero que identifica el recurso.
    // O_CREAT: Crea el objeto si no existe.
    // O_RDWR: Abre el objeto para poder leer y escribir en él.
    // 0666: Permisos de lectura y escritura para dueño, grupo y otros.
    int fd = shm_open(nombre, O_CREAT | O_RDWR, 0666);
    
    // Si shm_open devuelve -1, ocurrió un error (ej. falta de permisos o memoria).
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Imprimimos el ID del descriptor de archivo (suele ser un número pequeño como 3).
    printf("\n[Escritor] Memoria creada, ID: %d\n", fd);

    // 2. Establecer el tamaño del segmento de memoria.
    // shm_open crea el objeto con tamaño 0. ftruncate lo expande al tamaño definido en SIZE.
    ftruncate(fd, SIZE);

    // 3. Mapear el objeto de memoria al espacio de direcciones del proceso.
    // mmap hace que una dirección de memoria (ptr) apunte directamente al objeto compartido.
    // 0: El sistema elige la dirección de memoria automáticamente.
    // SIZE: Cuánta memoria queremos mapear.
    // PROT_WRITE: Indica que tenemos permiso para escribir en esta memoria.
    // MAP_SHARED: Los cambios hechos aquí serán visibles para otros procesos.
    // fd: El descriptor que obtuvimos en shm_open.
    // 0: El punto de inicio (offset) dentro del objeto de memoria.
    char *ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    
    // Imprimimos la dirección de memoria virtual (puntero) donde se "conectó" el segmento.
    printf ("[Escritor] Direccion de Memoria: %p\n", (void*)ptr);

    // 4. Escribir el Mensaje en la memoria compartida.
    const char *mensaje = "Hola desde C!";
    
    // sprintf escribe el contenido de 'mensaje' en la dirección a la que apunta 'ptr'.
    // Esto es equivalente a escribir directamente en la RAM compartida.
    sprintf(ptr, "%s", mensaje);

    printf("[C Escritor] Datos escritos en %s\n", nombre);
    printf("[C Escritor] Esperando 10 segundos...\n");

    // Pausamos 10 segundos para que nos dé tiempo a ejecutar el lector antes de que se borre.
    sleep(10);

    // 5. Limpieza del recurso (Gestión de memoria).
    
    // munmap deshace el mapeo: el puntero 'ptr' deja de ser válido para este proceso.
    munmap(ptr, SIZE);
    
    // close cierra el descriptor de archivo 'fd'. El proceso ya no puede manipular el objeto.
    close(fd);
    
    // shm_unlink elimina el nombre del objeto del sistema operativo.
    // La memoria se libera físicamente cuando el último proceso que la usa se desconecta.
    shm_unlink(nombre); 

    return 0; // Finaliza el programa con éxito.
}
