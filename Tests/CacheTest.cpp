// main.cpp
#include <array>
#include <iostream>
#include "Cache.h"

int main() {
    // Create a cache instance with 8 blocks
    Cache cache(8, 1);

    uint16_t writeBackAddressVal = -1;
    uint64_t writeBackDataVal[4] = {0};

    // Simulate cache writes
    cache.allocateBlock(0, {42, 0, 0, 0}, MESIState::Modified, &writeBackAddressVal, &writeBackDataVal);
    cache.allocateBlock(4, {84, 0, 0, 0}, MESIState::Modified, &writeBackAddressVal, &writeBackDataVal);
    cache.allocateBlock(8, {168, 0, 0, 0}, MESIState::Modified, &writeBackAddressVal, &writeBackDataVal);
    cache.allocateBlock(12, {168, 0, 0, 0}, MESIState::Modified, &writeBackAddressVal, &writeBackDataVal);
    cache.allocateBlock(16, {33, 0, 0, 0}, MESIState::Modified, &writeBackAddressVal, &writeBackDataVal);
    cache.allocateBlock(20, {44, 0, 0, 0}, MESIState::Modified, &writeBackAddressVal, &writeBackDataVal);
    cache.allocateBlock(24, {11, 0, 0, 0}, MESIState::Modified, &writeBackAddressVal, &writeBackDataVal);
    cache.allocateBlock(28, {29, 0, 0, 0}, MESIState::Modified, &writeBackAddressVal, &writeBackDataVal);

    std::cout << "Value at address 0: " << cache.read(0) << std::endl;

    cache.write(5, 82, MESIState::Modified);
    cache.allocateBlock(36, {50, 0, 0, 0}, MESIState::Modified, &writeBackAddressVal, &writeBackDataVal);
    std::cout << "WriteBack Data: ";
    for (const auto& data : writeBackDataVal) {
        std::cout << data << " ";
    }
    std::cout << std::endl;

    cache.allocateBlock(34, {0, 0, 42, 0}, MESIState::Modified, &writeBackAddressVal, &writeBackDataVal);
    std::cout << " WriteBack Data: ";
    for (const auto& data : writeBackDataVal) {
        std::cout << data << " ";
    }
    std::cout << std::endl;

    std::cout << "Value at address 0: " << cache.read(0) << std::endl;

    std::cout << "Value at address 4: " << cache.read(4) << std::endl;
    std::cout << "Value at address 5: " << cache.read(5) << std::endl;


    std::cout << "Value at address 34: " << cache.read(34) << std::endl;

    // Mostrar estadísticas de caché
    std::cout << "\nEstadisticas de la cache:" << std::endl;
    std::cout << "PEId: " << cache.getPEId() << std::endl;
    std::cout << "Cache misses: " << cache.getCacheMisses() << std::endl;
    std::cout << "Invalidaciones: " << cache.getInvalidations() << std::endl;
    std::cout << "Data Transfered: " << cache.getDataTransferred() << " bytes" << std::endl;

    // Display the cache state
    std::cout << "Cache 1" << std::endl;
    cache.displayCache();

    uint64_t* blockCache = cache.readBlock(34);
    std::cout << "Writing Block Data: ";
    for (int i = 0; i < 4; ++i) {
        std::cout << blockCache[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
