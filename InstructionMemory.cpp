#include "InstructionMemory.h"

InstructionMemory::InstructionMemory(size_t peId) : peId(peId) {}

void InstructionMemory::addInstruction(const std::string& instruction) {
    instructions.push_back(instruction);
}

std::string InstructionMemory::getInstruction(size_t index) const {
    if (index < instructions.size()) {
        return instructions[index];
    }
    return "NOP"; // No Operation if index out of range
}

size_t InstructionMemory::getInstructionCount() const {
    return instructions.size();
}

size_t InstructionMemory::getPEId() const {
    return peId;
}
