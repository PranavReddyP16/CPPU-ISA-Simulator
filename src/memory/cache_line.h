#pragma once

#include <stdint.h>
#include "system_constants.h"

class CacheLine {
private:
    int last_used;

public:
    int tag;
    MemoryDataType data[CACHE_LINE_SIZE]; // Needs to be public for GUI access

    CacheLine(int tag);
    CacheLine(int tag, int last_used);

    MemoryDataType& operator[](int offset);

    void update_last_used(int time);

    template<typename T = uint8_t>
    MemoryDataType read_data(T offset);

    template<typename T = uint8_t>
    void write_data(T offset, MemoryDataType data);
};