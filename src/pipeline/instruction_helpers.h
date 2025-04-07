// instruction_helpers.h

#pragma once

// Functions for extracting parts of a 32-bit instruction
int extract_opcode(int instruction);
int extract_rs1(int instruction);
int extract_rs2(int instruction);
int extract_rd(int instruction);
int extract_memory_address(int instruction);
int extract_length(int instruction);
int extract_imm(int instruction);