#pragma once

#include "system_constants.h"
#include <algorithm>  // for std::fill

class Memory {
public:
    Memory() {
        // initialize to zero
        std::fill(std::begin(data), std::end(data), 0);
    }

    MemoryDataType read_data(int address) const {
        return data[address];
    }

    void write_data(int address, MemoryDataType value) {
        data[address] = value;
    }

    MemoryDataType& operator[](int address) {
        return data[address];
    }

private:
    MemoryDataType data[MEMORY_SIZE];
};
