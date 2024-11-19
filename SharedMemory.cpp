// SharedMemory.cpp
#include "SharedMemory.h"

#include <array>
#include <stdexcept>

// Constructor
SharedMemory::SharedMemory(size_t size) : memory(size, 0) {}

// Read data from memory
uint64_t SharedMemory::read(size_t address) const {
    if (address >= memory.size()) {
        std::cout << "Address: " << address << std::endl;
        throw std::out_of_range("Address out of range");
    }
    return memory[address];
}

// Write data to memory
void SharedMemory::write(size_t address, uint64_t value) {
    if (address >= memory.size()) {
        std::cout << "Address: " << address << std::endl;
        throw std::out_of_range("Address out of range");
    }
    memory[address] = value;
}

std::array<uint64_t, 4> SharedMemory::readBlockMem(uint16_t blockNum) {
    std::array<uint64_t, 4> blockMem; // Array de 4 elementos
    for (int i = 0; i < 4; ++i) {
        blockMem[i] = read((blockNum*4)+i);
    }
    return blockMem;
}

void SharedMemory::writeBlockMem(uint16_t address, uint64_t (writeBackData)[4]) {
    if (address >= memory.size()) {
        std::cout << "Address: " << address << std::endl;
        throw std::out_of_range("Address out of range");
    }
    address = address/4;
    for (int i = 0; i < 4; ++i) {
        memory[(address*4)+i] = writeBackData[i];
    }
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

void SharedMemory::clear() {
    std::fill(memory.begin(), memory.end(), 0);
}

