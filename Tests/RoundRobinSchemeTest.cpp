#include <iostream>
#include <vector>
#include "InstructionMemory.h"
#include "ProcessingElement.h"
#include "Cache.h"
#include "BusInterconnect.h"
#include "SharedMemory.h"
#include "RoundRobinScheme.h"


int main() {
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

    std::vector<ProcessingElement*> pes = {&pe1, &pe2, &pe3, &pe4};

    // Crear memorias de instrucciones para 4 PEs
    std::vector<InstructionMemory> instructionMemories;
    for (size_t i = 1; i < 5; ++i) {
        InstructionMemory mem(i);
        // Instrucciones de ejemplo para cada PE
        mem.addInstruction(instruction(InstructionType::LOAD, 0, 8));
        mem.addInstruction(instruction(InstructionType::INC, 0, 12));
        mem.addInstruction(instruction(InstructionType::STORE, 1, 4));
        instructionMemories.push_back(mem);
    }

    // Crear un controlador de arbitraje round-robin y ejecutar las instrucciones
    RoundRobinScheme roundRobinScheme(instructionMemories, pes);
    roundRobinScheme.executeInstructions();

    return 0;
}
