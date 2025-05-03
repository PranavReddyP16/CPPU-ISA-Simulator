#pragma once

#include "system_constants.h"
#include "cache_set.h"
#include "cache_line.h"
#include "Memory.h"

class Cache {
public:
    Cache(Memory &main_memory, ReplacementPolicy rp);
    void reset();

    MemoryDataType read_data(int address);
    void write_data(int address, MemoryDataType data);

    const CacheSet& getSet(int index) const { return cache_sets[index]; }
    static int numSets() { return CACHE_NUM_SETS; }

private:
    Memory &main_memory;
    ReplacementPolicy replacement_policy;
    CacheSet cache_sets[CACHE_NUM_SETS];

    int extract_tag(int address)     const;
    int extract_index(int address)   const;
    int extract_offset(int address)  const;

    int find_cache_line_to_evict_from_set(int set_index) const;
    void evict_and_replace_cache_line(int set_index, int start_addr);

    // No GUI calls in here—view is driven from mainwindow::refreshGui()
};
