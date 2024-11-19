// SharedMemory.h
#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <vector>
#include <cstdint>
#include <iostream>

class SharedMemory {
public:
    // Constructor
    SharedMemory(size_t size = 256);

    // Methods to read and write data
    uint64_t read(size_t address) const;
    void write(size_t address, uint64_t value);
    std::array<uint64_t, 4> readBlockMem(uint16_t blockNum);
    void writeBlockMem(uint16_t address, uint64_t (writeBackData)[4]);

    // Method to display the memory content (for debugging)
    void displayMemory() const;
    void clear();

    std::vector<uint64_t> getWholeMemory() const;

private:
    std::vector<uint64_t> memory;
};

#endif // SHAREDMEMORY_H