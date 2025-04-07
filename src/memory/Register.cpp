#include <iostream>
#include "Register.h"
#include <stdexcept>  // For exception handling

Register::Register(MainWindow* mainWindow) : mainWindow(mainWindow) { 
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
    // TODO: Reset the GUI also
}

// ------------------- Instruction Fetch -------------------
void Register::load_instruction(int instruction) {
    IR = instruction;
    mainWindow->setRegisterValue("IR", instruction);
}

int Register::get_instruction() const {
    return IR;
}

// ------------------- Stack Operations -------------------
void Register::push_stack(int value) {
    SP -= 4;  // Assume each stack entry is 4 bytes
    // Stack memory is handled separately in the pipeline
    mainWindow->setRegisterValue("SP", SP);
}

int Register::pop_stack() {
    SP += 4;
    mainWindow->setRegisterValue("SP", SP);
    return SP;  // Should return the popped value, but stack memory is separate
}

// ------------------- Function Call Handling -------------------
void Register::save_return_address(int return_addr) {
    RA = return_addr;
    mainWindow->setRegisterValue("RA", return_addr);
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
    char* regName = (char* )("GPR[" + std::to_string(reg) + "]").c_str();
    mainWindow->setRegisterValue(regName, value);
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
    char* regName = (char* )("FPR[" + std::to_string(reg) + "]").c_str();
    mainWindow->setRegisterValue(regName, value);
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
    mainWindow->setRegisterValue("SR", flag);
}

int Register::get_status() const {
    return SR;
}