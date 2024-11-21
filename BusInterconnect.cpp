// BusInterconnect.cpp
#include "BusInterconnect.h"

#include <array>

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
                cache->setDataTransferred(data);
                ++readResponses;
                dataTransferred += sizeof(data);
                std::cout << "PE " << sourcePE << " read address " << address << ", data: " << data << " from cache." << std::endl;
                return;
            }
        }
    }

    // If not found in the PE's cache, read from shared memory
    int notify = notifyCaches(BusTransactionType::ReadResponse, sourcePE, address);
    uint16_t blockMem = address/4;
    std::array<uint64_t, 4> blockData = sharedMemory.readBlockMem(blockMem);
    ++readResponses;
    dataTransferred += sizeof(blockData);
    uint16_t writeBackAddressVal = -1;
    uint64_t writeBackData[4] = {0};
    uint16_t* writeBackAddress = &writeBackAddressVal;
    if(notify == 0) {
        sourceCache->allocateBlock(address, blockData, MESIState::Exclusive, writeBackAddress, &writeBackData);
    } else {
        sourceCache->allocateBlock(address, blockData, MESIState::Shared, writeBackAddress, &writeBackData);
    }
    if (*writeBackAddress != static_cast<uint16_t>(-1)) {
        sharedMemory.writeBlockMem(*writeBackAddress, writeBackData);
        std::cout << "PE " << sourcePE << " wroteback data ";
        for (const auto& data : writeBackData) {
            std::cout << data << " ";
        }
        std::cout << ", to address " << writeBackData[0] << " in shared memory." << std::endl;
    }
    std::cout << "PE " << sourcePE << " read address " << address << ", data: " << blockData[0] << " from shared memory." << std::endl;
}

// Process write request
void BusInterconnect::processWriteRequest(size_t sourcePE, uint64_t address, uint64_t data) {
    Cache* sourceCache;
    for (Cache* cache : connectedCaches) {
        if (cache->getPEId() == sourcePE) {
            sourceCache = cache;
            uint64_t addr = cache->read(address);
            if (addr != -1) { // If data is valid in cache
                cache->write(address, data, MESIState::Modified);
                ++writeResponses;
                dataTransferred += sizeof(data);
                notifyCaches(BusTransactionType::WriteResponse, sourcePE, address);
                std::cout << "PE " << sourcePE << " wrote data " << data << " to address " << address << " in cache." << std::endl;
                return;
            }
        }
    }

    uint16_t writeBackAddressVal = -1;
    uint64_t writeBackData[4] = {0};
    uint16_t* writeBackAddress = &writeBackAddressVal;

    uint16_t blockMem = address/4;

    std::array<uint64_t, 4> blockData = sharedMemory.readBlockMem(blockMem);

    sourceCache->allocateBlock(address, blockData, MESIState::Modified, writeBackAddress, &writeBackData);
    sourceCache->write(address, data, MESIState::Modified);
    ++writeResponses;
    dataTransferred += sizeof(data);
    dataTransferred += sizeof(blockData);
    notifyCaches(BusTransactionType::WriteResponse, sourcePE, address);

    if (*writeBackAddress != static_cast<uint16_t>(-1)) {
        sharedMemory.writeBlockMem(*writeBackAddress, writeBackData);
        std::cout << "PE " << sourcePE << " wroteback data ";
        for (const auto& data : writeBackData) {
            std::cout << data << " ";
        }
        std::cout << ", to address " << writeBackData[0] << " in shared memory." << std::endl;
    }
}

// Notify connected caches of a coherence action
int BusInterconnect::notifyCaches(BusTransactionType type, size_t sourcePE, uint64_t address) {
    int exclusive = 0;
    for (Cache* cache : connectedCaches) {
        if (cache->getPEId() != sourcePE) {
            int currentValue = cache->handleBusTransaction(type, address);
            if (currentValue == -1) {
                uint64_t* blockCache = cache->readBlock(address);
                static uint64_t blockData[4];
                for (int i = 0; i < 4; ++i) {
                    blockData[i] = blockCache[i];
                }
                sharedMemory.writeBlockMem(address, blockData);
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

// Devuelve el número de invalidaciones
size_t BusInterconnect::getInvalidations() const {
    return invalidations;
}

// Devuelve el número de cache misses
size_t BusInterconnect::getReadRequests() const {
    return readRequests;
}

// Devuelve el número de cache misses
size_t BusInterconnect::getWriteRequests() const {
    return writeRequests;
}

// Devuelve el número de datos transferidos
size_t BusInterconnect::getDataTransfered() const {
    return dataTransferred;
}

void BusInterconnect::clear() {
    readRequests = 0;
    readResponses = 0;
    writeRequests = 0;
    writeResponses = 0;
    invalidations = 0;
    dataTransferred = 0;
}