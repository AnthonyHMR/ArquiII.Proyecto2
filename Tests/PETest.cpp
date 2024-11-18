/// main.cpp
#include "ProcessingElement.h"
#include "Cache.h"
#include "BusInterconnect.h"
#include "SharedMemory.h"

int main()
{
    SharedMemory sharedMem(256);

    Cache cache1(8, 1);
    Cache cache2(8, 2);
    Cache cache3(8, 3);
    Cache cache4(8, 4);

    std::vector<Cache*> caches = {&cache1, &cache2, &cache3, &cache4};
    BusInterconnect bus(sharedMem, caches);

    ProcessingElement pe1(1, &cache1, &bus);
    ProcessingElement pe2(2, &cache2, &bus);
    ProcessingElement pe3(3, &cache3, &bus);
    ProcessingElement pe4(4, &cache4, &bus);

    pe1.load(0, 4);
    pe2.load(0, 4);
    pe1.load(1, 8);
    pe3.load(0, 4);
    pe3.load(1, 16);

    pe2.increment(0);
    pe2.increment(0);
    pe2.store(0, 4);
    pe1.load(0, 4);
    pe2.load(0, 4);

    pe2.increment(1);
    pe2.store(1, 4);

    pe3.increment(2);
    pe3.increment(2);
    pe3.increment(2);
    pe3.store(2, 4);
    pe2.load(3, 4);

    // Display the cache state
    std::cout << "Cache 1" << std::endl;
    cache1.displayCache();
    std::cout << "Cache misses: " <<  cache1.getCacheMisses() << std::endl;
    std::cout << "Cache invalidations: " << cache1.getInvalidations() << std::endl;
    std::cout << "Data transferred: " << cache1.getDataTransferred() << " bytes" << std::endl;

    std::cout << "Cache 2" << std::endl;
    cache2.displayCache();
    std::cout << "Cache misses: " <<  cache2.getCacheMisses() << std::endl;
    std::cout << "Cache invalidations: " << cache2.getInvalidations() << std::endl;
    std::cout << "Data transferred: " << cache2.getDataTransferred() << " bytes" << std::endl;

    std::cout << "Cache 3" << std::endl;
    cache3.displayCache();
    std::cout << "Cache misses: " <<  cache3.getCacheMisses() << std::endl;
    std::cout << "Cache invalidations: " << cache3.getInvalidations() << std::endl;
    std::cout << "Data transferred: " << cache3.getDataTransferred() << " bytes" << std::endl;

    // Display bus statistics
    bus.displayStatistics();

    // Display the full memory content (optional)
    std::cout << "Shared Memory" << std::endl;
    sharedMem.displayMemory();

    return 0;
}