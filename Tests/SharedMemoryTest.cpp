// main.cpp
#include "../SharedMemory.h"

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

    // Display the full memory content (optional)
    sharedMem.displayMemory();

    return 0;
}