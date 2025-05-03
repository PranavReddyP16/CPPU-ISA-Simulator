#include "Cache.h"
#include "utils.h"
#include <iostream>
#include <climits>

Cache::Cache(Memory &main_memory, ReplacementPolicy rp)
  : main_memory(main_memory), replacement_policy(rp)
{
    enabled = true;
    reset();
}

void Cache::reset() {
    // Invalidate all lines and zero them out
    for (int s = 0; s < CACHE_NUM_SETS; ++s) {
        for (int w = 0; w < CACHE_ASSOCIATIVITY; ++w) {
            auto &line = cache_sets[s][w];
            line.valid     = false;
            line.dirty     = false;
            line.last_used = 0;
            line.tag       = 0;
            for (int b = 0; b < CACHE_LINE_SIZE; ++b)
                line.data[b] = 0;
        }
    }
}

int Cache::extract_offset(int address) const {
    return address & ((1 << CACHE_LINE_OFFSET_BITS) - 1);
}

int Cache::extract_index(int address) const {
    return (address >> CACHE_LINE_OFFSET_BITS) & ((1 << CACHE_LINE_INDEX_BITS) - 1);
}

int Cache::extract_tag(int address) const {
    return address >> (CACHE_LINE_OFFSET_BITS + CACHE_LINE_INDEX_BITS);
}

int Cache::find_cache_line_to_evict_from_set(int set_index) const {
    // LRU: pick the line with smallest last_used, or an invalid one
    int best = 0;
    int best_time = INT_MAX;
    for (int i = 0; i < CACHE_ASSOCIATIVITY; ++i) {
        const auto &L = cache_sets[set_index][i];
        if (!L.valid) {
            return i;
        }
        if (L.last_used < best_time) {
            best_time = L.last_used;
            best = i;
        }
    }
    return best;
}

void Cache::evict_and_replace_cache_line(int set_index, int start_addr) {
    int way = find_cache_line_to_evict_from_set(set_index);
    auto &L = cache_sets[set_index][way];

    // Write back if dirty
    if (L.dirty) {
        int base = (L.tag << (CACHE_LINE_INDEX_BITS + CACHE_LINE_OFFSET_BITS))
                 | (set_index << CACHE_LINE_OFFSET_BITS);
        for (int b = 0; b < CACHE_LINE_SIZE; ++b) {
            main_memory.write_data(base + b, L.data[b]);
        }
    }

    // Refill
    L.tag       = extract_tag(start_addr);
    L.valid     = true;
    L.dirty     = false;
    static int global_clock = 0;
    L.last_used = global_clock++;

    for (int b = 0; b < CACHE_LINE_SIZE; ++b) {
        L.data[b] = main_memory.read_data(start_addr + b);
    }
}

MemoryDataType Cache::read_data(int address) {
    if (!enabled)
        return main_memory.read_data(address);

    int set_index = extract_index(address);
    int tag       = extract_tag(address);
    int offset    = extract_offset(address);

    // Hit?
    for (auto &L : cache_sets[set_index]) {
        if (L.valid && L.tag == tag) {
            static int global_clock = 0;
            L.last_used = global_clock++;
            return L.data[offset];
        }
    }
    // Miss:
    int base = address - offset;
    evict_and_replace_cache_line(set_index, base);
    return read_data(address);  // now it's a hit
}

void Cache::write_data(int address, MemoryDataType value) {
    if (!enabled)
        return main_memory.write_data(address, value);

    int set_index = extract_index(address);
    int tag       = extract_tag(address);
    int offset    = extract_offset(address);

    // Hit?
    for (auto &L : cache_sets[set_index]) {
        if (L.valid && L.tag == tag) {
            static int global_clock = 0;
            L.last_used = global_clock++;
            L.data[offset] = value;
            L.dirty = true;
            return;
        }
    }
    // Miss: bring in then write
    int base = address - offset;
    evict_and_replace_cache_line(set_index, base);
    write_data(address, value);
}
