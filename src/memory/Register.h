#pragma once

#include "system_constants.h"
#include "gui/mainwindow.h"

class Register {
public:
    int PC;         // Program Counter
    int IR;         // Instruction Register
    int SP;         // Stack Pointer
    int BP;         // Stack Base Pointer
    int RA;         // Return Address Register
    int SR;         // Status Register

    int GPR[8];     // General Purpose Registers R0 - R7
    float FPR[8];   // Floating Point Registers F0 - F7

    Register(MainWindow* mainWindow);
    void reset();   

    // Instruction Fetch
    void load_instruction(int instruction);
    int get_instruction() const;

    // Stack Operations
    void push_stack(int value);
    int pop_stack();

    // Function Call Handling
    void save_return_address(int return_addr);
    int get_return_address() const;

    // Register Access
    void write_register(int reg, int value);
    int read_register(int reg) const;

    void write_fregister(int reg, float value);
    float read_fregister(int reg) const;

    // Status Register
    void update_status(int flag);
    int get_status() const;

private:
    MainWindow* mainWindow;
};