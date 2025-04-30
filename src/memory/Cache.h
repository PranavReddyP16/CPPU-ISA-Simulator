#pragma once

#include "system_constants.h"
#include "cache_set.h"
#include "cache_line.h"
#include "Memory.h"

class Cache {
public:
    // ctor
    Cache(Memory& main_memory, ReplacementPolicy rp);

    MemoryDataType read_data(int address);
    void write_data(int address, MemoryDataType data);

    void show_cache();  // Console dump

    // NEW: GUI inspection
    static constexpr int numSets() { return CACHE_NUM_SETS; }
    const CacheSet& getSet(int idx) const { return cache_sets[idx]; }

private:
    Memory&              main_memory;
    ReplacementPolicy    replacement_policy;
    CacheSet             cache_sets[CACHE_NUM_SETS];

    int extract_tag(int address);
    int extract_index(int address);
    int extract_offset(int address);

    int find_cache_line_to_evict_from_set(CacheSet& cache_set);
    void evict_and_replace_cache_line(CacheSet& cache_set,
                                      int set_idx,
                                      int base_addr);
};
