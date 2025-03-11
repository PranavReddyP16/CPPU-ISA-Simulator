#pragma once

#include <stdint.h>
#include "system_constants.h"

class CacheLine {
private:
    int last_used;
    MemoryDataType data[CACHE_LINE_SIZE];

public:
    int tag;
    CacheLine(int tag);
    CacheLine(int tag, int last_used);

    void update_last_used(int time);

    template<typename T = uint8_t>
    MemoryDataType read_data(T offset);

    template<typename T = uint8_t>
    void write_data(T offset, MemoryDataType data);
};