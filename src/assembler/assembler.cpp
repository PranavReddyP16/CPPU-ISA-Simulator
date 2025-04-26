#include "assembler.h"
#include <sstream>
#include <cstdint>
#include <vector>

uint64_t assembleInstrxn(const std::string& instrxnLine) {
    std::istringstream iss(instrxnLine);
    std::string instrxn;
    iss >> instrxn;

    auto it = opcodeMap.find(instrxn);
    if (it == opcodeMap.end()) {
        throw std::invalid_argument("Unknown instruction: " + instrxn);
    }

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
    } else {
        throw std::invalid_argument("Unhandled instruction type: " + instrxn);
    }

    return encoded;
}

std::vector<int> assembleProgram(const std::vector<std::string>& program) {
    std::vector<int> assembledProgram;
    for (const auto& instrxn : program)
        assembledProgram.push_back(assembleInstrxn(instrxn));
    
    return assembledProgram;
}