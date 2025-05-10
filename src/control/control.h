#pragma once

#include "../memory/Register.h"

class Control {
public:
    Register reg;
    Control(Register& reg);
    void execute(int opcode);
};