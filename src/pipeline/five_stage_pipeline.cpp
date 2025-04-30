#include "five_stage_pipeline.h"
#include "system_constants.h"
#include "instruction_helpers.h"
#include "alu/ALU.h"
#include "memory/Register.h"
#include "memory/Memory.h"
#include "memory/Cache.h"
#include "memory/utils.h"
#include <iostream>
#include <unistd.h>

// Constructor: initialize pipeline registers and link cache
five_stage_pipeline::five_stage_pipeline(Cache& c)
  : cache(c),
    registers()
{
    // Initialize pipeline registers to NOP state
    ifr = {0, 0};                     // only two fields (PC, instruction)
    idr = {0, 0, 0, 0};
    exr = {0, 0, 0, 0, 0};
    memr= {0, 0, 0};
    wbr = {0, 0};
}

// ------------------- IF stage -------------------
void five_stage_pipeline::fetch() {
    sleep(1);
    std::cout<<"=================Fetching Stage======================"<<std::endl;
    // (printing ifr contents omitted for brevity)
    ifr.instruction = mem_dtype_to_int(cache.read_data(ifr.PC));
    if (ifr.instruction == 0b11111111) {
        halted = true;
        std::cout << "HALT instruction fetched — stopping fetch.\n";
    } else {
        ifr.PC += 1;
    }
}

// ------------------- ID stage -------------------
void five_stage_pipeline::decode() {
    sleep(1);
    std::cout<<"=================Decoding Stage======================"<<std::endl;
    // (printing idr contents omitted for brevity)
    idr.opcode = extract_opcode(idr.instruction);
    idr.rs1    = extract_rs1(idr.instruction);
    idr.rs2    = extract_rs2(idr.instruction);
    idr.rd     = extract_rd(idr.instruction);
    idr.imm    = (idr.opcode == 0b000101) ? extract_imm(idr.instruction) : 0;
}

// ------------------- EX stage -------------------
void five_stage_pipeline::execute() {
    sleep(1);
    std::cout<<"=================Executing Stage======================"<<std::endl;
    // (printing exr contents omitted for brevity)
    if (exr.opcode == 0b000101) {
        exr.mem_address = exr.rs1 + exr.imm;
    } else if (exr.opcode == 0b000000) {
        exr.mem_address = exr.rs1;
    } else if (exr.opcode == 0b000001) {
        exr.mem_address = exr.rs1;
        exr.write_data = exr.rs2;
    } else {
        exr.alu_res = ALU::execute(exr.opcode,
                                   registers.GPR[exr.rs1],
                                   registers.GPR[exr.rs2]);
    }
}

// ------------------- MEM stage -------------------
void five_stage_pipeline::mem_stage() {
    sleep(1);
    std::cout<<"=================Memory Stage======================"<<std::endl;
    // (printing memr contents omitted for brevity)
    if (memr.opcode == 0b000001) {
        memr.write_data = mem_dtype_to_int(cache.read_data(memr.mem_address));
    } else if (memr.opcode == 0b000000) {
        cache.write_data(memr.mem_address, int_to_mem_dtype(memr.rs2));
    } else {
        memr.write_data = memr.alu_res;
    }
}

// ------------------- WB stage -------------------
void five_stage_pipeline::writeback() {
    sleep(1);
    std::cout<<"=================WriteBack Stage======================"<<std::endl;
    if (wbr.opcode != -1) {
        registers.write_register(wbr.rd, wbr.write_data);
    }
}

// ------------------- Single clock tick -------------------
void five_stage_pipeline::clock_cycle() {
    // Advance in reverse order (WB→MEM→EX→ID→IF)
    if (wb_valid)  writeback();
    if (mem_valid) mem_stage();
    if (ex_valid)  execute();
    if (id_valid)  decode();
    if (!halted)   fetch();

    // Hazard detection & stall logic (unchanged)
    bool hazard = false;
    if (exr.opcode != -1 && ((idr.rs1 == exr.rd && exr.rd != 0) ||
                             (idr.rs2 == exr.rd && exr.rd != 0)))
        hazard = true;
    if (memr.opcode != -1 && ((idr.rs1 == memr.rd && memr.rd != 0) ||
                              (idr.rs2 == memr.rd && memr.rd != 0)))
        hazard = true;

    // Propagate validity bits with or without a stall
    wbr       = memr;
    memr      = exr;
    if (hazard) {
        ex_valid = false;      // insert bubble in EX
    } else {
        exr       = idr;
        idr       = ifr;
    }
    wb_valid  = mem_valid;
    mem_valid = ex_valid;
    ex_valid  = id_valid;
    id_valid  = if_valid;
    if_valid  = !halted;
}

// ------------------- Continuous run -------------------
void five_stage_pipeline::run_pipeline() {
    int cycle = 0;
    while ((if_valid || id_valid || ex_valid || mem_valid || wb_valid || !halted)
           && cycle < 20) {
        std::cout << "=== Clock Cycle: " << cycle << " ===\n";
        clock_cycle();
        // Touch first 16 bytes to exercise cache
        for (int i = 0; i < 16; ++i) {
            cache.read_data(i);
        }
        ++cycle;
        // GUI update now happens in MainWindow::refreshGui()
    }
}
