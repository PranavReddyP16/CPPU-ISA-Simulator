#pragma once

#include "system_constants.h"

// A pure data‐only register file, no GUI hooks.
class Register {
public:
    Register();
    void reset();

    // Integer registers R0–R7
    void   write_register(int reg, int value);
    int    read_register (int reg) const;

    // Floating-point registers F0–F7
    void   write_fregister(int reg, float value);
    float  read_fregister (int reg) const;

    // Public state for GUI inspection
    int    PC;   // Program Counter
    int    IR;   // Instruction Register
    int    SP;   // Stack Pointer
    int    BP;   // Base Pointer
    int    RA;   // Return Address
    int    SR;   // Status Register

    int    GPR[8];   // integer regs
    float  FPR[8];   // float regs
};
