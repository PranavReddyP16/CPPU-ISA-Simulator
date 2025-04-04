#pragma once
#include "memory/Register.h"

struct if_id_reg{
    int instruction;
    int PC;
};

struct id_ex_reg{
    int opcode;
    int rs1,rs2,rd;
    int PC;
};

struct ex_mem_reg{
    int alu_res;
    int rd;
    bool mem_write;
    int mem_address;
};

struct mem_wb_reg{
    int write_data;
    int rd;
    bool reg_write;
};