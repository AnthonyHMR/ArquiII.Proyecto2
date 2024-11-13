// main.cpp
#include <iostream>
#include "Cache.h"

int main() {
    // Create a cache instance with 8 blocks
    Cache cache(8);

    // Simulate cache writes
    cache.write(0, 42);
    cache.write(4, 84);
    cache.write(8, 168);
    cache.write(12, 168);
    cache.write(16, 33);
    cache.write(20, 44);
    cache.write(24, 11);
    cache.write(28, 29);

    std::cout << "Value at address 0: " << cache.read(0) << std::endl;

    cache.write(32, 42);


    std::cout << "Value at address 0: " << cache.read(0) << std::endl;

    std::cout << "Value at address 4: " << cache.read(4) << std::endl;

    // Mostrar estadísticas de caché
    std::cout << "\nEstadisticas de la cache:" << std::endl;
    std::cout << "Cache misses: " << cache.getCacheMisses() << std::endl;
    std::cout << "Invalidaciones: " << cache.getInvalidations() << std::endl;


    // Display the cache state
    std::cout << "Cache 1" << std::endl;
    cache.displayCache();

    return 0;
}
