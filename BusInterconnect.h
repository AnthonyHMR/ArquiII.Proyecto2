// BusInterconnect.h
#ifndef BUSINTERCONNECT_H
#define BUSINTERCONNECT_H

#include <vector>
#include <cstdint>
#include <iostream>
#include "SharedMemory.h"
#include "Cache.h"

struct BusEvent {
    BusTransactionType type;
    size_t sourcePE;
    uint64_t address;
    uint64_t data;

    BusEvent(BusTransactionType type, size_t sourcePE, uint64_t address, uint64_t data = 0)
        : type(type), sourcePE(sourcePE), address(address), data(data) {}
};

class BusInterconnect {
public:
    // Constructor
    BusInterconnect(SharedMemory& sharedMemory, std::vector<Cache*>& caches);

    // Method to handle transactions from PEs
    void handleTransaction(const BusEvent& event);

    // Method to display bus statistics
    void displayStatistics() const;

private:
    SharedMemory& sharedMemory;
    std::vector<Cache*> connectedCaches;

    // Statistics
    size_t readRequests;
    size_t readResponses;
    size_t writeRequests;
    size_t writeResponses;
    size_t invalidations;
    size_t dataTransferred;

    // Helper methods
    void processReadRequest(size_t sourcePE, uint64_t address);
    void processWriteRequest(size_t sourcePE, uint64_t address, uint64_t data);

    // Notify caches of coherence actions
    int notifyCaches(BusTransactionType type, size_t sourcePE, uint64_t address);
};

#endif // BUSINTERCONNECT_H
