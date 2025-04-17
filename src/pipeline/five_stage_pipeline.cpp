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


five_stage_pipeline::five_stage_pipeline(Cache& cache, MainWindow* mainWindow): cache(cache), mainWindow(mainWindow), registers(mainWindow) {
    ifr = {0, 0};  // PC = 0, instruction = 0
    ifr.PC = 3;
    idr = {0, 0, 0, 0};  // Default values for opcode, registers
    exr = {0,0,0,0,0};
    memr = {0, 0, 0}; 
    wbr = {0, 0};
}

void five_stage_pipeline::fetch() {
    sleep(1);
    mainWindow->pipelineTable->setItem(0, 0, new QTableWidgetItem("X"));
    mainWindow->pipelineTable->setItem(0, 1, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 2, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 3, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 4, new QTableWidgetItem(""));

    std::cout<<"=================Fetching Stage======================"<<std::endl;
    std::cout << "Fetching instruction at PC: " << ifr.PC << std::endl;
    std::cout << "===== IF Stage Register (IFR) =====" << std::endl;
    std::cout << "Instruction : " << ifr.instruction << std::endl;
    std::cout << "PC          : " << ifr.PC << std::endl;
    std::cout << "Opcode      : " << ifr.opcode << std::endl;
    std::cout << "rs1         : " << ifr.rs1 << std::endl;
    std::cout << "rs2         : " << ifr.rs2 << std::endl;
    std::cout << "rd          : " << ifr.rd << std::endl;
    std::cout << "Imm         : " << ifr.imm << std::endl;
    std::cout << "Mem Address : " << ifr.mem_address << std::endl;
    std::cout << "Mem Write   : " << ifr.mem_write << std::endl;
    std::cout << "Reg Write   : " << ifr.reg_write << std::endl;
    std::cout << "ALU Result  : " << ifr.alu_res << std::endl;
    std::cout << "Write Data  : " << ifr.write_data << std::endl;
    std::cout << "===================================" << std::endl;
    ifr.instruction = mem_dtype_to_int(cache.read_data(ifr.PC));
    std::cout << "Fetched instruction: " << ifr.instruction << " from memory." << std::endl;
    for(int i=0;i<8;i++){
        std::cout<<"reg value "<<i<<": "<<registers.GPR[i]<<" ";
    }
    if (ifr.instruction == 0b11111111) {
        halted = true;
        std::cout << "HALT instruction fetched — will stop fetching new instructions." << std::endl;
    } else {
        ifr.PC += 1;
    }
}

void five_stage_pipeline::decode() {
    sleep(1);
    mainWindow->pipelineTable->setItem(0, 0, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 1, new QTableWidgetItem("X"));
    mainWindow->pipelineTable->setItem(0, 2, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 3, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 4, new QTableWidgetItem(""));

    std::cout<<"=================Decoding Stage======================"<<std::endl;
    std::cout << "Decoding instruction: " << idr.instruction << std::endl;
    idr.imm = (idr.opcode == 0b000101) ? extract_imm(idr.instruction) : 0;
    std::cout << "===== ID Stage Register (IDR) =====" << std::endl;
    std::cout << "Instruction : " << idr.instruction << std::endl;
    std::cout << "PC          : " << idr.PC << std::endl;
    std::cout << "Opcode      : " << idr.opcode << std::endl;
    std::cout << "rs1         : " << idr.rs1 << std::endl;
    std::cout << "rs2         : " << idr.rs2 << std::endl;
    std::cout << "rd          : " << idr.rd << std::endl;
    std::cout << "Imm         : " << idr.imm << std::endl;
    std::cout << "Mem Address : " << idr.mem_address << std::endl;
    std::cout << "Mem Write   : " << idr.mem_write << std::endl;
    std::cout << "Reg Write   : " << idr.reg_write << std::endl;
    std::cout << "ALU Result  : " << idr.alu_res << std::endl;
    std::cout << "Write Data  : " << idr.write_data << std::endl;
    std::cout << "===================================" << std::endl;
    idr.opcode = extract_opcode(idr.instruction);  // Decode opcode
    idr.rs1 = extract_rs1(idr.instruction);  // Extract register 1
    idr.rs2 = extract_rs2(idr.instruction);  // Extract register 2
    idr.rd = extract_rd(idr.instruction);  // Extract destination register
    for(int i=0;i<8;i++){
        std::cout<<"reg value "<<i<<": "<<registers.GPR[i]<<" ";
    }
    std::cout << "Decoded opcode: " << idr.opcode << " rs1: " << idr.rs1
              << " rs2: " << idr.rs2 << " rd: " << idr.rd 
              << " imm: " << idr.imm << std::endl;
}

void five_stage_pipeline::execute() {
    sleep(1);
    mainWindow->pipelineTable->setItem(0, 0, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 1, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 2, new QTableWidgetItem("X"));
    mainWindow->pipelineTable->setItem(0, 3, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 4, new QTableWidgetItem(""));

    std::cout<<"=================Executing Stage======================"<<std::endl;
    std::cout << "===== EX Stage Register (EXR) =====" << std::endl;
    std::cout << "Instruction : " << exr.instruction << std::endl;
    std::cout << "PC          : " << exr.PC << std::endl;
    std::cout << "Opcode      : " << exr.opcode << std::endl;
    std::cout << "rs1         : " << exr.rs1 << std::endl;
    std::cout << "rs2         : " << exr.rs2 << std::endl;
    std::cout << "rd          : " << exr.rd << std::endl;
    std::cout << "Imm         : " << exr.imm << std::endl;
    std::cout << "Mem Address : " << exr.mem_address << std::endl;
    std::cout << "Mem Write   : " << exr.mem_write << std::endl;
    std::cout << "Reg Write   : " << exr.reg_write << std::endl;
    std::cout << "ALU Result  : " << exr.alu_res << std::endl;
    std::cout << "Write Data  : " << exr.write_data << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Executing instruction with opcode: " << exr.opcode << std::endl;
    for(int i=0;i<8;i++){
        std::cout<<"reg value "<<i<<": "<<registers.GPR[i]<<" ";
    }
    if(exr.opcode==-1){

    }
    else if (exr.opcode == 0b000101) {
        exr.mem_address = exr.rs1 + exr.imm;
        std::cout << "ISTORE memory address: " << exr.rs1 << std::endl;
    } else if (exr.opcode == 0b0000) {
        exr.mem_address = exr.rs1;
        std::cout << "ILOAD memory address: " << exr.rs1 << std::endl;
    }
    else if (exr.opcode == 0b0001) {
        exr.mem_address = exr.rs1;
        exr.write_data = exr.rs2;
        std::cout << "ISTORE memory address: " << exr.rs1 << std::endl;
    }
    else {
        std::cout<<"Register RS1: "<<exr.rs1<<" "<<"Register RS2: "<<exr.rs2<<std::endl;
        exr.alu_res = ALU::execute(exr.opcode, registers.GPR[exr.rs1], registers.GPR[exr.rs2]);
        // exr.alu_res = exr.write_data;
    }
    std::cout << "ALU result: " << exr.alu_res << ", rd: " << exr.rd << std::endl;
}

void five_stage_pipeline::mem_stage() {
    sleep(1);
    mainWindow->pipelineTable->setItem(0, 0, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 1, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 2, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 3, new QTableWidgetItem("X"));
    mainWindow->pipelineTable->setItem(0, 4, new QTableWidgetItem(""));

    std::cout<<"=================Memory Stage======================"<<std::endl;
    std::cout << "===== MEM Stage Register (MEMR) =====" << std::endl;
    std::cout << "Instruction : " << memr.instruction << std::endl;
    std::cout << "PC          : " << memr.PC << std::endl;
    std::cout << "Opcode      : " << memr.opcode << std::endl;
    std::cout << "rs1         : " << memr.rs1 << std::endl;
    std::cout << "rs2         : " << memr.rs2 << std::endl;
    std::cout << "rd          : " << memr.rd << std::endl;
    std::cout << "Imm         : " << memr.imm << std::endl;
    std::cout << "Mem Address : " << memr.mem_address << std::endl;
    std::cout << "Mem Write   : " << memr.mem_write << std::endl;
    std::cout << "Reg Write   : " << memr.reg_write << std::endl;
    std::cout << "ALU Result  : " << memr.alu_res << std::endl;
    std::cout << "Write Data  : " << memr.write_data << std::endl;
    std::cout << "Memory Access for Address : " << memr.mem_address << std::endl;
    std::cout << "=====================================" << std::endl;
    for(int i=0;i<8;i++){
        std::cout<<"reg value "<<i<<": "<<registers.GPR[i]<<" ";
    }
    if(memr.opcode==-1){

    }
    else if (memr.opcode == 0b0001) {
        memr.write_data = mem_dtype_to_int(cache.read_data(memr.mem_address));
        std::cout << "Loaded value: " << memr.write_data<<" From Address: "<<memr.alu_res << std::endl;
    } else if (memr.opcode == 0b0000) {
        cache.write_data(memr.mem_address, int_to_mem_dtype(memr.rs2));
        std::cout << "Store: Memory write at address: " << memr.alu_res << " with value: " << memr.rs2 << std::endl;
    } else {
        memr.write_data = memr.alu_res;
    }
}

void five_stage_pipeline::writeback() {
    sleep(1);
    mainWindow->pipelineTable->setItem(0, 0, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 1, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 2, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 3, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 4, new QTableWidgetItem("X"));

    std::cout<<"=================WriteBack Stage======================"<<std::endl;
    std::cout << "===== Write Back Stage Register (WBR) =====" << std::endl;
    std::cout << "Instruction : " << wbr.instruction << std::endl;
    std::cout << "PC          : " << wbr.PC << std::endl;
    std::cout << "Opcode      : " << wbr.opcode << std::endl;
    std::cout << "rs1         : " << wbr.rs1 << std::endl;
    std::cout << "rs2         : " << wbr.rs2 << std::endl;
    std::cout << "rd          : " << wbr.rd << std::endl;
    std::cout << "Imm         : " << wbr.imm << std::endl;
    std::cout << "Mem Address : " << wbr.mem_address << std::endl;
    std::cout << "Mem Write   : " << wbr.mem_write << std::endl;
    std::cout << "Reg Write   : " << wbr.reg_write << std::endl;
    std::cout << "ALU Result  : " << wbr.alu_res << std::endl;
    std::cout << "Write Data  : " << wbr.write_data << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "Writing back to register: " << wbr.write_data << std::endl;
    for(int i=0;i<8;i++){
        std::cout<<"reg value "<<i<<": "<<registers.GPR[i]<<" ";
    }
    // if(wbr.opcode!=-1) registers.GPR[wbr.rd] = wbr.write_data;
    if(wbr.opcode!=-1) registers.write_register(wbr.rd,wbr.write_data);
    std::cout << "Register " << wbr.rd << " updated with value: " << registers.GPR[wbr.rd] << std::endl;
}
void five_stage_pipeline::clock_cycle() {
    // Parallel execution (from WB to IF)
    if (wb_valid) writeback();
    if (mem_valid) mem_stage();
    if (ex_valid) execute();
    if (id_valid) decode();
    if (!halted) fetch();

    std::cout << "Pipeline Validity Flags Before:" << std::endl;
    std::cout << "  IF: " << if_valid
              << " | ID: " << id_valid
              << " | EX: " << ex_valid
              << " | MEM: " << mem_valid
              << " | WB: " << wb_valid 
              << " | Halted: " << halted << std::endl;

    // Propagate the valid bits
    // wb_valid  = mem_valid;
    // mem_valid = ex_valid;
    // ex_valid  = id_valid;
    // id_valid  = if_valid;
    // if_valid  = !halted;  // Only fetch if not halted

    

    bool hazard = false;

    if (exr.opcode!=-1 && ( (idr.rs1 == exr.rd && exr.rd != 0) || (idr.rs2 == exr.rd && exr.rd != 0) ))
        hazard = true;
    if (memr.opcode!=-1 && ( (idr.rs1 == memr.rd && memr.rd != 0) || (idr.rs2 == memr.rd && memr.rd != 0) ))
        hazard = true;
    if (hazard) {
        // Propagate valid bits with stall
        wb_valid  = mem_valid;
        mem_valid = ex_valid;
        ex_valid  = false;     // NOP in exr
        // id_valid and if_valid stay the same (stall fetch/decode)
    } else {
        // Normal pipeline propagation
        wb_valid  = mem_valid;
        mem_valid = ex_valid;
        ex_valid  = id_valid;
        id_valid  = if_valid;
        if_valid  = !halted;
    }
    std::cout << "Pipeline Validity Flags After:" << std::endl;
    std::cout << "  IF: " << if_valid
              << " | ID: " << id_valid
              << " | EX: " << ex_valid
              << " | MEM: " << mem_valid
              << " | WB: " << wb_valid
              << " | Halted: " << halted << std::endl;

    std::cout << "---------------------------------------------" << std::endl;

    std::cout << "[IFR]  "
          << "Instr: " << ifr.instruction << " | "
          << "PC: " << ifr.PC << " | "
          << "Opcode: " << ifr.opcode << " | "
          << "rs1: " << ifr.rs1 << " | "
          << "rs2: " << ifr.rs2 << " | "
          << "rd: " << ifr.rd << " | "
          << "Imm: " << ifr.imm << std::endl;

std::cout << "[IDR]  "
          << "Instr: " << idr.instruction << " | "
          << "PC: " << idr.PC << " | "
          << "Opcode: " << idr.opcode << " | "
          << "rs1: " << idr.rs1 << " | "
          << "rs2: " << idr.rs2 << " | "
          << "rd: " << idr.rd << " | "
          << "Imm: " << idr.imm << std::endl;

std::cout << "[EXR]  "
          << "Instr: " << exr.instruction << " | "
          << "PC: " << exr.PC << " | "
          << "Opcode: " << exr.opcode << " | "
          << "rs1: " << exr.rs1 << " | "
          << "rs2: " << exr.rs2 << " | "
          << "rd: " << exr.rd << " | "
          << "Imm: " << exr.imm << " | "
          << "ALURes: " << exr.alu_res << std::endl;

std::cout << "[MEMR] "
          << "Instr: " << memr.instruction << " | "
          << "PC: " << memr.PC << " | "
          << "Opcode: " << memr.opcode << " | "
          << "MemAddr: " << memr.mem_address << " | "
          << "MemWrite: " << memr.mem_write << " | "
          << "RegWrite: " << memr.reg_write << " | "
          << "ALURes: " << memr.alu_res << " | "
          << "WriteData: " << memr.write_data << std::endl;

std::cout << "[WBR]  "
          << "Instr: " << wbr.instruction << " | "
          << "PC: " << wbr.PC << " | "
          << "Opcode: " << wbr.opcode << " | "
          << "MemAddr: " << wbr.mem_address << " | "
          << "MemWrite: " << wbr.mem_write << " | "
          << "RegWrite: " << wbr.reg_write << " | "
          << "ALURes: " << wbr.alu_res << " | "
          << "WriteData: " << wbr.write_data
          << std::endl;
    if (hazard) {
        std::cout<<"HAZARD"<<std::endl;
        wbr = memr;
        memr = exr;
        exr.opcode =-1;
        // idr, ifr remain unchanged — stall fetch and decode
    } else {
        std::cout<<"NO HAZARD"<<std::endl;
        wbr = memr;
        memr = exr;
        exr = idr;
        idr = ifr;
        // fetch next instruction into ifr
    }
    std::cout<<"After SWAP"<<std::endl;
    std::cout << "[IFR]  "
          << "Instr: " << ifr.instruction << " | "
          << "PC: " << ifr.PC << " | "
          << "Opcode: " << ifr.opcode << " | "
          << "rs1: " << ifr.rs1 << " | "
          << "rs2: " << ifr.rs2 << " | "
          << "rd: " << ifr.rd << " | "
          << "Imm: " << ifr.imm << std::endl;

std::cout << "[IDR]  "
          << "Instr: " << idr.instruction << " | "
          << "PC: " << idr.PC << " | "
          << "Opcode: " << idr.opcode << " | "
          << "rs1: " << idr.rs1 << " | "
          << "rs2: " << idr.rs2 << " | "
          << "rd: " << idr.rd << " | "
          << "Imm: " << idr.imm << std::endl;

std::cout << "[EXR]  "
          << "Instr: " << exr.instruction << " | "
          << "PC: " << exr.PC << " | "
          << "Opcode: " << exr.opcode << " | "
          << "rs1: " << exr.rs1 << " | "
          << "rs2: " << exr.rs2 << " | "
          << "rd: " << exr.rd << " | "
          << "Imm: " << exr.imm << " | "
          << "ALURes: " << exr.alu_res << std::endl;

std::cout << "[MEMR] "
          << "Instr: " << memr.instruction << " | "
          << "PC: " << memr.PC << " | "
          << "Opcode: " << memr.opcode << " | "
          << "MemAddr: " << memr.mem_address << " | "
          << "MemWrite: " << memr.mem_write << " | "
          << "RegWrite: " << memr.reg_write << " | "
          << "ALURes: " << memr.alu_res << " | "
          << "WriteData: " << memr.write_data << std::endl;

std::cout << "[WBR]  "
          << "Instr: " << wbr.instruction << " | "
          << "PC: " << wbr.PC << " | "
          << "Opcode: " << wbr.opcode << " | "
          << "MemAddr: " << wbr.mem_address << " | "
          << "MemWrite: " << wbr.mem_write << " | "
          << "RegWrite: " << wbr.reg_write << " | "
          << "ALURes: " << wbr.alu_res << " | "
          << "WriteData: " << wbr.write_data
          << std::endl;
    // wbr = memr;
    // memr = exr;
    // exr = idr;
    // idr = ifr;
}
void five_stage_pipeline::run_pipeline() {
    // Example: Loop through the pipeline until a certain condition (e.g., no more instructions)
    // int cycle_count = 0;
    // while (cycle_count < 1) {  // Limiting the number of cycles for testing
    //     std::cout << "Cycle: " << cycle_count << std::endl;
    //     fetch();        // Stage 1: Fetch the instruction
    //     decode();       // Stage 2: Decode the instruction
    //     execute();      // Stage 3: Execute the instruction
    //     mem_stage();       // Stage 4: Perform memory operations (e.g., load/store)
    //     writeback();    // Stage 5: Write the results back to registers
    //     cycle_count++;  // Increment cycle count
    // }

    int cycle = 0;
    while ((if_valid || id_valid || ex_valid || mem_valid || wb_valid || !halted) && cycle<20) {
        std::cout << "=== Clock Cycle: " << cycle << " ===" << std::endl;
        clock_cycle();
        for(int i=0;i<16;i++){
            cache.read_data(i);
        }
        cycle++;
    }
}