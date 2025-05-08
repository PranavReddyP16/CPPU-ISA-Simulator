#include "Register.h"
#include <stdexcept>
#include <algorithm>

Register::Register() {
    reset();
}

void Register::reset() {
    PC = IR = SP = BP = RA = SR = 0;
    std::fill(std::begin(GPR), std::end(GPR), 0);
    std::fill(std::begin(FPR), std::end(FPR), 0.0f);
}

void Register::write_register(int reg, int value) {
    if (reg < 0 || reg >= 8) throw std::out_of_range("Invalid GPR index");
    GPR[reg] = value;
}

int Register::read_register(int reg) const {
    if (reg < 0 || reg >= 8) throw std::out_of_range("Invalid GPR index");
    return GPR[reg];
}

void Register::write_fregister(int reg, float value) {
    if (reg < 0 || reg >= 8) throw std::out_of_range("Invalid FPR index");
    FPR[reg] = value;
}

float Register::read_fregister(int reg) const {
    if (reg < 0 || reg >= 8) throw std::out_of_range("Invalid FPR index");
    return FPR[reg];
}
