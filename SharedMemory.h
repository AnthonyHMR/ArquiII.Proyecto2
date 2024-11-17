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

    // Method to display the memory content (for debugging)
    void displayMemory() const;

private:
    std::vector<uint64_t> memory;
};

#endif // SHAREDMEMORY_H