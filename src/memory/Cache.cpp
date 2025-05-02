#include <iostream>
#include <climits>
#include "Cache.h"
#include "system_constants.h"
#include "Memory.h"
#include "utils.h"

static int clock_cycle = 0;

// Constructor: zero out all cache lines
Cache::Cache(Memory& main_memory, ReplacementPolicy rp)
  : main_memory(main_memory),
    replacement_policy(rp)
{
    for (auto &cache_set : cache_sets)
        for (auto &cache_line : cache_set)
            for (auto &d : cache_line.data)
                d = int_to_mem_dtype(0);
}

// Extract offset = low bits
int Cache::extract_offset(int address) {
    return address & ((1 << CACHE_LINE_OFFSET_BITS) - 1);
}

// Extract index = middle bits
int Cache::extract_index(int address) {
    return (address >> CACHE_LINE_OFFSET_BITS)
           & ((1 << CACHE_LINE_INDEX_BITS) - 1);
}

// Extract tag = high bits
int Cache::extract_tag(int address) {
    return address >> (CACHE_LINE_INDEX_BITS + CACHE_LINE_OFFSET_BITS);
}

// Console‐only display of cache contents
void Cache::show_cache() {
    for (int set_index = 0; set_index < CACHE_NUM_SETS; ++set_index) {
        std::cout << "Set " << set_index << ":\n";
        for (const auto &line : cache_sets[set_index]) {
            std::cout << "  Tag=" << line.tag << " Data=[";
            for (auto datum : line.data) {
                print_data(datum);
                std::cout << " ";
            }
            std::cout << "]\n";
        }
        std::cout << "\n";
    }
}

// Find eviction candidate in a set (LRU / first‐invalid)
int Cache::find_cache_line_to_evict_from_set(CacheSet& cache_set) {
    int best_i = 0, best_time = INT_MAX;
    for (int i = 0; i < CACHE_ASSOCIATIVITY; ++i) {
        auto &ln = cache_set[i];        // now allowed on non‐const
        if (!ln.valid) return i;
        if (ln.last_used < best_time) {
            best_time = ln.last_used;
            best_i = i;
        }
    }
    return best_i;
}

// Evict & refill one cache line
void Cache::evict_and_replace_cache_line(CacheSet& cache_set,
                                         int      set_idx,
                                         int      mem_base_addr)
{
    int idx = find_cache_line_to_evict_from_set(cache_set);
    auto &line = cache_set[idx];

    // Write-back if dirty
    if (line.dirty) {
        int wb_addr = (line.tag << (CACHE_LINE_INDEX_BITS + CACHE_LINE_OFFSET_BITS))
                      | (set_idx << CACHE_LINE_OFFSET_BITS);
        for (int i = 0; i < CACHE_LINE_SIZE; ++i)
            main_memory.write_data(wb_addr + i, line.data[i]);
    }

    // Refill from memory
    for (int i = 0; i < CACHE_LINE_SIZE; ++i)
        line.data[i] = main_memory.read_data(mem_base_addr + i);

    line.tag       = extract_tag(mem_base_addr);
    line.valid     = true;
    line.dirty     = false;
    line.last_used = clock_cycle++;
}

// Read-through: on miss evict+refill, then return value
MemoryDataType Cache::read_data(int address) {
    int set_idx = extract_index(address);
    int tag     = extract_tag(address);
    int offset  = extract_offset(address);

    auto &set = cache_sets[set_idx];
    for (auto &ln : set) {
        if (ln.valid && ln.tag == tag) {
            ln.last_used = clock_cycle++;
            return ln.data[offset];
        }
    }
    // Miss
    int base = address - offset;
    evict_and_replace_cache_line(set, set_idx, base);
    return read_data(address);
}

// Write-through: no-allocate on miss
void Cache::write_data(int address, MemoryDataType data) {
    int set_idx = extract_index(address);
    int tag     = extract_tag(address);
    int offset  = extract_offset(address);

    auto &set = cache_sets[set_idx];
    for (auto &ln : set) {
        if (ln.valid && ln.tag == tag) {
            ln.data[offset] = data;
            ln.dirty        = true;
            ln.last_used    = clock_cycle++;
            return;
        }
    }
    // Miss: write directly to memory
    main_memory.write_data(address, data);
}
