#pragma once

#include "system_constants.h"
#include "cache_set.h"
#include "cache_line.h"
#include "Memory.h"
#include "gui/mainwindow.h"

class Cache {
private:
    Memory& main_memory;
    ReplacementPolicy replacement_policy;

    CacheSet cache_sets[CACHE_NUM_SETS];

    // Address is broken down into Tag (18 bits) | Index (8 bits) | Offset (6 bits)
    // Based on number of sets and cache line size
    int extract_tag(int address);
    int extract_index(int address);
    int extract_offset(int address);

    MainWindow* mainWindow;

public:
    Cache(Memory& main_memory, ReplacementPolicy rp, MainWindow* mainWindow);
    MemoryDataType read_data(int address);
    void write_data(int address, MemoryDataType data);
    void show_cache();
    int find_cache_line_to_evict_from_set(CacheSet cache_set);
    void evict_and_replace_cache_line(CacheSet& cache_set, int cache_set_index, int starting_memory_address);
};