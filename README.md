# Proyecto de Memoria Compartida (Interoperabilidad C & Rust)

Este repositorio contiene una implementación pedagógica de Comunicación entre Procesos (IPC) utilizando **Memoria Compartida POSIX**. Se demuestra cómo un programa escrito en **C** puede comunicarse con uno escrito en **Rust** (y viceversa) compartiendo el mismo segmento de memoria física en el Kernel.

---

## 📥 Guía de Inicio Rápido (Git y Comunes)

Si acabas de llegar a este repositorio, aquí tienes los comandos básicos para preparar tu entorno local:

### 1. Clonar el repositorio
Para traer el código a tu máquina:
```bash
git clone https://github.com/Sececo/shm_posix.git
cd nombre-del-repo
```

### 2. Gestión de ramas (Opcional)
Si quieres experimentar sin dañar el código principal:
```bash
git checkout -b mi-rama-de-pruebas  # Crea y entra a una nueva rama
```

---

## 🛠️ Requisitos Previos

Antes de compilar, asegúrate de tener instaladas las herramientas base:

* **GCC**: Compilador de C (estándar en Linux).
* **Rust & Cargo**: Entorno de ejecución de Rust.
* **Librería RT**: Necesaria en C para funciones de tiempo real/memoria compartida (`-lrt`).

---

## 📋 Estructura del Proyecto

| Archivo | Rol | Descripción |
| :--- | :--- | :--- |
| `shm_w_c.c` | **Escritor (C)** | Crea el objeto `/Mi_memoria_compartida`, define su tamaño y escribe un mensaje `'Hola desde C!'`. |
| `shm_r_rs.c` | **Lector (C)** | Abre el objeto existente en modo solo lectura y muestra los datos en consola. |
| `shm_w_r.rs` | **Escritor (Rust)** | Crea la memoria compartida `/Mi_memoria_compartida` usando el crate `shared_memory` y escribe un mensaje `'Hola desde Rust!'`. |
| `shm_r_r.rs` | **Lector (Rust)** | Lee los bytes de la memoria y los convierte en un String legible. |

---

## 🚀 Compilación y Ejecución

### Fase 1: Compilar los programas en C
Ejecuta los siguientes comandos en tu terminal:
```bash
gcc shm_w_c.c -o shm_w_c
gcc shm_r_c.c -o shm_r_c
```

### Fase 2: Configurar el proyecto de Rust
Asegúrate de que tu archivo `Cargo.toml` incluya la dependencia necesaria:
```toml
[dependencies]
shared_memory = "0.12"
```

### Fase 3: Pruebas Cruzadas
Lo más interesante es mezclar los lenguajes. Prueba estas combinaciones:

**Opción A (Escritor C -> Lector Rust):**
1. Ejecuta en c `./shm_w_c &`.
2. En otra terminal dentro de rs, ejecuta `cargo run --bin shm_r_r`.
3. En la terminal donde ejcutaste el paso 1 puedes ejecutar `./shm_r_c`, verás como el lector de C tambien puede acceder al shm
4. En otro terminal ejecuta `ls -l /dev/shm` y verifica que la memoria compartida se creó correctamente

**Opción B (Escritor Rust -> Lector C):**
1. Ejecuta en rs `cargo run --bin shm_w_r &`.
2. En otra terminal dentro de c, ejecuta `./shm_r_c`.
3. En la terminal donde ejcutaste el paso 1 puedes ejecutar `cargo run --bin shm_r_r`, verás como el lector de rust tambien puede acceder al shm
4. En otro terminal ejecuta `ls -l /dev/shm` y verifica que la memoria compartida se creó correctamente

---

## 🧠 Detalles Técnicos: ¿Cómo funciona?

### El concepto de `unsafe` en Rust
En los archivos de Rust verás bloques `unsafe { ... }`. Esto es necesario porque:
* Estamos accediendo a una dirección de memoria que el sistema operativo controla, no Rust.
* Manipulamos punteros crudos (`raw pointers`) para crear *slices* de bytes.
* Rust requiere que el programador confirme manualmente que el acceso a esa memoria externa es válido.



### Flujo de Memoria POSIX
1.  **`shm_open`**: Crea un descriptor de archivo en `/dev/shm/`.
2.  **`ftruncate`**: Establece el tamaño del segmento (ej. 1024 bytes).
3.  **`mmap`**: Mapea ese archivo a la memoria RAM del proceso.
4.  **`shm_unlink`**: Marca el archivo para ser borrado cuando todos los procesos se desconecten.

---

## ⚠️ Solución de Problemas
Si recibes un error tipo **"No se encontró la memoria"**:
* Asegúrate de ejecutar primero un **Escritor**.
* Verifica que el nombre `/Mi_memoria_compartida` sea idéntico en todos los archivos.
* Si el programa falla, el archivo puede quedar bloqueado; bórralo con: `rm /dev/shm/Mi_memoria_compartida`.

---
*Desarrollado como ejercicio de interoperabilidad de sistemas de bajo nivel.*
```
