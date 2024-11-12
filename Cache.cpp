#include "Cache.h"
#include <stdexcept>

// Constructor que inicializa la referencia a la memoria compartida
Cache::Cache(SharedMemory& sharedMemory) : sharedMemory(sharedMemory), cacheBlocks(NUM_BLOCKS), cacheMisses(0), invalidations(0) {
    for (auto &block : cacheBlocks) {
        block.valid = false;
        block.dirty = false;
    }
}

// Método para leer un dato desde la caché
uint64_t Cache::read(uint16_t address) {
    int blockIndex = findBlock(address);

    if (blockIndex == -1) { // Miss de caché
        ++cacheMisses;
        return sharedMemory.read(address); // Política read-through
    }

    return cacheBlocks[blockIndex].data[address % 4]; // Selección de palabra
}

// Método para escribir un dato en la caché
void Cache::write(uint16_t address, uint64_t value) {
    int blockIndex = findBlock(address);

    if (blockIndex == -1) { // Miss de caché
        ++cacheMisses;
        replaceBlock(address, value, true); // Política write-allocate
    } else {
        // Escribe el valor y marca como modificado (write-back)
        cacheBlocks[blockIndex].data[address % 4] = value;
        cacheBlocks[blockIndex].dirty = true;
    }
}

// Devuelve el número de cache misses
int Cache::getCacheMisses() const {
    return cacheMisses;
}

// Devuelve el número de invalidaciones
int Cache::getInvalidations() const {
    return invalidations;
}

// Encuentra un bloque en la caché
int Cache::findBlock(uint16_t address) {
    uint16_t tag = address / 4; // Cálculo simplificado de la etiqueta
    for (int i = 0; i < NUM_BLOCKS; ++i) {
        if (cacheBlocks[i].valid && cacheBlocks[i].tag == tag) {
            return i; // Bloque encontrado
        }
    }
    return -1; // Miss de caché
}

// Reemplaza un bloque utilizando la política FIFO
void Cache::replaceBlock(uint16_t address, uint64_t value, bool isWrite) {
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
    if (cacheBlocks[replaceIndex].dirty) {
        writeBack(replaceIndex);
    }

    // Reemplaza el bloque con la nueva dirección y valor
    cacheBlocks[replaceIndex].valid = true;
    cacheBlocks[replaceIndex].dirty = isWrite;
    cacheBlocks[replaceIndex].tag = address / 4; // Etiqueta simplificada
    cacheBlocks[replaceIndex].data[address % 4] = value; // Almacena el valor si es escritura
}

// Realiza write-back al sistema de memoria compartida
void Cache::writeBack(int blockIndex) {
    if (!cacheBlocks[blockIndex].valid) return; // No hacer nada si el bloque no es válido

    uint16_t baseAddress = cacheBlocks[blockIndex].tag * 4; // Calcular dirección base del bloque
    for (int i = 0; i < 4; ++i) {
        sharedMemory.write(baseAddress + i, cacheBlocks[blockIndex].data[i]);
    }

    // Marcar el bloque como limpio después del write-back
    cacheBlocks[blockIndex].dirty = false;
}
