#include "no_pipeline_exec.h"
 
 using OP = Opcodes;
 
 no_pipeline_exec::no_pipeline_exec(Cache &c)
     : cache(c)
 {
     regs.reset();
     halted = false;
     alsr_valid = false;
     stage = 0; 
 }
 
 void no_pipeline_exec::allstages() {
     if (!alsr_valid) {
         
         if (halted) return;
 
         
         uint64_t inst = cache.read_data(regs.PC);
         alsr.instruction = inst;
         alsr.PC = regs.PC;
         alsr.opcode = extract_opcode(inst);
         alsr_valid = true;
         stage = 1; 
         regs.PC += 1;
         if (alsr.opcode == OP::HLT_OP) halted = true;
         return;
     }
 
     if (stage == 1) {
         
         uint64_t inst = alsr.instruction;
         int opc = alsr.opcode;
 
         if (opc == OP::LI_OP) {
             alsr.rd = extract_rs1(inst);
             alsr.imm = inst & 0xFFFFFFFFULL; // 32-bit immediate
         }
         else if (opc == OP::ILOAD_OP) {
             alsr.rd = extract_rs1(inst);
             alsr.mem_address = extract_mem_address(inst);
         }
         else if (opc == OP::ISTORE_OP) {
             alsr.rs1 = extract_rs1(inst);
             alsr.mem_address = extract_mem_address(inst);
         }
         else { 
             alsr.rd = extract_rs1(inst);
             alsr.rs1 = alsr.rd; // Two-operand form: rs1 == rd
             alsr.rs2 = extract_rs2(inst);
         }
         stage = 2; 
     }
     else if (stage == 2) {
         
         int opc = alsr.opcode;
 
         if (opc == OP::LI_OP) {
             alsr.write_data = alsr.imm;
             alsr.reg_write = true;
         }
         else if (opc == OP::ADD_OP || opc == OP::SUB_OP ||
                  opc == OP::MUL_OP || opc == OP::DIV_OP ||
                  opc == OP::MOD_OP || opc == OP::AND_OP ||
                  opc == OP::OR_OP  || opc == OP::NOT_OP) {
             int a = regs.read_register(alsr.rs1);
             int b = regs.read_register(alsr.rs2);
             alsr.alu_res = ALU::execute(opc, a, b);
             alsr.write_data = alsr.alu_res;
             alsr.reg_write = true;
         }
         else if (opc == OP::ISTORE_OP) {
             alsr.write_data = regs.read_register(alsr.rs1);
             alsr.mem_write = true;
             alsr.reg_write = false;
         }
         stage = 3; 
     }
     else if (stage == 3) {
         
         int opc = alsr.opcode;
         if (opc == OP::ILOAD_OP) {
             auto v = cache.read_data(alsr.mem_address);
             alsr.write_data = int(mem_dtype_to_int(v));
             alsr.reg_write = true;
         }
         else if (opc == OP::ISTORE_OP) {
             cache.write_data(alsr.mem_address, int_to_mem_dtype(alsr.write_data));
             alsr.reg_write = false;
         }
         stage = 4; 
     }
     else if (stage == 4) {
         
         if (alsr.reg_write && alsr.rd < 8) {
             regs.write_register(alsr.rd, alsr.write_data);
             std::cout << ">> WB: GPR[" << alsr.rd << "] = "
                       << alsr.write_data << std::endl;
         }
         
         alsr_valid = false;
         stage = 0; // Move to Fetch
     }
 }
 
 void no_pipeline_exec::clock_cycle() {
     allstages();
 }
 
 void no_pipeline_exec::run_instructions() {
     while (!halted || alsr_valid) {
         clock_cycle();
     }
 }