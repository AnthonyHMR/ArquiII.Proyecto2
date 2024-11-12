#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <cstdint>
#include <vector>
#include <mutex>

class SharedMemory {
public:
    SharedMemory();

    uint64_t read(uint16_t address); // Lee un dato de la memoria compartida
    void write(uint16_t address, uint64_t value); // Escribe un dato en la memoria compartida

private:
    static const int MEMORY_SIZE = 256; // Tamaño fijo de la memoria
    std::vector<uint64_t> memory; // Vector para simular la memoria de 256 posiciones de 64 bits
    std::mutex memoryMutex; // Mutex para proteger el acceso concurrente a la memoria
};

#endif // SHAREDMEMORY_H
