#include "Compiler.h"
#include <sstream>
#include <iostream>
#include <regex>
#include <unordered_map>

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
    std::regex labelRegex(R"((\w+):)");

    // Primero, identificar todas las etiquetas
    std::unordered_map<std::string, int> labels; // Mapa de etiquetas a líneas
    std::vector<std::string> lines;
    int lineNumber = 0;

    while (std::getline(stream, line)) {
        lines.push_back(line);
        std::smatch match;
        if (std::regex_match(line, match, labelRegex)) {
            labels[match[1].str()] = lineNumber;
        }
        lineNumber++;
    }

    // Ahora procesar las instrucciones
    bool skipMode = false; // Indica si debemos ignorar instrucciones
    int skipUntil = -1;    // Línea hasta la cual ignorar instrucciones

    for (int i = 0; i < lines.size(); ++i) {
        line = lines[i];
        std::smatch match;

        // Revisar si estamos en modo de ignorar instrucciones
        if (skipMode) {
            // Verificar si llegamos a la etiqueta de destino
            if (i == skipUntil) {
                skipMode = false; // Salir del modo de omisión
            } else {
                continue; // Ignorar esta línea
            }
        }

        if (std::regex_match(line, match, jnzRegex)){
            // Procesar JNZ
            std::string label = match[1].str();
            if (labels.find(label) != labels.end()) {
                skipMode = true;                // Activar modo de omisión
                skipUntil = labels[label];      // Línea hasta donde saltar
            }
        }else if (std::regex_match(line, match, loadRegex)) {
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
        } else if (std::regex_match(line, match, labelRegex)) {
            // Etiqueta: no hacer nada directamente
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
