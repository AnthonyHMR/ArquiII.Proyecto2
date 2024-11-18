#include "StaticPriorityScheme.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <limits>

// Constructor
StaticPriorityScheme::StaticPriorityScheme(const std::vector<InstructionMemory>& instructionMemories, const std::vector<int>& priorities, const std::vector<ProcessingElement*>& pes)
    : instructionMemories(instructionMemories), priorities(priorities), pes(pes) {}

// Ejecutar instrucciones usando prioridad estática
void StaticPriorityScheme::executeInstructions() {
    size_t peCount = instructionMemories.size();
    std::vector<size_t> instructionIndices(peCount, 0); // Rastrea la posición de la instrucción para cada PE
    bool instructionsPending = true;

    while (instructionsPending) {
        instructionsPending = false;
        int highestPriorityIndex = -1;
        int highestPriorityValue = std::numeric_limits<int>::min();

        // Encontrar el PE con la mayor prioridad que aún tenga instrucciones pendientes
        for (size_t i = 0; i < peCount; ++i) {
            if (instructionIndices[i] < instructionMemories[i].getInstructionCount() && priorities[i] > highestPriorityValue) {
                highestPriorityIndex = static_cast<int>(i);
                highestPriorityValue = priorities[i];
            }
        }

        // Ejecutar la instrucción del PE con mayor prioridad
        if (highestPriorityIndex != -1) {
            InstructionMemory& currentInstMem = instructionMemories[highestPriorityIndex];
            ProcessingElement& currentPE = *pes[highestPriorityIndex];
            size_t& currentIndex = instructionIndices[highestPriorityIndex];

            // Obtener e imprimir la instrucción actual
            switch (currentInstMem.getInstruction(currentIndex).inst){
            case InstructionType::LOAD:
                std::cout << "PE " << currentInstMem.getPEId() << " ejecutando instruccion: " << "LOAD" << std::endl;
                currentPE.load(currentInstMem.getInstruction(currentIndex).reg, currentInstMem.getInstruction(currentIndex).address);
                break;
            case InstructionType::STORE:
                std::cout << "PE " << currentInstMem.getPEId() << " ejecutando instruccion: " << "STORE" << std::endl;
                currentPE.store(currentInstMem.getInstruction(currentIndex).reg, currentInstMem.getInstruction(currentIndex).address);
                break;
            case InstructionType::INC:
                std::cout << "PE " << currentInstMem.getPEId() << " ejecutando instruccion: " << "INC" << std::endl;
                currentPE.increment(currentInstMem.getInstruction(currentIndex).reg);
                break;
            case InstructionType::DEC:
                std::cout << "PE " << currentInstMem.getPEId() << " ejecutando instruccion: " << "DEC" << std::endl;
                currentPE.decrement(currentInstMem.getInstruction(currentIndex).reg);
                break;
            case InstructionType::JNZ:
                std::cout << "PE " << currentInstMem.getPEId() << " ejecutando instruccion: " << "JNZ" << std::endl;
                break;
            default:
                break;
            }
            std::cout << std::endl;

            ++currentIndex; // Avanzar a la siguiente instrucción de este PE
            instructionsPending = true; // Indica que aún hay instrucciones pendientes
        }

        // Simular un tiempo de espera para el arbitraje basado en prioridad (por ejemplo, 100 milisegundos)
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
