#include "instruction_helpers.h"
#include <iostream>  // Include for printing

// Extract the 6-bit opcode (bits 26-31)
int extract_opcode(int instruction) {
    std::cout << "Instruction: " << instruction << std::endl;  // Add this line to print the full instruction
    // int opcode = (instruction >> 26) & 0x3F;
    int opcode = (instruction>>12) & 0xF;
    std::cout << "Opcode: " << opcode << std::endl;
    return opcode;
}


// Extract the 3-bit rs1 register (bits 21-23)
int extract_rs1(int instruction) {
    // int rs1 = (instruction >> 21) & 0x7;  // Extract register 1 (next 3 bits)
    int rs1 = (instruction>>8) & 0xF;
    std::cout << "RS1: " << rs1 << std::endl;
    return rs1;
}

// Extract the 3-bit rs2 register (bits 18-20)
int extract_rs2(int instruction) {
    // int rs2 = (instruction >> 18) & 0x7;  // Extract register 2 (next 3 bits)
    int rs2 = (instruction>>4) & 0xF;
    std::cout << "RS2: " << rs2 << std::endl;
    return rs2;
}

// Extract the 5-bit rd register (bits 13-17)
int extract_rd(int instruction) {
    // int rd = (instruction >> 13) & 0x1F; // Extract destination register (next 5 bits)
    int rd = (instruction) & 0xF;
    std::cout << "RD: " << rd << std::endl;
    return rd;
}

// Extract the 24-bit memory address (bits 8-31)
int extract_memory_address(int instruction) {
    int address = (instruction >> 8) & 0xFFFFFF;  // Extract memory address
    std::cout << "Memory Address: " << address << std::endl;
    return address;
}

// Extract the 8-bit length (bits 0-7)
int extract_length(int instruction) {
    int length = instruction & 0xFF;  // Extract the last 8 bits for length
    std::cout << "Length: " << length << std::endl;
    return length;
}

int extract_imm(int instruction) {
    return (instruction >> 6) & 0xFFFFFFFF;  // Extract bits 6-37 (32-bit imm)
}
