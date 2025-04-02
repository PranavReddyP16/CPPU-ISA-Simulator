#include "ALU.h"
#include "../../include/system_constants.h"

int ALU::execute(int opcode, int operand1, int operand2, int operand3) {
    switch (opcode) {
        case 0b010001: return operand1 + operand2; // ADD
        case 0b010011: return operand1 - operand2; // SUB
        case 0b010100: return operand1 * operand2; // MUL
        case 0b010101: return operand2 ? operand1 / operand2 : 0; // DIV
        case 0b011001: return operand1 % operand2; // MOD
        default: return 0;
    }
}

float ALU::execute(int opcode, float operand1, float operand2) {
    switch (opcode) {
        case 0b010010: return operand1 + operand2; // FADD
        case 0b010011: return operand1 - operand2; // FSUB
        case 0b010100: return operand1 * operand2; // FMUL  (Fixed duplicate case)
        case 0b010110: return operand2 ? operand1 / operand2 : 0.0f; // FDIV
        default: return 0.0f;
    }
}