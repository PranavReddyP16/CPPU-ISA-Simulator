#include "assembler.h"
#include "memory/Memory.h"
#include <sstream>
#include <fstream>
#include <iostream>

Assembler::Assembler(Memory *mem) : mem(mem) {}
Assembler::~Assembler() = default;

uint64_t Assembler::assembleInstrxn(const std::string& instrxnLine) {
    std::istringstream iss(instrxnLine);
    std::string instrxn;
    iss >> instrxn;

    auto it = opcodeMap.find(instrxn);
    if (it == opcodeMap.end()) {
        std::cerr << "Unknown instruction: " << instrxn << std::endl;
        return 0;
    }

    uint64_t opcode = static_cast<uint64_t>(it->second);
    uint64_t encoded = opcode << 58; // 6 bits shifted to top

    if (instrxn == "IN" || instrxn == "OUT") {
        uint64_t addr, len;
        iss >> addr >> len;
        encoded |= (addr << 26);
        encoded |= (len & 0x3FFFFF);
    }
    else if (instrxn == "ILOAD" || instrxn == "FLOAD" || instrxn == "SLOAD" ||
             instrxn == "ISTORE"|| instrxn == "FSTORE"|| instrxn == "SSTORE") {
        uint64_t reg, addr, len;
        iss >> reg >> addr >> len;
        encoded |= (reg << 55);
        encoded |= (addr << 23);
        encoded |= (len & 0xFF);
    }
    else if (instrxn == "CALL"|| instrxn == "JMP"|| instrxn == "JE" ||
             instrxn == "JNE" ||instrxn == "JG"|| instrxn == "JL"||
             instrxn == "JZ") {
        uint64_t target;
        iss >> target;
        encoded |= (target << 26);
    }
    else if (instrxn == "CMP"|| instrxn == "ADD"|| instrxn == "FADD"||
             instrxn == "SUB"|| instrxn == "FSUB"|| instrxn == "MOD"||
             instrxn == "AND"|| instrxn == "OR") {
        uint64_t r1, r2;
        iss >> r1 >> r2;
        encoded |= (r1 << 55);
        encoded |= (r2 << 52);
    }
    else if (instrxn == "MUL"|| instrxn == "FMUL"|| instrxn == "DIV"|| instrxn == "FDIV") {
        uint64_t r1, r2, r3;
        iss >> r1 >> r2 >> r3;
        encoded |= (r1 << 55);
        encoded |= (r2 << 52);
        encoded |= (r3 << 49);
    }
    else if (instrxn == "LSHIFT"|| instrxn == "RSHIFT"||
             instrxn == "SETBIT"|| instrxn == "CLRIT") {
        uint64_t r1, amount, flag = 0;
        iss >> r1 >> amount;
        if (instrxn == "LSHIFT" || instrxn == "RSHIFT")
            iss >> flag;
        encoded |= (r1 << 55);
        encoded |= (amount << 47);
        encoded |= (flag << 46);
    }
    else if (instrxn == "GETBIT") {
        uint64_t r1, bit, r2;
        iss >> r1 >> bit >> r2;
        encoded |= (r1 << 55);
        encoded |= (bit << 47);
        encoded |= (r2 << 44);
    }
    else if (instrxn == "POPCNT" || instrxn == "NOT") {
        uint64_t r1, r2 = 0;
        iss >> r1;
        if (instrxn == "POPCNT") iss >> r2;
        encoded |= (r1 << 55);
        encoded |= (r2 << 52);
    }
    // RET and other single‐opcode cases fall through with just the opcode

    return encoded;
}

std::vector<uint64_t> Assembler::assembleProgram(const std::vector<std::string> &programLines) {
    int address = 0;
    for (auto &ln : programLines) {
        std::istringstream iss(ln);
        std::string token;
        iss >> token;

        // Check if the line is a label (ends with ':')
        if (token.back() == ':') {
            std::string label = token.substr(0, token.size() - 1); // Remove the ':'
            if (labelMap.find(label) != labelMap.end()) {
                std::cerr << "Duplicate label: " << label << std::endl;
                continue;
            }
            labelMap[label] = address; // Store the label and its address
        } else {
            // If not a label, it's an instruction
            ++address;
        }
    }

    std::vector<uint64_t> assembled;
    assembled.reserve(programLines.size());
    for (auto &ln : programLines) {
        if (ln.back() == ':')
            continue;
        assembled.push_back(assembleInstrxn(ln));
    }
    return assembled;
}

void Assembler::writeProgramToMemory(const std::vector<uint64_t> &program) {
    for (size_t i = 0; i < program.size(); ++i)
        mem->write_data(static_cast<int>(i), program[i]);
}

void Assembler::loadProgram(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != ';')
            lines.push_back(line);
    }

    auto prog = assembleProgram(lines);
    writeProgramToMemory(prog);
    std::cout << "Assembly complete: wrote " << prog.size() << " instructions.\n";
}
