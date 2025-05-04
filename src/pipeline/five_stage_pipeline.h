#pragma once

#include "system_constants.h"
#include "pipeline_registers.h"
#include "memory/Register.h"
#include "memory/Cache.h"
#include "alu/ALU.h"
#include "instruction_helpers.h"
#include "memory/utils.h"
#include <iostream>

// All of your opcodes as named constants:
struct Opcodes {
    static constexpr int IN_OP     = 0b000000;
    static constexpr int OUT_OP    = 0b000001;
    static constexpr int ILOAD_OP  = 0b000010;
    static constexpr int FLOAD_OP  = 0b000011;
    static constexpr int SLOAD_OP  = 0b000100;
    static constexpr int ISTORE_OP = 0b000101;
    static constexpr int FSTORE_OP = 0b000110;
    static constexpr int SSTORE_OP = 0b000111;
    static constexpr int CALL_OP   = 0b001000;
    static constexpr int RET_OP    = 0b001001;
    static constexpr int CMP_OP    = 0b001010;
    static constexpr int JMP_OP    = 0b001011;
    static constexpr int JE_OP     = 0b001100;
    static constexpr int JNE_OP    = 0b001101;
    static constexpr int JG_OP     = 0b001110;
    static constexpr int JL_OP     = 0b001111;
    static constexpr int JZ_OP     = 0b010000;
    static constexpr int ADD_OP    = 0b010001;
    static constexpr int FADD_OP   = 0b010010;
    static constexpr int SUB_OP    = 0b010011;
    static constexpr int MUL_OP    = 0b010100;
    static constexpr int FMUL_OP   = 0b010100;
    static constexpr int DIV_OP    = 0b010101;
    static constexpr int FDIV_OP   = 0b010110;
    static constexpr int MOD_OP    = 0b011001;
    static constexpr int LSHIFT_OP = 0b011010;
    static constexpr int RSHIFT_OP = 0b011011;
    static constexpr int SETBIT_OP = 0b011100;
    static constexpr int CLRIT_OP  = 0b011101;
    static constexpr int GETBIT_OP = 0b011110;
    static constexpr int POPCNT_OP = 0b011111;
    static constexpr int AND_OP    = 0b100000;
    static constexpr int OR_OP     = 0b100001;
    static constexpr int NOT_OP    = 0b100010;
    static constexpr int LI_OP     = 0b100011;
    static constexpr int HLT_OP    = 0b111111;
};

class five_stage_pipeline {
public:
    Register        regs;      // PC, IR, GPRs, etc.
    Cache&          cache;     // reference

    bool            halted    = false;
    bool            enabled   = true;
    
    pipeline_reg    ifr, idr, exr, memr, wbr;
    bool            if_valid  = false;
    bool            id_valid  = false;
    bool            ex_valid  = false;
    bool            mem_valid = false;
    bool            wb_valid  = false;

    explicit five_stage_pipeline(Cache &c);

    void fetch();
    void decode();
    void execute();
    void mem_stage();
    void writeback();
    void clock_cycle();
    void run_pipeline();
};
