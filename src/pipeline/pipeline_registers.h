#pragma once
#include "../memory/Register.h"

struct pipeline_reg {
    int instruction;
    int PC;
    int opcode;
    int rs1, rs2, rd;
    bool mem_write;
    int mem_address;
    int imm;
    int write_data;
    bool reg_write;
    int alu_res;
};