#pragma once
#include "system_constants.h"

class Memory {
public:
    Memory();
    void reset();

    MemoryDataType read_data(int address) const;
    void write_data(int address, MemoryDataType data);

    MemoryDataType& operator[](int address);

private:
    char data[MEMORY_SIZE];
};
