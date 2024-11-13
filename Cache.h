// Cache.h
#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cstdint>
#include <string>
#include <queue>
#include <iostream>

// Enum to represent the MESI states
enum class MESIState {
    Modified,
    Exclusive,
    Shared,
    Invalid
};

// Enum to represent the type of bus transaction
enum class BusTransactionType {
    ReadRequest,
    ReadResponse,
    WriteRequest,
    WriteResponse,
    Invalidate
};

// Structure to represent a cache block
struct CacheBlock {
    uint64_t data[4];
    uint16_t tag;
    MESIState state;

    CacheBlock() : tag(0), state(MESIState::Invalid) {}
};

class Cache {
public:
    // Constructor
    Cache(size_t numBlocks, int sourcePE);

    // Methods to handle cache operations
    int read(uint16_t address);
    void write(uint16_t address, uint64_t value,  MESIState selectedState);
    int handleBusTransaction(BusTransactionType type, uint64_t address);

    // Métodos para estadísticas
    int getCacheMisses() const;
    int getInvalidations() const;
    int getPEId() const;

    // Method to display the cache state (for debugging)
    void displayCache() const;

    // Method to find de blockcache index
    int findBlockIndex(uint16_t address) const;

private:
    static const int NUM_BLOCKS = 8; // Número de bloques de caché
    static const int BLOCK_SIZE = 32; // Tamaño de cada bloque en bytes

    std::vector<CacheBlock> blocks;
    std::queue<int> fifoQueue; // Cola FIFO para política de reemplazo
    int PEId; // Id del PE
    int cacheMisses; // Contador de cache misses
    int invalidations; // Contador de invalidaciones

    // Helper methods
    void allocateBlock(uint16_t address, uint64_t value, MESIState selectedState);
};

#endif // CACHE_H