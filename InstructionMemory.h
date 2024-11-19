#ifndef INSTRUCTIONMEMORY_H
#define INSTRUCTIONMEMORY_H

#include <vector>
#include <string>
#include <cstdint>

// Enum to represent the type of bus transaction
enum class InstructionType {
    LOAD,
    STORE,
    INC,
    DEC,
    JNZ
};

struct instruction {
    InstructionType inst;
    uint64_t reg;
    uint16_t address;

    instruction(InstructionType inst, uint64_t reg, uint16_t address)
        : inst(inst), reg(reg), address(address) {}
};

class InstructionMemory {
public:
    InstructionMemory(size_t peId);
    void addInstruction(const instruction& instruction);
    void addInstructionsVector(std::vector<instruction> instructionsVector);
    instruction getInstruction(size_t index) const;
    size_t getInstructionCount() const;
    size_t getPEId() const;
    void clear();

private:
    size_t peId;
    std::vector<instruction> instructions;
};

#endif // INSTRUCTIONMEMORY_H
