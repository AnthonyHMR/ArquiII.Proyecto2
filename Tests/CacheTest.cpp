// main.cpp
#include <iostream>
#include "Cache.h"

int main() {
    // Create a cache instance with 8 blocks
    Cache cache(8, 1);

    // Simulate cache writes
    cache.write(0, 42, MESIState::Modified);
    cache.write(4, 84, MESIState::Modified);
    cache.write(8, 168, MESIState::Exclusive);
    cache.write(12, 168, MESIState::Exclusive);
    cache.write(16, 33, MESIState::Exclusive);
    cache.write(20, 44, MESIState::Exclusive);
    cache.write(24, 11, MESIState::Exclusive);
    cache.write(28, 29, MESIState::Exclusive);

    std::cout << "Value at address 0: " << cache.read(0) << std::endl;

    cache.write(4, 42, MESIState::Shared);
    cache.write(32, 42, MESIState::Exclusive);


    std::cout << "Value at address 0: " << cache.read(0) << std::endl;

    std::cout << "Value at address 4: " << cache.read(4) << std::endl;

    // Mostrar estadísticas de caché
    std::cout << "\nEstadisticas de la cache:" << std::endl;
    std::cout << "PEId: " << cache.getPEId() << std::endl;
    std::cout << "Cache misses: " << cache.getCacheMisses() << std::endl;
    std::cout << "Invalidaciones: " << cache.getInvalidations() << std::endl;


    // Display the cache state
    std::cout << "Cache 1" << std::endl;
    cache.displayCache();

    return 0;
}
