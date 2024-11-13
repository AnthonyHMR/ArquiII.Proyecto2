// main.cpp
#include "BusInterconnect.h"
#include "SharedMemory.h"
#include "Cache.h"

int main() {
    SharedMemory sharedMem(256);

    Cache cache1(8, 1);
    Cache cache2(8, 2);
    Cache cache3(8, 3);
    Cache cache4(8, 4);

    std::vector<Cache*> caches = {&cache1, &cache2, &cache3, &cache4};
    BusInterconnect bus(sharedMem, caches);

    // Simulate bus transactions
    bus.handleTransaction(BusEvent(BusTransactionType::ReadRequest, 1, 4));
    bus.handleTransaction(BusEvent(BusTransactionType::ReadRequest, 2, 4));
    bus.handleTransaction(BusEvent(BusTransactionType::ReadRequest, 1, 8));
    bus.handleTransaction(BusEvent(BusTransactionType::ReadRequest, 3, 4));
    bus.handleTransaction(BusEvent(BusTransactionType::ReadRequest, 3, 16));
    bus.handleTransaction(BusEvent(BusTransactionType::WriteRequest, 2, 4, 1234));
    bus.handleTransaction(BusEvent(BusTransactionType::ReadRequest, 1, 4));
    bus.handleTransaction(BusEvent(BusTransactionType::WriteRequest, 1, 4, 124));
    bus.handleTransaction(BusEvent(BusTransactionType::WriteRequest, 3, 4, 12));
    //bus.handleTransaction(BusEvent(BusTransactionType::Invalidate, 3, 10));

    // Display the cache state
    std::cout << "Cache 1" << std::endl;
    cache1.displayCache();
    std::cout << "Cache misses: " <<  cache1.getCacheMisses() << std::endl;
    std::cout << "Cache invalidations: " << cache1.getInvalidations() << std::endl;

    std::cout << "Cache 2" << std::endl;
    cache2.displayCache();
    std::cout << "Cache misses: " <<  cache2.getCacheMisses() << std::endl;
    std::cout << "Cache invalidations: " << cache2.getInvalidations() << std::endl;

    std::cout << "Cache 3" << std::endl;
    cache3.displayCache();
    std::cout << "Cache misses: " <<  cache3.getCacheMisses() << std::endl;
    std::cout << "Cache invalidations: " << cache3.getInvalidations() << std::endl;

    // Display bus statistics
    bus.displayStatistics();

    // Display the full memory content (optional)
    std::cout << "Shared Memory" << std::endl;
    sharedMem.displayMemory();

    return 0;
}