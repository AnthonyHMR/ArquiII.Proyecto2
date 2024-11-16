#ifndef ARBITRATIONCONTROL_H
#define ARBITRATIONCONTROL_H

#include <vector>
#include "InstructionMemory.h"

class ArbitrationControl {
public:
    // Constructor
    ArbitrationControl(const std::vector<InstructionMemory>& instructionMemories);

    // Ejecutar instrucciones con arbitraje Round-Robin
    void executeInstructions();

private:
    std::vector<InstructionMemory> instructionMemories;
};

#endif // ARBITRATIONCONTROL_H
