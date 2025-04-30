#pragma once

#include "system_constants.h"
#include <stdexcept>

class Register {
public:
    Register() { reset(); }

    // Reset all registers to zero
    void reset() {
        PC = IR = SP = BP = RA = SR = 0;
        for (int i = 0; i < 8; ++i) {
            GPR[i] = 0;
            FPR[i] = 0.0f;
        }
    }

    // Instruction Fetch
    void load_instruction(int instruction) { IR = instruction;  }
    int  get_instruction() const         { return IR;        }

    // Stack Operations
    void push_stack(int value) { SP -= 4; }
    int  pop_stack()           { SP += 4; return SP; }

    // Function Call Handling
    void save_return_address(int return_addr) { RA = return_addr; }
    int  get_return_address() const           { return RA;         }

    // General Purpose Register Access
    void write_register(int reg, int value) {
        if (reg < 0 || reg >= 8) throw std::out_of_range("Invalid GPR index");
        GPR[reg] = value;
    }
    int read_register(int reg) const {
        if (reg < 0 || reg >= 8) throw std::out_of_range("Invalid GPR index");
        return GPR[reg];
    }

    // Floating Point Register Access
    void write_fregister(int reg, float value) {
        if (reg < 0 || reg >= 8) throw std::out_of_range("Invalid FPR index");
        FPR[reg] = value;
    }
    float read_fregister(int reg) const {
        if (reg < 0 || reg >= 8) throw std::out_of_range("Invalid FPR index");
        return FPR[reg];
    }

    // Status Register
    void update_status(int flag) { SR = flag; }
    int  get_status() const      { return SR; }

    // Publicly readable fields
    int PC;         ///< Program Counter
    int IR;         ///< Instruction Register
    int SP;         ///< Stack Pointer
    int BP;         ///< Base Pointer
    int RA;         ///< Return Address
    int SR;         ///< Status Register

    int   GPR[8];   ///< General-Purpose Registers R0–R7
    float FPR[8];   ///< Floating-Point Registers F0–F7
};
