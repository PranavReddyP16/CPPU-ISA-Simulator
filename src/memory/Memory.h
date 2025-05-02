#pragma once

#include "system_constants.h"
#include <algorithm>  // for std::fill
#include <cstring>   // for std::memcpy

class Memory {
public:
    Memory() {
        // initialize to zero
        std::fill(std::begin(data), std::end(data), 0);
    }

    MemoryDataType read_data(int address) const {
        MemoryDataType value;
        std::memcpy(&value, &data[address], sizeof(MemoryDataType));
        return value;
    }

    void write_data(int address, MemoryDataType value) {
        std::memcpy(&data[address], &value, sizeof(MemoryDataType));
    }

    char& operator[](int address) {
        return data[address];
    }

private:
    char data[MEMORY_SIZE];
};
