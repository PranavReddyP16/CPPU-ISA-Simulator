#pragma once

#include "system_constants.h"
#include "cache_set.h"
#include "cache_line.h"

class Cache {
private:
    ReplacementPolicy replacement_policy;
    WritePolicy write_policy;

    CacheSet cache_sets[CACHE_NUM_SETS];

    // Address is broken down into Tag (18 bits) | Index (8 bits) | Offset (6 bits)
    // Based on number of sets and cache line size
    int extract_tag(int address);
    int extract_index(int address);
    int extract_offset(int address);

public:
    Cache(ReplacementPolicy replacement_policy, WritePolicy write_policy);
    MemoryDataType read_data(int address);
    void write_data(int address, MemoryDataType data);
    void show_cache();
};