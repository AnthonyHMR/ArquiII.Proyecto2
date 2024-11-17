#include "InstructionMemory.h"

InstructionMemory::InstructionMemory(size_t peId) : peId(peId) {}

void InstructionMemory::addInstruction(const instruction& instruction) {
    instructions.push_back(instruction);
}

instruction InstructionMemory::getInstruction(size_t index) const {
    if (index < instructions.size()) {
        return instructions[index];
    }
}

size_t InstructionMemory::getInstructionCount() const {
    return instructions.size();
}

void InstructionMemory::addInstructionsVector(std::vector<instruction> instructionsVector) {
    instructions = instructionsVector;
}

size_t InstructionMemory::getPEId() const {
    return peId;
}
