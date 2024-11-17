#include "RoundRobinScheme.h"
#include <iostream>
#include <thread>
#include <chrono>

// Constructor
RoundRobinScheme::RoundRobinScheme(const std::vector<InstructionMemory>& instructionMemories, const std::vector<ProcessingElement*>& pes)
    : instructionMemories(instructionMemories), pes(pes) {}

// Ejecutar instrucciones usando Round-Robin
void RoundRobinScheme::executeInstructions() {
    size_t peCount = instructionMemories.size();
    std::vector<size_t> instructionIndices(peCount, 0); // Rastrea la posición de la instrucción para cada PE
    bool instructionsPending = true;

    while (instructionsPending) {
        instructionsPending = false;

        for (size_t i = 0; i < peCount; ++i) {
            InstructionMemory& currentInstMem  = instructionMemories[i];
            ProcessingElement& currentPE = *pes[i];
            size_t& currentIndex = instructionIndices[i];

            if (currentIndex < currentInstMem.getInstructionCount()) {
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
        }

        // Simular un tiempo de espera para el arbitraje Round-Robin (por ejemplo, 100 milisegundos)
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
