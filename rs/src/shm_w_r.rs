use shared_memory::{ShmemConf, Shmem}; // Importa ShmemConf (configurador) y la estructura Shmem (el recurso).

fn main (){
    // Define el identificador del objeto de memoria que se creará en el sistema.
    let memoria = "/Mi_memoria_compartida";

    // 1. Configuración y Creación
    // ShmemConf::new() inicia la construcción del segmento.
    // .os_id(memoria) establece el nombre del "archivo" en /dev/shm/.
    // .size(1024) reserva exactamente 1024 bytes de espacio.
    // .create() da la orden al Sistema Operativo de asignar el bloque de RAM.
    // .expect(...) lanza un error si la memoria ya existe o no hay permisos.
    let mut shmem: Shmem = ShmemConf::new()
        .os_id(memoria)
        .size(1024)
        .create()
        .expect("Error al crear la memoria compartida");

    // shmem.as_ptr() obtiene la dirección base y 'as usize' la convierte a un número entero.
    let direccion = shmem.as_ptr() as usize;

    // Imprime la dirección de memoria donde el SO mapeó el segmento para este proceso.
    println!("\n[Escritor] Memoria compartida creada con éxito en {:?}", direccion);

    // Definimos el mensaje como un arreglo de bytes (u8) de longitud fija (16 bytes).
    // El prefijo 'b' convierte el string directamente en bytes.
    let mensaje: &[u8; 16] = b"Hola desde Rust!";

    // 2. Acceso Mutable a la Memoria
    // shmem.as_slice_mut() es un método de la librería que convierte el puntero crudo 
    // en un slice de Rust (&mut [u8]) sobre el cual podemos operar de forma segura.
    // Es 'unsafe' porque el compilador no puede evitar que otro proceso externo toque esto.
    let data: &mut [u8] = unsafe { shmem.as_slice_mut() };

    // 3. Escritura de Datos
    // Tomamos un rango del slice 'data' desde el inicio hasta la longitud del mensaje.
    // .copy_from_slice(mensaje) copia los bytes de forma eficiente a la RAM compartida.
    data[..mensaje.len()].copy_from_slice(mensaje);

    // Confirmamos el nombre del recurso donde escribimos.
    println!("[Escritor] Mensaje escrito en la memoria compartida: {}", memoria);

    // Pausamos el hilo durante 10 segundos. 
    // Es vital: si el programa termina aquí, el objeto 'shmem' se destruye y la memoria desaparece.
    std::thread::sleep(std::time::Duration::from_secs(10));
}
