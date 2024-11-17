#include <iostream>
#include <vector>
#include "InstructionMemory.h"
#include "ProcessingElement.h"
#include "Cache.h"
#include "BusInterconnect.h"
#include "SharedMemory.h"
#include "StaticPriorityScheme.h"

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

    // Crear instancias de InstructionMemory
    InstructionMemory mem1(1);
    mem1.addInstruction(instruction(InstructionType::LOAD, 0, 8));
    mem1.addInstruction(instruction(InstructionType::INC, 0, 12));
    mem1.addInstruction(instruction(InstructionType::STORE, 1, 4));

    InstructionMemory mem2(2);
    mem2.addInstruction(instruction(InstructionType::LOAD, 0, 8));
    mem2.addInstruction(instruction(InstructionType::INC, 0, 12));
    mem2.addInstruction(instruction(InstructionType::STORE, 1, 4));

    InstructionMemory mem3(3);
    mem3.addInstruction(instruction(InstructionType::LOAD, 0, 8));
    mem3.addInstruction(instruction(InstructionType::INC, 0, 12));
    mem3.addInstruction(instruction(InstructionType::STORE, 1, 4));

    InstructionMemory mem4(4);
    mem4.addInstruction(instruction(InstructionType::LOAD, 0, 8));
    mem4.addInstruction(instruction(InstructionType::INC, 0, 12));
    mem4.addInstruction(instruction(InstructionType::STORE, 1, 4));

    std::vector<InstructionMemory> instructionMemories = {mem1, mem2, mem3, mem4};
    std::vector<int> priorities = {3, 2, 4, 1}; // Asignar prioridades estáticas (3 es la mayor)

    // Crear una instancia de ArbitrationControl y ejecutar las instrucciones
    StaticPriorityScheme control(instructionMemories, priorities, pes);
    control.executeInstructions();

    return 0;
}
