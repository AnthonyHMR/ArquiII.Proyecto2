#ifndef ROUNDROBINSCHEME_H
#define ROUNDROBINSCHEME_H

#include <vector>
#include "InstructionMemory.h"
#include "ProcessingElement.h"

class RoundRobinScheme {
public:
    // Constructor
    RoundRobinScheme(const std::vector<InstructionMemory>& instructionMemories, const std::vector<ProcessingElement*>& pes);

    // Ejecutar instrucciones con arbitraje Round-Robin
    void executeInstructions();

private:
    std::vector<InstructionMemory> instructionMemories;
    std::vector<ProcessingElement*> pes;
};

#endif // ROUNDROBINSCHEME_H
