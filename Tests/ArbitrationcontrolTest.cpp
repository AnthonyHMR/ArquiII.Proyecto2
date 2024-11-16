#include <iostream>
#include <vector>
#include "InstructionMemory.h"
#include "ArbitrationControl.h"

int main() {
    // Crear memorias de instrucciones para 4 PEs
    std::vector<InstructionMemory> instructionMemories;
    for (size_t i = 0; i < 4; ++i) {
        InstructionMemory mem(i);
        // Instrucciones de ejemplo para cada PE
        mem.addInstruction("LOAD A, 10");
        mem.addInstruction("ADD A, B");
        mem.addInstruction("STORE A, 20");
        instructionMemories.push_back(mem);
    }

    // Crear un controlador de arbitraje y ejecutar las instrucciones
    ArbitrationControl arbitrationControl(instructionMemories);
    arbitrationControl.executeInstructions();

    return 0;
}
