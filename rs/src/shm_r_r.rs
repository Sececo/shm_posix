use shared_memory::{ShmemConf}; // Importa la estructura ShmemConf para configurar y abrir la memoria compartida.

fn main (){
    // 1. Intentamos abrir la memoria por su nombre
    // ShmemConf::new() crea una nueva instancia de configuración.
    // .os_id("/Mi_memoria_compartida") especifica el nombre exacto del objeto en el sistema operativo.
    // .open() intenta mapear el segmento ya existente en el espacio de direcciones de este proceso.
    // .expect(...) detiene el programa y lanza este mensaje si la memoria no existe o no es accesible.
    let shmem = ShmemConf::new()
        .os_id("/Mi_memoria_compartida")
        .open()
        .expect("No se encontró la memoria de C. ¿Ya corriste el escritor?");

    // shmem.as_ptr() obtiene la dirección de memoria donde se mapeó el segmento.
    // 'as usize' convierte ese puntero en un número entero para poder imprimirlo o manipularlo.
    let direccion = shmem.as_ptr() as usize;

    // Imprime la dirección de memoria en formato hexadecimal ({:#?}). 
    // Nota: El '%d\n' al final es texto literal, no un formateador de C en este caso.
    println!("[Lector] ¡Memoria encontrada! {:#?} %d\n", direccion);

    // 2. Leemos los bytes
    // Se requiere el bloque 'unsafe' porque estamos trabajando con punteros crudos (raw pointers)
    // que Rust no puede garantizar que sean válidos en tiempo de compilación.
    unsafe {
        // Obtenemos el puntero inicial del segmento de memoria compartida.
        let ptr = shmem.as_ptr();

        // Creamos una "vista" (slice) de 20 bytes para leer.
        // std::slice::from_raw_parts construye un slice de Rust ([u8]) a partir del puntero y una longitud fija.
        let slice = std::slice::from_raw_parts(ptr, 20);

        // Convertimos esos bytes a texto legible (UTF-8).
        // from_utf8_lossy reemplaza los bytes que no sean caracteres válidos por un símbolo de reemplazo ().
        let mensaje = String::from_utf8_lossy(slice);

        // Muestra en consola el contenido extraído de la memoria compartida.
        println!("[Lector] El mensaje es: {}", mensaje);
    }
}

