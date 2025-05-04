
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
    halted = false;
    if_valid = id_valid = ex_valid = mem_valid = wb_valid = false;
}

void five_stage_pipeline::fetch() {
    // if (halted) { if_valid=false; return; }

    uint64_t inst = cache.read_data(regs.PC);
    ifr.instruction = inst;
    ifr.PC     = regs.PC;
    ifr.opcode = extract_opcode(inst);
    if_valid   = true;

    regs.PC += 1;
    if (ifr.opcode == OP::HLT_OP) halted = true;
}

/* ─── decode ───────────────────────────────────────────── */
void five_stage_pipeline::decode() {
    // if (!if_valid) { id_valid=false; return; }
    // idr = ifr; id_valid = true;

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
    // if (!id_valid){ ex_valid=false; return; }
    // exr = idr; ex_valid=true;

    int opc = exr.opcode;
    if(opc == OP::HLT_OP){
        //Stall
    }
    else if (opc == OP::LI_OP) {
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
    // if (!ex_valid){ mem_valid=false; return; }
    // memr = exr; mem_valid=true;

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
    wbr = memr; wb_valid=true;

    if (wbr.reg_write && wbr.rd < 8) {
        regs.write_register(wbr.rd, wbr.write_data);
        std::cout << ">> WB: GPR[" << int(wbr.rd) << "] = "
                  << wbr.write_data << std::endl;
    }
}

/* ─── one clock ───────────────────────────────────────── */
void five_stage_pipeline::clock_cycle() {
    // writeback();
    // mem_stage();
    // execute();
    // decode();
    // fetch();
    if (wb_valid) writeback();
    if (mem_valid) mem_stage();
    if (ex_valid) execute();
    if (id_valid) decode();
    if (!halted) fetch();


    bool hazard = false;

    if (exr.opcode!=OP::HLT_OP && ( (idr.rs1 == exr.rd && exr.rd != 0) || (idr.rs2 == exr.rd && exr.rd != 0) ))
        hazard = true;
    if (memr.opcode!=OP::HLT_OP && ( (idr.rs1 == memr.rd && memr.rd != 0) || (idr.rs2 == memr.rd && memr.rd != 0) ))
        hazard = true;
    if (hazard) {
        wb_valid  = mem_valid;
        mem_valid = ex_valid;
        ex_valid  = false;     
    } else {
        wb_valid  = mem_valid;
        mem_valid = ex_valid;
        ex_valid  = id_valid;
        id_valid  = if_valid;
        if_valid  = !halted;
    }

    if (hazard) {
        std::cout<<"HAZARD"<<std::endl;
        wbr = memr;
        memr = exr;
        exr.opcode =0b100010;
    } 
    else {
        std::cout<<"NO HAZARD"<<std::endl;
        wbr = memr;
        memr = exr;
        exr = idr;
        idr = ifr;
    }
}

void five_stage_pipeline::run_pipeline() {
    while(!(halted && !if_valid&& !id_valid&& !ex_valid&& !mem_valid&& !wb_valid))
        clock_cycle();
}
