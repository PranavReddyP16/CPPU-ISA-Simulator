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
    if_id = {0, 0};  // PC = 0, instruction = 0
    if_id.PC = 1;
    id_ex = {0, 0, 0, 0, 0};  // Default values for opcode, registers
    ex_mem = {0, 0, 0}; 
    mem_wb = {0, 0};
}

void five_stage_pipeline::fetch() {
    sleep(1);
    mainWindow->pipelineTable->setItem(0, 0, new QTableWidgetItem("X"));
    mainWindow->pipelineTable->setItem(0, 1, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 2, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 3, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 4, new QTableWidgetItem(""));

    std::cout << "Fetching instruction at PC: " << if_id.PC << std::endl;
    if_id.instruction = mem_dtype_to_int(cache.read_data(if_id.PC));  // Load instruction from memory
    // if_id.instruction = 1957745664;
    std::cout << "Fetched instruction: " << if_id.instruction << " from memory." << std::endl;
    // if_id.PC += 4;  // Increment PC to next instruction (assuming 32-bit instructions)
    std::cout << "Next PC: " << if_id.PC << std::endl;
}

void five_stage_pipeline::decode() {
    sleep(1);
    mainWindow->pipelineTable->setItem(0, 0, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 1, new QTableWidgetItem("X"));
    mainWindow->pipelineTable->setItem(0, 2, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 3, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 4, new QTableWidgetItem(""));

    std::cout << "Decoding instruction: " << if_id.instruction << std::endl;
    id_ex.opcode = extract_opcode(if_id.instruction);  // Decode opcode
    id_ex.rs1 = extract_rs1(if_id.instruction);  // Extract register 1
    id_ex.rs2 = extract_rs2(if_id.instruction);  // Extract register 2
    id_ex.rd = extract_rd(if_id.instruction);  // Extract destination register
    id_ex.PC = if_id.PC;  // Carry forward PC for branch or jump handling
    if (id_ex.opcode == 0b000101) {  
        id_ex.imm = extract_imm(if_id.instruction);
        std::cout << "Extracted immediate value: " << id_ex.imm << std::endl;
    } else {
        id_ex.imm = 0;  // Default to zero for non-immediate instructions
    }
        std::cout << "Decoded opcode: " << id_ex.opcode << ", rs1: " << id_ex.rs1 
              << ", rs2: " << id_ex.rs2 << ", rd: " << id_ex.rd << ", PC: " << id_ex.PC << std::endl;
}

void five_stage_pipeline::execute() {
    sleep(1);
    mainWindow->pipelineTable->setItem(0, 0, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 1, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 2, new QTableWidgetItem("X"));
    mainWindow->pipelineTable->setItem(0, 3, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 4, new QTableWidgetItem(""));

    std::cout << "Executing instruction with opcode: " << id_ex.opcode << std::endl;
    // Perform ALU operation based on opcode
    if (id_ex.opcode == 0b000101) {  // ISTORE
        ex_mem.alu_res = id_ex.rs1 + id_ex.imm;  // Compute memory address manually
        std::cout << "ISTORE memory address: " << ex_mem.alu_res << std::endl;
    }
    else if (id_ex.opcode == 0b00) {  // ISTORE
        ex_mem.alu_res = id_ex.rs1;  // Compute memory address manually
        std::cout << "ISTORE memory address: " << ex_mem.alu_res << std::endl;
    }
    else if (id_ex.opcode == 0b01) {  // ILOAD
        ex_mem.alu_res = id_ex.rs1;  // Compute memory address manually
        std::cout << "ILOAD memory address: " << ex_mem.alu_res << std::endl;
    }
    else {
        std::cout<<"Register RS1: "<<id_ex.rs1<<" "<<"Register RS2: "<<id_ex.rs2<<std::endl;
        ex_mem.alu_res = ALU::execute(id_ex.opcode, registers.GPR[id_ex.rs1], registers.GPR[id_ex.rs2]);
    }
        ex_mem.rd = id_ex.rd;  // Carry forward destination register
    std::cout << "ALU result: " << ex_mem.alu_res << ", rd: " << ex_mem.rd << std::endl;
}

void five_stage_pipeline::mem_stage() {
    sleep(1);
    mainWindow->pipelineTable->setItem(0, 0, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 1, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 2, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 3, new QTableWidgetItem("X"));
    mainWindow->pipelineTable->setItem(0, 4, new QTableWidgetItem(""));

    std::cout << "Memory access for address: " << ex_mem.alu_res << std::endl;
    mem_wb.rd = ex_mem.rd;
    // If the instruction is a load operation, fetch from cache
    // if (id_ex.opcode == 0b000010 || id_ex.opcode == 0b000011) {  
    //     mem_wb.write_data = mem_dtype_to_int(cache.read_data(ex_mem.alu_res));  
    //     std::cout << "Memory read data: " << mem_wb.write_data << std::endl;
    // } 
    // If the instruction is a store operation, write to cache
    if (id_ex.opcode == 0b01) {  
        mem_wb.write_data = mem_dtype_to_int(cache.read_data(ex_mem.alu_res));  
        std::cout << "Memory read data: " << mem_wb.write_data << std::endl;
    }
    else if (id_ex.opcode == 0b00) {  
        cache.write_data(ex_mem.alu_res, int_to_mem_dtype(id_ex.rs2));  
        std::cout << "Memory write at address: " << ex_mem.alu_res << " with value: " << id_ex.rs2 << std::endl;
    } 
    else {
        mem_wb.write_data = ex_mem.alu_res;  
    }
}

void five_stage_pipeline::writeback() {
    sleep(1);
    mainWindow->pipelineTable->setItem(0, 0, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 1, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 2, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 3, new QTableWidgetItem(""));
    mainWindow->pipelineTable->setItem(0, 4, new QTableWidgetItem("X"));

    std::cout << "Writing back to register: " << mem_wb.write_data << std::endl;
    registers.GPR[mem_wb.rd] = mem_wb.write_data;  
    std::cout << "Register " << mem_wb.rd << " updated with value: " << registers.GPR[mem_wb.rd] << std::endl;
}

void five_stage_pipeline::run_pipeline() {
    // Example: Loop through the pipeline until a certain condition (e.g., no more instructions)
    int cycle_count = 0;
    while (cycle_count < 1) {  // Limiting the number of cycles for testing
        std::cout << "Cycle: " << cycle_count << std::endl;
        fetch();        // Stage 1: Fetch the instruction
        decode();       // Stage 2: Decode the instruction
        execute();      // Stage 3: Execute the instruction
        mem_stage();       // Stage 4: Perform memory operations (e.g., load/store)
        writeback();    // Stage 5: Write the results back to registers
        cycle_count++;  // Increment cycle count
    }
}