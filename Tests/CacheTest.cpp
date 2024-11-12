#include <iostream>
#include "../SharedMemory.h"
#include "../Cache.h"

int main() {
    // Crear una instancia de memoria compartida
    SharedMemory sharedMemory;

    // Crear una instancia de caché asociada a la memoria compartida
    Cache cache(sharedMemory);

    // Escribir algunos valores a través de la caché
    cache.write(0, 42); // Escribe 42 en la dirección 0
    cache.write(4, 84); // Escribe 84 en la dirección 4
    cache.write(8, 126); // Escribe 126 en la dirección 8
    cache.write(12, 168); // Escribe 168 en la dirección 12

    // Leer valores desde la caché
    std::cout << "Lectura desde la cache (direccion 0): " << cache.read(0) << std::endl;
    std::cout << "Lectura desde la cache (direccion 4): " << cache.read(4) << std::endl;

    // Forzar una operación de reemplazo para verificar el comportamiento de write-back
    cache.write(16, 210); // Escribe 210 en la dirección 16
    cache.write(20, 252); // Escribe 252 en la dirección 20
    cache.write(24, 294); // Escribe 294 en la dirección 24
    cache.write(28, 336); // Escribe 336 en la dirección 28
    cache.write(32, 378); // Escribe 378 en la dirección 32 - Esto forzará un reemplazo en la caché
    cache.write(0, 77); // Escribe 378 en la dirección 32 - Esto forzará un reemplazo en la caché

    // Leer valores desde la caché
    std::cout << "Lectura desde la cache (direccion 0): " << cache.read(0) << std::endl;
    std::cout << "Lectura desde la cache (direccion 4): " << cache.read(4) << std::endl;

    // Mostrar el contenido de la memoria compartida
    std::cout << "\nContenido de la memoria compartida:" << std::endl;
    for (int i = 0; i < 32; ++i) { // Mostrar solo las primeras 16 posiciones
        std::cout << "Memoria[" << i << "] = " << sharedMemory.read(i) << std::endl;
    }

    // Mostrar estadísticas de caché
    std::cout << "\nEstadisticas de la cache:" << std::endl;
    std::cout << "Cache misses: " << cache.getCacheMisses() << std::endl;
    std::cout << "Invalidaciones: " << cache.getInvalidations() << std::endl;

    return 0;
}
