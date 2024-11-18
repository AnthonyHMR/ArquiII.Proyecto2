// SharedMemory.cpp
#include "SharedMemory.h"
#include <stdexcept>

// Constructor
SharedMemory::SharedMemory(size_t size) : memory(size, 0) {}

// Read data from memory
uint64_t SharedMemory::read(size_t address) const {
    if (address >= memory.size()) {
        throw std::out_of_range("Address out of range");
    }
    return memory[address];
}

// Write data to memory
void SharedMemory::write(size_t address, uint64_t value) {
    if (address >= memory.size()) {
        throw std::out_of_range("Address out of range");
    }
    memory[address] = value;
}

// Display memory content (for debugging)
void SharedMemory::displayMemory() const {
    for (size_t i = 0; i < memory.size(); ++i) {
        std::cout << "Address [" << i << "]: " << memory[i] << std::endl;
    }
}

std::vector<uint64_t> SharedMemory::getWholeMemory() const {
    return memory;
}
