/// ProcessingElement.cpp
#include "ProcessingElement.h"

ProcessingElement::ProcessingElement(int pe_id, Cache* pe_cache, BusInterconnect* interconnect)
    : id(pe_id), cache(pe_cache), bus(interconnect) {
    // Initialize registers to zero
    for (int i = 0; i < 4; ++i) {
        registers[i] = 0;
    }
}

void ProcessingElement::load(int reg_index, uint64_t addr) {
    if (reg_index >= 0 && reg_index < 4) {
        bus->handleTransaction(BusEvent(BusTransactionType::ReadRequest, id, addr));
        registers[reg_index] = cache->read(addr);
        std::cout << "PE" << id << " LOAD REG" << reg_index << " <- [" << addr << "]\n";
    } else {
        std::cerr << "Invalid register index\n";
    }
}

void ProcessingElement::store(int reg_index, uint64_t addr) {
    if (reg_index >= 0 && reg_index < 4) {
        uint64_t data = registers[reg_index];
        bus->handleTransaction(BusEvent(BusTransactionType::WriteRequest, id, addr, data));
        std::cout << "PE" << id << " DATA: " << data << " STORE [" << addr << "] <- REG" << reg_index << "\n";
    } else {
        std::cerr << "Invalid register index\n";
    }
}

void ProcessingElement::increment(int reg_index) {
    if (reg_index >= 0 && reg_index < 4) {
        ++registers[reg_index];
        std::cout << "PE" << id << " INC REG" << reg_index << "\n";
    } else {
        std::cerr << "Invalid register index\n";
    }
}

void ProcessingElement::decrement(int reg_index) {
    if (reg_index >= 0 && reg_index < 4) {
        --registers[reg_index];
        std::cout << "PE" << id << " DEC REG" << reg_index << "\n";
    } else {
        std::cerr << "Invalid register index\n";
    }
}

void ProcessingElement::jnz(int reg_index, int target_address) {
    if (reg_index >= 0 && reg_index < 4) {
        if (registers[reg_index] != 0) {
            std::cout << "PE" << id << " JNZ REG" << reg_index << " Jumping to " << target_address << "\n";
            // Logic to change program counter or execution position goes here
        } else {
            std::cout << "PE" << id << " JNZ REG" << reg_index << " Condition not met\n";
        }
    } else {
        std::cerr << "Invalid register index\n";
    }
}

// Devuelve el id del PE
int ProcessingElement::getId() const {
    return id;
}