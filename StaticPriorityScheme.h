#ifndef STATICPRIORITYSCHEME_H
#define STATICPRIORITYSCHEME_H

#include "InstructionMemory.h"
#include <vector>
#include "ProcessingElement.h"

class StaticPriorityScheme {
public:
    // Constructor que recibe los bloques de memoria de instrucciones y sus prioridades
    StaticPriorityScheme(const std::vector<InstructionMemory>& instructionMemories, const std::vector<int>& priorities, const std::vector<ProcessingElement*>& pes);

    // Ejecutar las instrucciones usando arbitraje basado en prioridad estática
    void executeInstructions();

private:
    std::vector<InstructionMemory> instructionMemories;
    std::vector<int> priorities;
    std::vector<ProcessingElement*> pes;
};

#endif //STATICPRIORITYSCHEME_H
