#include "Compiler.h"
#include <sstream>
#include <iostream>
#include <regex>

Compiler::Compiler(size_t peId) : peId(peId) {}

void Compiler::compile(const std::string& input) {
    std::istringstream stream(input);
    std::string line;

    // Expresiones regulares para las instrucciones soportadas
    std::regex loadRegex(R"(LOAD\s+REG(\d+)\s+(\d+))");
    std::regex storeRegex(R"(STORE\s+REG(\d+)\s+(\d+))");
    std::regex incRegex(R"(INC\s+REG(\d+))");
    std::regex decRegex(R"(DEC\s+REG(\d+))");
    std::regex jnzRegex(R"(JNZ\s+\[(.+)\])");

    while (std::getline(stream, line)) {
        std::smatch match;

        if (std::regex_match(line, match, loadRegex)) {
            int reg = std::stoi(match[1].str());
            int addr = std::stoi(match[2].str());
            compiledInstructions.push_back(instruction(InstructionType::LOAD, reg, addr));
        } else if (std::regex_match(line, match, storeRegex)) {
            int reg = std::stoi(match[1].str());
            int addr = std::stoi(match[2].str());
            compiledInstructions.push_back(instruction(InstructionType::STORE, reg, addr));
        } else if (std::regex_match(line, match, incRegex)) {
            int reg = std::stoi(match[1].str());
            compiledInstructions.push_back(instruction(InstructionType::INC, reg, 0));
        } else if (std::regex_match(line, match, decRegex)) {
            int reg = std::stoi(match[1].str());
            compiledInstructions.push_back(instruction(InstructionType::DEC, reg, 0));
        } else if (std::regex_match(line, match, jnzRegex)) {
            std::string label = match[1].str();
            // JNZ instruction ...
        } else {
            // Manejo de casos no reconocidos
            std::cout << "Not recognize case" << std::endl;
        }
    }
}

size_t Compiler::getInstructionCount() const {
    return compiledInstructions.size();
}

instruction Compiler::getInstruction(size_t index) const {
    if (index < compiledInstructions.size()) {
        return compiledInstructions[index];
    }
}

std::vector<instruction> Compiler::getInstructionsVector() const {
    return compiledInstructions;
}

void Compiler::clearInstructions() {
    compiledInstructions.clear();
}
