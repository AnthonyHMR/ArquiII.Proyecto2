// Cache.cpp
#include "Cache.h"
#include <stdexcept>

// Constructor
Cache::Cache(size_t numBlocks) : blocks(numBlocks), cacheMisses(0), invalidations(0)  {}

// Read operation
uint64_t Cache::read(uint16_t address) {
    size_t blockIndex = findBlockIndex(address);

    if (blockIndex == -1) {
        ++cacheMisses;
        return 0;
    }

    return blocks[blockIndex].data[address % 4];
}

// Write operation
void Cache::write(uint16_t address, uint64_t value) {
    int blockIndex = findBlockIndex(address);

    if (blockIndex == -1) {
        ++cacheMisses;
        allocateBlock(address, value); // Allocate a block if needed
    }
    blocks[blockIndex].state = MESIState::Modified; // Update MESI state
    blocks[blockIndex].data[address % 4] = value; // Simulate writing data
}

// Display cache state
void Cache::displayCache() const {
    for (size_t i = 0; i < blocks.size(); ++i) {
        std::cout << "Block " << i << " | Tag: " << blocks[i].tag << " | State: ";
        switch (blocks[i].state) {
            case MESIState::Modified: std::cout << "Modified"; break;
            case MESIState::Exclusive: std::cout << "Exclusive"; break;
            case MESIState::Shared: std::cout << "Shared"; break;
            case MESIState::Invalid: std::cout << "Invalid"; break;
        }
        std::cout << std::endl;
    }
}

// Find the index of a block based on an address
int Cache::findBlockIndex(uint16_t address) const {
    uint16_t tag = address / 4;
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        if (blocks[i].tag == tag && blocks[i].state != MESIState::Invalid) {
            return i;
        }
    }
    return -1; // Indicate block not found
}

// Allocate a block for a given address
void Cache::allocateBlock(uint16_t address, uint64_t value) {
    int replaceIndex;
    if (fifoQueue.size() < NUM_BLOCKS) {
        replaceIndex = fifoQueue.size();
        fifoQueue.push(replaceIndex);
    } else {
        replaceIndex = fifoQueue.front();
        fifoQueue.pop();
        fifoQueue.push(replaceIndex);
    }

    // Si el bloque que se reemplaza está modificado (dirty), realizar write-back
    if (blocks[replaceIndex].state != MESIState::Invalid) {
        std::cout << "Hacer writeback" << std::endl;
    }

    // Reemplaza el bloque con la nueva dirección y valor
    blocks[replaceIndex].state = MESIState::Exclusive;
    blocks[replaceIndex].tag = address / 4; // Etiqueta simplificada
    blocks[replaceIndex].data[address % 4] = value; // Almacena el valor si es escritura
}

// Devuelve el número de cache misses
int Cache::getCacheMisses() const {
    return cacheMisses;
}

// Devuelve el número de invalidaciones
int Cache::getInvalidations() const {
    return invalidations;
}