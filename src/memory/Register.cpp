#include "Register.h"
#include <stdexcept>  // For exception handling

Register::Register() {
    reset();
}

void Register::reset() {
    PC = 0;
    IR = 0;
    SP = 0;
    BP = 0;
    RA = 0;
    SR = 0;

    for (int i = 0; i < 8; i++) {
        GPR[i] = 0;
        FPR[i] = 0.0f;
    }
}

// ------------------- Instruction Fetch -------------------
void Register::load_instruction(int instruction) {
    IR = instruction;
}

int Register::get_instruction() const {
    return IR;
}

// ------------------- Stack Operations -------------------
void Register::push_stack(int value) {
    SP -= 4;  // Assume each stack entry is 4 bytes
    // Stack memory is handled separately in the pipeline
}

int Register::pop_stack() {
    SP += 4;
    return SP;  // Should return the popped value, but stack memory is separate
}

// ------------------- Function Call Handling -------------------
void Register::save_return_address(int return_addr) {
    RA = return_addr;
}

int Register::get_return_address() const {
    return RA;
}

// ------------------- General Purpose Register Access -------------------
void Register::write_register(int reg, int value) {
    if (reg < 0 || reg >= 8) {
        throw std::out_of_range("Invalid register index");
    }
    GPR[reg] = value;
}

int Register::read_register(int reg) const {
    if (reg < 0 || reg >= 8) {
        throw std::out_of_range("Invalid register index");
    }
    return GPR[reg];
}

// ------------------- Floating Point Register Access -------------------
void Register::write_fregister(int reg, float value) {
    if (reg < 0 || reg >= 8) {
        throw std::out_of_range("Invalid floating point register index");
    }
    FPR[reg] = value;
}

float Register::read_fregister(int reg) const {
    if (reg < 0 || reg >= 8) {
        throw std::out_of_range("Invalid floating point register index");
    }
    return FPR[reg];
}

// ------------------- Status Register -------------------
void Register::update_status(int flag) {
    SR = flag;  // Update status register with new flag
}

int Register::get_status() const {
    return SR;
}