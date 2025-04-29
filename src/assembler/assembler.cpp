#include "assembler.h"
#include <sstream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <memory/Memory.h>
#include <gui/mainwindow.h>
#include <qmessagebox.h>

Assembler::Assembler(Memory *mem) : mem(mem) { }
Assembler::~Assembler() { }

uint64_t Assembler::assembleInstrxn(const std::string& instrxnLine) {
    std::istringstream iss(instrxnLine);
    std::string instrxn;
    iss >> instrxn;

    auto it = opcodeMap.find(instrxn);
    if (it == opcodeMap.end())
        QMessageBox::critical(mainWindow, "Error", "Unknown instruction: " + QString::fromStdString(instrxn));
        // throw std::invalid_argument("Unknown instruction: " + instrxn);

    uint64_t opcode = static_cast<uint64_t>(it->second);
    uint64_t encoded = opcode << 58; // 6 bits shifted to the top

    if (instrxn == "IN" || instrxn == "OUT") {
        uint64_t addr, len;
        iss >> addr >> len;
        encoded |= (addr << 26);    // 32 bits for address
        encoded |= (len & 0x3FFFFF); // 22 bits for length
    } else if (instrxn == "ILOAD" || instrxn == "FLOAD" || instrxn == "SLOAD" ||
               instrxn == "ISTORE" || instrxn == "FSTORE" || instrxn == "SSTORE") {
        uint64_t reg, addr, len;
        iss >> reg >> addr >> len;
        encoded |= (reg << 55);     // 3 bits for register
        encoded |= (addr << 23);    // 32 bits for address
        encoded |= (len & 0xFF);    // 8 bits for length
    } else if (instrxn == "CALL" || instrxn == "JMP" || instrxn == "JE" ||
               instrxn == "JNE" || instrxn == "JG" || instrxn == "JL" || instrxn == "JZ") {
        uint64_t target;
        iss >> target;
        encoded |= (target << 26);   // 32 bits for target address
        // 22 bits unused
    } else if (instrxn == "CMP" || instrxn == "ADD" || instrxn == "FADD" ||
               instrxn == "SUB" || instrxn == "FSUB" || instrxn == "MOD" ||
               instrxn == "AND" || instrxn == "OR") {
        uint64_t r1, r2;
        iss >> r1 >> r2;
        encoded |= (r1 << 55);      // 3 bits for r1
        encoded |= (r2 << 52);      // 3 bits for r2
    } else if (instrxn == "MUL" || instrxn == "FMUL" || instrxn == "DIV" || instrxn == "FDIV") {
        uint64_t r1, r2, r3;
        iss >> r1 >> r2 >> r3;
        encoded |= (r1 << 55);
        encoded |= (r2 << 52);
        encoded |= (r3 << 49);
    } else if (instrxn == "LSHIFT" || instrxn == "RSHIFT" || instrxn == "SETBIT" || instrxn == "CLRIT") {
        uint64_t r1, amount, flag = 0;
        iss >> r1 >> amount;
        if (instrxn == "LSHIFT" || instrxn == "RSHIFT") {
            iss >> flag; // 1 bit flag for logical/arithmetic shift
        }
        encoded |= (r1 << 55);
        encoded |= (amount << 47);
        encoded |= (flag << 46);
    } else if (instrxn == "GETBIT") {
        uint64_t r1, bit, r2;
        iss >> r1 >> bit >> r2;
        encoded |= (r1 << 55);
        encoded |= (bit << 47);
        encoded |= (r2 << 44);
    } else if (instrxn == "POPCNT" || instrxn == "NOT") {
        uint64_t r1, r2 = 0;
        iss >> r1;
        if (instrxn == "POPCNT") {
            iss >> r2; // POPCNT needs second register
        }
        encoded |= (r1 << 55);
        encoded |= (r2 << 52);
    } else if (instrxn == "RET") {
        // Nothing more needed; all operands are unused
    } else
        QMessageBox::critical(mainWindow, "Error", "Unhandled instruction type: " + QString::fromStdString(instrxn));
        // throw std::invalid_argument("Unhandled instruction type: " + instrxn);

    return encoded;
}

std::vector<std::uint64_t> Assembler::assembleProgram(std::vector<std::string> &programLines) {
    std::vector<uint64_t> assembledProgram;
    for (const auto& instrxn : programLines)
        assembledProgram.push_back(assembleInstrxn(instrxn));
    
    return assembledProgram;
}

void Assembler::writeProgramToMemory(std::vector<std::uint64_t> &program) {
    for (int i = 0; i < program.size(); i++)
        mem->write_data(i, program[i]);
}

void Assembler::loadProgram(const std::string &filename) {
    std::ifstream file(filename);
    // if (!file.is_open())
    //     QMessageBox::critical(mainWindow, "Error", "Could not open file: " + QString::fromStdString(filename));

    std::vector<std::string> programLines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') // Ignore empty lines and comments
            programLines.push_back(line);
    }
    file.close();

    std::vector<uint64_t> assembledProgram = assembleProgram(programLines);
    writeProgramToMemory(assembledProgram);

    std::cout << mem->read_data(0) << std::endl;
}