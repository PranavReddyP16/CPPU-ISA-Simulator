#include "five_stage_pipeline.h"
#include "instruction_helpers.h"
#include "memory/utils.h"
#include "alu/ALU.h"
#include <iostream>

using OP = Opcodes;

five_stage_pipeline::five_stage_pipeline(Cache &c)
  : cache(c)
{
    regs.reset();
}

void five_stage_pipeline::fetch() {
    if (halted) { if_valid=false; return; }
    if (!enabled) wb_valid=false; 

    uint64_t inst = cache.read_data(regs.PC);
    ifr.instruction = inst;
    ifr.PC     = regs.PC;
    ifr.opcode = extract_opcode(inst);
    if_valid   = true;

    regs.PC += 1;
    regs.IR = inst;
    if (ifr.opcode == OP::HLT_OP) halted = true;
}

/* ─── decode ───────────────────────────────────────────── */
void five_stage_pipeline::decode() {
    if (!if_valid) { id_valid=false; return; }
    idr = ifr; id_valid = true; if (!enabled) if_valid=false;

    uint64_t inst = idr.instruction;
    int opc       = idr.opcode;

    if (opc == OP::LI_OP) {
        idr.rd  = extract_rs1(inst);
        idr.imm = inst & 0x007FFFFFFFFFFFFFULL;
    }
    else if (opc == OP::ILOAD_OP) {
        idr.rd          = extract_rs1(inst);
        idr.mem_address = extract_mem_address(inst);
    }
    else if (opc == OP::ISTORE_OP) {
        idr.rs1         = extract_rs1(inst);
        idr.mem_address = extract_mem_address(inst);
    }
    else {                                   // ALU two-operand
        idr.rd  = extract_rs1(inst);         // rd encoded in bits 55-53
        idr.rs1 = idr.rd;                    // two-operand form: rs1 == rd
        idr.rs2 = extract_rs2(inst);         // source register
    }
}

/* ─── execute ──────────────────────────────────────────── */
void five_stage_pipeline::execute() {
    if (!id_valid){ ex_valid=false; return; }
    exr = idr; ex_valid=true; if (!enabled) id_valid=false;

    int opc = exr.opcode;

    if (opc == OP::LI_OP) {
        exr.write_data = exr.imm;
        exr.reg_write  = true;
    }
    else if (opc == OP::ADD_OP || opc == OP::SUB_OP ||
             opc == OP::MUL_OP || opc == OP::DIV_OP ||
             opc == OP::MOD_OP || opc == OP::AND_OP ||
             opc == OP::OR_OP  || opc == OP::NOT_OP) {

        int a = regs.read_register(exr.rs1);
        int b = regs.read_register(exr.rs2);
        exr.alu_res    = ALU::execute(opc,a,b);
        exr.write_data = exr.alu_res;
        exr.reg_write  = true;
    }
    else if (opc == OP::ISTORE_OP) {
        exr.write_data = regs.read_register(exr.rs1);
        exr.mem_write  = true;
        exr.reg_write  = false;
    }
}

/* ─── memory ───────────────────────────────────────────── */
void five_stage_pipeline::mem_stage() {
    if (!ex_valid){ mem_valid=false; return; }
    memr = exr; mem_valid=true; if (!enabled) ex_valid=false;

    int opc = memr.opcode;
    if (opc == OP::ILOAD_OP) {
        auto v = cache.read_data(memr.mem_address);
        memr.write_data = int(mem_dtype_to_int(v));
        memr.reg_write  = true;
    }
    else if (opc == OP::ISTORE_OP) {
        cache.write_data(memr.mem_address,
                         int_to_mem_dtype(memr.write_data));
        memr.reg_write = false;
    }
}

/* ─── write back ──────────────────────────────────────── */
void five_stage_pipeline::writeback() {
    if (!mem_valid){ wb_valid=false; return; }
    wbr = memr; wb_valid=true; if (!enabled) mem_valid=false;

    if (wbr.reg_write && wbr.rd < 8) {
        regs.write_register(wbr.rd, wbr.write_data);
        std::cout << ">> WB: GPR[" << int(wbr.rd) << "] = "
                  << wbr.write_data << std::endl;
    }
}

/* ─── one clock ───────────────────────────────────────── */
void five_stage_pipeline::clock_cycle() {
    if (enabled) {
        writeback();
        mem_stage();
        execute();
        decode();
        fetch();
    } else {
        std::cout << "TERI MAA" << std::endl;
        if (mem_valid) writeback();
        else if (ex_valid) mem_stage();
        else if (id_valid) execute();
        else if (if_valid) decode();
        else if (!halted) fetch();
    }
}

/* run to completion (used by unit tests) */
void five_stage_pipeline::run_pipeline() {
    while(!(halted && !if_valid&& !id_valid&& !ex_valid&& !mem_valid&& !wb_valid))
        clock_cycle();
}
