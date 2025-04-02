#pragma once

#include "../../include/system_constants.h"

// TODO check public private stuff I'm making everything public for now
class Memory {
public:
    MemoryDataType data[MEMORY_SIZE];

    MemoryDataType read_data(int address);
    void write_data(int address, MemoryDataType data);

    MemoryDataType& operator[](int address);
};