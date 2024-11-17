#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <vector>
#include <cstdint>
#include "InstructionMemory.h"

class Compiler {
public:
    Compiler(size_t peId);

    void compile(const std::string& input);
    instruction getInstruction(size_t index) const;
    std::vector<instruction> getInstructionsVector() const;
    size_t getInstructionCount() const;

    void clearInstructions();

private:
    size_t peId;
    std::vector<instruction> compiledInstructions;
};

#endif //COMPILER_H
