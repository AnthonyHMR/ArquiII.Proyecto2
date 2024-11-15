// BusInterconnect.cpp
#include "BusInterconnect.h"

// Constructor
BusInterconnect::BusInterconnect(SharedMemory& sharedMemory, std::vector<Cache*>& caches)
    : sharedMemory(sharedMemory), connectedCaches(caches), readRequests(0), readResponses(0),
      writeRequests(0), writeResponses(0), invalidations(0), dataTransferred(0) {}

// Handle a transaction from a PE
void BusInterconnect::handleTransaction(const BusEvent& event) {
    switch (event.type) {
    case BusTransactionType::ReadRequest:
        ++readRequests;
        processReadRequest(event.sourcePE, event.address);
        break;
    case BusTransactionType::WriteRequest:
        ++writeRequests;
        processWriteRequest(event.sourcePE, event.address, event.data);
        break;
    default:
        break;
    }
}

// Process read request
void BusInterconnect::processReadRequest(size_t sourcePE, uint64_t address) {
    Cache* sourceCache;
    for (Cache* cache : connectedCaches) {
        if (cache->getPEId() == sourcePE) {
            sourceCache = cache;
            uint64_t data = cache->read(address);
            if (data != -1) { // If data is valid in cache
                ++readResponses;
                dataTransferred += sizeof(data);
                std::cout << "PE " << sourcePE << " read address " << address << ", data: " << data << " from cache." << std::endl;
                return;
            }
        }
    }

    // If not found in the PE's cache, read from shared memory
    int notify = notifyCaches(BusTransactionType::ReadResponse, sourcePE, address);
    uint64_t data = sharedMemory.read(address);
    ++readResponses;
    dataTransferred += sizeof(data);
    uint16_t writeBackAddressVal = -1;
    uint64_t writeBackDataVal = -1;
    uint16_t* writeBackAddress = &writeBackAddressVal;
    uint64_t* writeBackData = &writeBackDataVal;
    if(notify == 0) {
        sourceCache->write(address, data, MESIState::Exclusive, writeBackAddress, writeBackData);
    } else {
        sourceCache->write(address, data, MESIState::Shared, writeBackAddress, writeBackData);
    }
    if (*writeBackAddress != static_cast<uint16_t>(-1) && *writeBackData != static_cast<uint64_t>(-1)) {
        sharedMemory.write(*writeBackAddress, *writeBackData);
        std::cout << "PE " << sourcePE << " wroteback data " << *writeBackData << ", to address " << *writeBackData << " in shared memory." << std::endl;
    }
    std::cout << "PE " << sourcePE << " read address " << address << ", data: " << data << " from shared memory." << std::endl;
}

// Process write request
void BusInterconnect::processWriteRequest(size_t sourcePE, uint64_t address, uint64_t data) {
    for (Cache* cache : connectedCaches) {
        if (cache->getPEId() == sourcePE) {
            uint16_t writeBackAddressVal = -1;
            uint64_t writeBackDataVal = -1;
            uint16_t* writeBackAddress = &writeBackAddressVal;
            uint64_t* writeBackData = &writeBackDataVal;

            cache->write(address, data, MESIState::Modified, writeBackAddress, writeBackData);

            if (*writeBackAddress != static_cast<uint16_t>(-1) && *writeBackData != static_cast<uint64_t>(-1)) {
                sharedMemory.write(*writeBackAddress, *writeBackData);
                std::cout << "PE " << sourcePE << " wroteback data " << *writeBackData << ", to address " << *writeBackData << " in shared memory." << std::endl;
            }
            ++writeResponses;
            dataTransferred += sizeof(data);
            notifyCaches(BusTransactionType::WriteResponse, sourcePE, address);
            std::cout << "PE " << sourcePE << " wrote data " << data << " to address " << address << " in cache." << std::endl;
            return;
        }
    }
}

// Notify connected caches of a coherence action
int BusInterconnect::notifyCaches(BusTransactionType type, size_t sourcePE, uint64_t address) {
    int exclusive = 0;
    for (Cache* cache : connectedCaches) {
        if (cache->getPEId() != sourcePE) {
            int currentValue = cache->handleBusTransaction(type, address);
            if (currentValue == -1) {
                sharedMemory.write(address, cache->read(address));
                return -1;
            }
            exclusive += currentValue;
        }
    }
    return exclusive;
}

// Display bus statistics
void BusInterconnect::displayStatistics() const {
    std::cout << "Bus Statistics:" << std::endl;
    std::cout << "Read Requests: " << readRequests << std::endl;
    std::cout << "Read Responses: " << readResponses << std::endl;
    std::cout << "Write Requests: " << writeRequests << std::endl;
    std::cout << "Write Responses: " << writeResponses << std::endl;
    std::cout << "Invalidations: " << invalidations << std::endl;
    std::cout << "Data Transferred: " << dataTransferred << " bytes" << std::endl;
}