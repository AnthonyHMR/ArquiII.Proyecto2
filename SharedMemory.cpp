#include "SharedMemory.h"
#include <stdexcept>

SharedMemory::SharedMemory() : memory(MEMORY_SIZE, 0) {}

// Metodo para leer de la memoria segun la direccion
uint64_t SharedMemory::read(uint16_t address) {
    if (address >= MEMORY_SIZE) {
        throw std::out_of_range("Address out of range");
    }

    std::lock_guard<std::mutex> lock(memoryMutex); // Bloqueo para acceso seguro
    return memory[address];
}

// Metodo para escribir en la memoria segun la direccion
void SharedMemory::write(uint16_t address, uint64_t value) {
    if (address >= MEMORY_SIZE) {
        throw std::out_of_range("Address out of range");
    }

    std::lock_guard<std::mutex> lock(memoryMutex); // Bloqueo para acceso seguro
    memory[address] = value;
}
