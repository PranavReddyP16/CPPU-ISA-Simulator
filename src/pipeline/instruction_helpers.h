#pragma once

#include <cstdint>

// All masks/shifts match your assembler:
//   opcode << 58,   reg1 <<55,   reg2 <<52,   reg3 <<49,
//   addr <<23,      len in bits 7–0

inline uint8_t extract_opcode(uint64_t inst) {
    return static_cast<uint8_t>((inst >> 58) & 0x3F);
}

inline uint8_t extract_rs1(uint64_t inst) {
    return static_cast<uint8_t>((inst >> 55) & 0x07);
}

inline uint8_t extract_rs2(uint64_t inst) {
    return static_cast<uint8_t>((inst >> 52) & 0x07);
}

inline uint8_t extract_rs3(uint64_t inst) {
    return static_cast<uint8_t>((inst >> 49) & 0x07);
}

// For loads/stores: 32‐bit address field in bits 54–23:
inline uint32_t extract_mem_address(uint64_t inst) {
    return static_cast<uint32_t>((inst >> 23) & 0xFFFFFFFFu);
}

// For ILOAD/SLOAD/etc (if you want imm length):
inline uint8_t extract_length(uint64_t inst) {
    return static_cast<uint8_t>(inst & 0xFF);
}
