/// ProcessingElement.h
#ifndef PROCESSING_ELEMENT_H
#define PROCESSING_ELEMENT_H

#include <vector>
#include <iostream>
#include "Cache.h"
#include "BusInterconnect.h"
#include "SharedMemory.h"

class ProcessingElement {
private:
    int id;
    uint64_t registers[4]; // REG0, REG1, REG2, REG3
    Cache* cache;
    BusInterconnect* bus;

public:
    // Constructor
    ProcessingElement(int pe_id, Cache* pe_cache, BusInterconnect* interconnect);

    // Method to execute a LOAD instruction
    void load(int reg_index, uint64_t addr);

    // Method to execute a STORE instruction
    void store(int reg_index, uint64_t addr);

    // Method to execute an INC instruction
    void increment(int reg_index);

    // Method to execute a DEC instruction
    void decrement(int reg_index);

    // Method to execute a JNZ instruction
    void jnz(int reg_index, int target_address);

    int getId() const;
};

#endif // PROCESSING_ELEMENT_H