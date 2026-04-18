use shared_memory::{ShmemConf};


fn main (){
    // 1. Intentamos abrir la memoria por su nombre
    let shmem = ShmemConf::new()
        .os_id("/Mi_memoria_compartida")
        .open()
        .expect("No se encontró la memoria de C. ¿Ya corriste el escritor?");
	
	let direccion = shmem.as_ptr() as usize;
	

    println!("[Lector] ¡Memoria encontrada! {:#?} %d\n", direccion);

    // 2. Leemos los bytes
    unsafe {
        let ptr = shmem.as_ptr();
        
        // Creamos una "vista" de 20 bytes para leer
        let slice = std::slice::from_raw_parts(ptr, 20);
        
        // Convertimos esos bytes a texto legible
        let mensaje = String::from_utf8_lossy(slice);
        
        println!("[Lector] El mensaje es: {}", mensaje);
    }
}
