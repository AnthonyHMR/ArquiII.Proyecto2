#include <iostream>
#include "../SharedMemory.h"

int main() {
    SharedMemory sharedMemory;

    try {
        // Escribir algunos valores en la memoria
        sharedMemory.write(0, 42);
        sharedMemory.write(1, 123456789);
        sharedMemory.write(255, 987654321);  // Última dirección válida

        // Leer los valores de la memoria
        std::cout << "Valor en la direccion 0: " << sharedMemory.read(0) << std::endl;
        std::cout << "Valor en la direccion 1: " << sharedMemory.read(1) << std::endl;
        std::cout << "Valor en la direccion 255: " << sharedMemory.read(255) << std::endl;

        // Intentar acceder a una dirección inválida
        std::cout << "Intentando leer de una direccion invalida (256)..." << std::endl;
        std::cout << "Valor en la direccion 256: " << sharedMemory.read(256) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
