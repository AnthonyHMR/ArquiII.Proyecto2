// Cache.cpp
#include "Cache.h"
#include <stdexcept>

// Constructor
Cache::Cache(size_t numBlocks, int sourcePE) : blocks(numBlocks), PEId(sourcePE), cacheMisses(0), invalidations(0), dataTransferred(0) {}

// Read operation
int Cache::read(uint16_t address) {
    int blockIndex = findBlockIndex(address);

    if (blockIndex == -1 || blocks[blockIndex].state == MESIState::Invalid) {
        return -1;
    }
    return blocks[blockIndex].data[address % 4];
}

// Write operation
void Cache::write(uint16_t address, uint64_t value, MESIState selectedState, uint16_t* writeBackAddress, uint64_t* writeBackdata) {
    int blockIndex = findBlockIndex(address);
    dataTransferred += sizeof(value);

    if (blockIndex == -1) {
        ++cacheMisses;
        allocateBlock(address, value, selectedState, writeBackAddress, writeBackdata); // Allocate a block if needed
    }
    blocks[blockIndex].state = selectedState; // Update MESI state
    blocks[blockIndex].data[address % 4] = value; // Simulate writing data
}

// handleBusTransaction operation
int Cache::handleBusTransaction(BusTransactionType type, uint64_t address) {
    int blockIndex = findBlockIndex(address);

    if (blockIndex == -1 || blocks[blockIndex].state == MESIState::Invalid) {
        return 0;
    }
    switch (type) {
    case BusTransactionType::ReadResponse:
        if(blocks[blockIndex].state == MESIState::Modified){
            blocks[blockIndex].state = MESIState::Shared;
            return -1;
        }
        blocks[blockIndex].state = MESIState::Shared;
        break;
    case BusTransactionType::WriteResponse:
        blocks[blockIndex].state = MESIState::Invalid;
        ++invalidations;
        break;
    default:
        break;
    }

    return 1;
}

// Display cache state
void Cache::displayCache() const {
    for (size_t i = 0; i < blocks.size(); ++i) {
        std::cout << "Block " << i << " | Tag: " << blocks[i].tag << " | Data: " << *blocks[i].data << " | State: ";
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
void Cache::allocateBlock(uint16_t address, uint64_t value, MESIState selectedState, uint16_t* writeBackAddress, uint64_t* writeBackData) {
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
    if (blocks[replaceIndex].state == MESIState::Modified) {
        std::cout << "Hacer writeback" << std::endl;
        *writeBackAddress = blocks[replaceIndex].tag*4;
        *writeBackData = blocks[replaceIndex].data[address % 4];
    }

    // Reemplaza el bloque con la nueva dirección y valor
    blocks[replaceIndex].state = selectedState;
    blocks[replaceIndex].tag = address / 4; // Etiqueta simplificada
    blocks[replaceIndex].data[address % 4] = value; // Almacena el valor si es escritura
}

std::vector<CacheBlock> Cache::getCacheBlocks() const {
    return blocks;
}

// Devuelve el id del PE
int Cache::getPEId() const {
    return PEId;
}

// Devuelve el número de cache misses
int Cache::getCacheMisses() const {
    return cacheMisses;
}

// Devuelve el número de invalidaciones
int Cache::getInvalidations() const {
    return invalidations;
}

// Devuelve el número de datos transferidos
size_t Cache::getDataTransferred() const {
    return dataTransferred;
}