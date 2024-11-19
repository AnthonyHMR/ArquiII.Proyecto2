// main.cpp
#include <array>

#include "SharedMemory.h"

int main() {
    // Create a shared memory instance with 256 positions
    SharedMemory sharedMem(256);

    // Write some values to memory
    sharedMem.write(0, 42);
    sharedMem.write(1, 100);
    sharedMem.write(255, 999);

    // Read and display values from memory
    std::cout << "Value at address 0: " << sharedMem.read(0) << std::endl;
    std::cout << "Value at address 1: " << sharedMem.read(1) << std::endl;
    std::cout << "Value at address 255: " << sharedMem.read(255) << std::endl;

    uint64_t blockData[4] = {0, 0, 72, 0};
    sharedMem.writeBlockMem(4, blockData);

    uint16_t blockAddr = 5/4;

    std::array<uint64_t, 4> blockMem = sharedMem.readBlockMem(blockAddr);

    // Display the full memory content (optional)
    sharedMem.displayMemory();

    std::cout << " WriteBack Data: ";
    for (const auto& data : blockMem) {
        std::cout << data << " ";
    }
    std::cout << std::endl;

    return 0;
}