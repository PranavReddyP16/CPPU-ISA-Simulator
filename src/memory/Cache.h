#pragma once

#include "system_constants.h"
#include "cache_set.h"
#include "cache_line.h"

class Cache {
private:
    ReplacementPolicy replacement_policy;
    WritePolicy write_policy;

    CacheSet cache_sets[CACHE_NUM_SETS];

    int extract_tag(int address);
    int extract_index(int address);
    int extract_offset(int address);

public:
    Cache(ReplacementPolicy replacement_policy, WritePolicy write_policy);
    MemoryDataType read_data(int address);
    void write_data(int address, MemoryDataType data);
};