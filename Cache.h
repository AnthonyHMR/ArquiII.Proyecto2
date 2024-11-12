#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cstdint>
#include <queue>
#include "SharedMemory.h" // Incluimos la clase de memoria compartida

struct CacheBlock {
    uint64_t data[4]; // Bloque de 32 bytes (4 valores de 64 bits)
    uint16_t tag; // Etiqueta para la dirección
    bool valid; // Bit de validez
    bool dirty; // Bit de modificación (escritura)
};

class Cache {
public:
    // Constructor
    Cache(SharedMemory& sharedMemory);

    // Métodos para operaciones de la caché
    uint64_t read(uint16_t address);
    void write(uint16_t address, uint64_t value);

    // Métodos para estadísticas
    int getCacheMisses() const;
    int getInvalidations() const;

private:
    static const int NUM_BLOCKS = 8; // Número de bloques de caché
    static const int BLOCK_SIZE = 32; // Tamaño de cada bloque en bytes

    std::vector<CacheBlock> cacheBlocks; // Vector de bloques de caché
    std::queue<int> fifoQueue; // Cola FIFO para política de reemplazo
    int cacheMisses; // Contador de cache misses
    int invalidations; // Contador de invalidaciones

    SharedMemory& sharedMemory; // Referencia a la memoria compartida

    // Métodos auxiliares
    int findBlock(uint16_t address); // Encuentra un bloque en caché
    void replaceBlock(uint16_t address, uint64_t value, bool isWrite); // Reemplaza un bloque según política FIFO
    void writeBack(int blockIndex); // Realiza write-back a la memoria compartida
};

#endif // CACHE_H
