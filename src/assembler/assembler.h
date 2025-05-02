#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>

class Memory;

class Assembler {
public:
    explicit Assembler(Memory *mem);
    ~Assembler();

    // Load an assembly file, assemble it, and write it into memory
    void loadProgram(const std::string& filename);

private:
    // Map mnemonics to 6‐bit opcodes
    std::unordered_map<std::string, int> opcodeMap = {
        {"IN",     0b000000},
        {"OUT",    0b000001},
        {"ILOAD",  0b000010},
        {"FLOAD",  0b000011},
        {"SLOAD",  0b000100},
        {"ISTORE", 0b000101},
        {"FSTORE", 0b000110},
        {"SSTORE", 0b000111},
        {"CALL",   0b001000},
        {"RET",    0b001001},
        {"CMP",    0b001010},
        {"JMP",    0b001011},
        {"JE",     0b001100},
        {"JNE",    0b001101},
        {"JG",     0b001110},
        {"JL",     0b001111},
        {"JZ",     0b010000},
        {"ADD",    0b010001},
        {"FADD",   0b010010},
        {"SUB",    0b010011},
        {"FSUB",   0b010100},
        {"MUL",    0b010011},
        {"FMUL",   0b010100},
        {"DIV",    0b010101},
        {"FDIV",   0b010110},
        {"MOD",    0b011001},
        {"LSHIFT", 0b011010},
        {"RSHIFT", 0b011011},
        {"SETBIT", 0b011100},
        {"CLRIT",  0b011101},
        {"GETBIT", 0b011110},
        {"POPCNT", 0b011111},
        {"AND",    0b100000},
        {"OR",     0b100001},
        {"NOT",    0b100010},
        {"HLT",    0b111111}
    };

    std::unordered_map<std::string, int> labelMap; // Label to address map

    Memory *mem;

    // Assemble one instruction line into a 64-bit word
    uint64_t assembleInstrxn(const std::string& instrxnLine);

    // Assemble a list of lines into words
    std::vector<uint64_t> assembleProgram(const std::vector<std::string> &programLines);

    // Write the assembled words into memory at successive addresses
    void writeProgramToMemory(const std::vector<uint64_t> &program);
};
