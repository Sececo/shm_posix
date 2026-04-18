use shared_memory::{ShmemConf, Shmem};
fn main (){
    
	let memoria = "/Mi_memoria_compartida";
    let mut shmem: Shmem = ShmemConf::new()
        .os_id(memoria)
        .size(1024)
        .create()
        .expect("Error al crear la memoria compartida");

	let direccion = shmem.as_ptr() as usize;

    println!("\n[Escritor] Memoria compartida creada con éxito en {:?}", direccion);
    let mensaje: &[u8; 16] = b"Hola desde Rust!";
    
        let data: &mut [u8] = unsafe { shmem.as_slice_mut() };
        data[..mensaje.len()].copy_from_slice(mensaje);

        println!("[Escritor] Mensaje escrito en la memoria compartida: {}", memoria);
        std::thread::sleep(std::time::Duration::from_secs(10));
}
