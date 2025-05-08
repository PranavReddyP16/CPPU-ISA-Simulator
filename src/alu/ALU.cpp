// #include "ALU.h"
// #include "system_constants.h"

// int ALU::execute(int opcode, int operand1, int operand2, int operand3) {
//     switch (opcode) {
//         case 0b0000: return operand1 + operand2;
//         case 0b0001: return operand1 - operand2;
//         case 0b0010: return operand1 * operand2;
//         case 0b0011: return operand1 / operand2;
//         case 0b010001: return operand1 + operand2; // ADD
//         case 0b010011: return operand1 - operand2; // SUB
//         case 0b010100: return operand1 * operand2; // MUL
//         case 0b010101: return operand2 ? operand1 / operand2 : 0; // DIV
//         case 0b011001: return operand1 % operand2; // MOD
//         default: return 0;
//     }
// }

// float ALU::execute(int opcode, float operand1, float operand2) {
//     switch (opcode) {
//         case 0b010010: return operand1 + operand2; // FADD
//         case 0b010011: return operand1 - operand2; // FSUB
//         case 0b010100: return operand1 * operand2; // FMUL  (Fixed duplicate case)
//         case 0b010110: return operand2 ? operand1 / operand2 : 0.0f; // FDIV
//         default: return 0.0f;
//     }
// }


#include "ALU.h"
#include <cmath>
#include <iostream>
#include <bitset>
#include "../../include/system_constants.h"

int ALU::execute(int opcode, int operand1, int operand2, int operand3, int flag) {
    std::cout << "Opcode: " << std::bitset<6>(opcode)
              << " Operand1: " << operand1
              << " Operand2: " << operand2
              << " Operand3: " << operand3
              << " Flag: " << flag << std::endl;

    switch (opcode) {
        case 0b010001: return operand1 + operand2;                           // ADD
        case 0b010011: return operand1 - operand2;                           // SUB
        case 0b010100: return operand1 * operand2;                           // MUL
        case 0b010101: return operand2 ? operand1 / operand2 : 0;            // DIV
        case 0b011001: return operand1 % operand2;                           // MOD

        case 0b011010: // LSHIFT
            return (flag == 0) ? (operand1 << operand2)                      // Logical
                               : (operand1 << operand2);                     // Arithmetic same for left

        case 0b011011: // RSHIFT
            return (flag == 0) ? (operand1 >> operand2)                         // Logical
                               : (operand1 >> operand2);                     // Arithmetic (preserves sign)

        case 0b011100: return operand1 | (1 << operand2);                    // SETBIT
        case 0b011101: return operand1 & ~(1 << operand2);                   // UNSETBIT

        case 0b011110: // GETBIT
            return ((operand1 >> operand2) & 1) ? 1 : 0;

        case 0b011111: // POPCOUNT
        {
            int count = 0;
            unsigned int n = operand1;
            while (n) {
                count += n & 1;
                n >>= 1;
            }
            return count;
        }

        default: 
            std::cerr << "Unsupported opcode: " << opcode << std::endl;
            return 0;
    }
}

float ALU::execute(int opcode, float operand1, float operand2) {
    switch (opcode) {
        case 0b010010: return operand1 + operand2;                           // FADD
        case 0b010011: return operand1 - operand2;                           // FSUB
        case 0b010100: return operand1 * operand2;                           // FMUL
        case 0b010110: return operand2 ? operand1 / operand2 : 0.0f;         // FDIV
        default: 
            std::cerr << "Unsupported float opcode: " << opcode << std::endl;
            return 0.0f;
    }
}