// Control.cpp
#include "control.h"
#include "ALU.h"
#include <iostream>
#include <bitset>

//––– adjust these to match your system_constants.h if you already have macros for them –––
constexpr int ZF_MASK         = 1 << 0;   // Zero Flag in SR
constexpr int SF_MASK         = 1 << 1;   // Sign Flag in SR
constexpr int INSTRUCTION_SIZE = 4;       // bytes per instruction

Control::Control(Register& regPtr)
  : reg(regPtr)
{}

void Control::execute(int opcode) {
    int instr = reg.get_instruction();
    switch (opcode) {
      case 0b001000: { // CALL
        int target     = instr & 0x03FFFFFF;
        int returnAddr = reg.PC + INSTRUCTION_SIZE;
        reg.save_return_address(returnAddr);
        reg.PC = target;
        break;
      }
      case 0b001001: { // RET
        reg.PC = reg.get_return_address();
        break;
      }
      case 0b001010: { // CMP
        // bits 25–23 = reg1, 22–20 = reg2
        int r1 = (instr >> 23) & 0x7;
        int r2 = (instr >> 20) & 0x7;
        int v1 = reg.read_register(r1);
        int v2 = reg.read_register(r2);
        bool zf = (v1 == v2);
        bool sf = (v1 <  v2);
        // clear old ZF/SF then set new ones
        int status = reg.get_status() & ~(ZF_MASK | SF_MASK);
        status |= (zf ? ZF_MASK : 0);
        status |= (sf ? SF_MASK : 0);
        reg.update_status(status);
        break;
      }
      case 0b001011: { // JMP
        int target = instr & 0x03FFFFFF;
        reg.PC = target;
        break;
      }
      case 0b001100: { // JE
        {
          int target = instr & 0x03FFFFFF;
          bool zf = reg.get_status() & ZF_MASK;
          reg.PC = zf ? target : reg.PC + INSTRUCTION_SIZE;
        }
        break;
      }
      case 0b001101: { // JNE
        {
          int target = instr & 0x03FFFFFF;
          bool zf = reg.get_status() & ZF_MASK;
          reg.PC = (!zf) ? target : reg.PC + INSTRUCTION_SIZE;
        }
        break;
      }
      case 0b001110: { // JG  (greater → SF=0 && ZF=0)
        {
          int target = instr & 0x03FFFFFF;
          int st     = reg.get_status();
          bool zf    = st & ZF_MASK;
          bool sf    = st & SF_MASK;
          reg.PC = (!sf && !zf) ? target : reg.PC + INSTRUCTION_SIZE;
        }
        break;
      }
      case 0b001111: { // JL  (less → SF=1)
        {
          int target = instr & 0x03FFFFFF;
          bool sf    = reg.get_status() & SF_MASK;
          reg.PC = sf ? target : reg.PC + INSTRUCTION_SIZE;
        }
        break;
      }
      case 0b010000: { // JZ  (same as JE)
        int target = instr & 0x03FFFFFF;
        bool zf    = reg.get_status() & ZF_MASK;
        reg.PC = zf ? target : reg.PC + INSTRUCTION_SIZE;
        break;
      }

      default:
        std::cerr 
          << "Unsupported control opcode: " 
          << std::bitset<6>(opcode) << std::endl;
    }
}
