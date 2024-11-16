#include "ArbitrationControl.h"
#include <iostream>
#include <thread>
#include <chrono>

// Constructor
ArbitrationControl::ArbitrationControl(const std::vector<InstructionMemory>& instructionMemories)
    : instructionMemories(instructionMemories) {}

// Ejecutar instrucciones usando Round-Robin
void ArbitrationControl::executeInstructions() {
    size_t peCount = instructionMemories.size();
    std::vector<size_t> instructionIndices(peCount, 0); // Rastrea la posición de la instrucción para cada PE
    bool instructionsPending = true;

    while (instructionsPending) {
        instructionsPending = false;

        for (size_t i = 0; i < peCount; ++i) {
            InstructionMemory& currentPE = instructionMemories[i];
            size_t& currentIndex = instructionIndices[i];

            if (currentIndex < currentPE.getInstructionCount()) {
                // Obtener e imprimir la instrucción actual
                std::string instruction = currentPE.getInstruction(currentIndex);
                std::cout << "PE " << currentPE.getPEId() << " ejecutando instruccion: " << instruction << std::endl;
                ++currentIndex; // Avanzar a la siguiente instrucción de este PE
                instructionsPending = true; // Indica que aún hay instrucciones pendientes
            }
        }

        // Simular un tiempo de espera para el arbitraje Round-Robin (por ejemplo, 100 milisegundos)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
