#ifndef INSTRUCTIONMEMORY_H
#define INSTRUCTIONMEMORY_H

#include <vector>
#include <string>

class InstructionMemory {
public:
    InstructionMemory(size_t peId);
    void addInstruction(const std::string& instruction);
    std::string getInstruction(size_t index) const;
    size_t getInstructionCount() const;
    size_t getPEId() const;

private:
    size_t peId;
    std::vector<std::string> instructions;
};

#endif // INSTRUCTIONMEMORY_H
