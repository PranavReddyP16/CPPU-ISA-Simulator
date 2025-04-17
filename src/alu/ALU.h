#pragma once

class ALU {
public:
    static int execute(int opcode, int operand1, int operand2, int operand3 = 0,int flag=0);
    static float execute(int opcode, float operand1, float operand2);
};