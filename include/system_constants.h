#pragma once

#include <cstddef>
#include <cmath>
#include <iostream>

// Memory
constexpr int MEMORY_ADDRESS_BITS = 32;
// TODO increase memory
constexpr int MEMORY_SIZE = 16 * 1024 * 1024; // 16 MB
using MemoryDataType = std::byte;

// Cache
// Note: keep all of these powers of 2 to make it easier
constexpr int CACHE_SIZE = 16 * 1024; // 16 KB
constexpr int CACHE_LINE_SIZE = 64; // 64 bytes per block
constexpr int CACHE_ASSOCIATIVITY = 4; // 4 way set associative
constexpr int CACHE_NUM_SETS = CACHE_SIZE / CACHE_LINE_SIZE; // Number of sets in the cache

constexpr int CACHE_LINE_OFFSET_BITS = __builtin_ctz(CACHE_LINE_SIZE); // log2(line size)
constexpr int CACHE_LINE_INDEX_BITS = __builtin_ctz(CACHE_NUM_SETS); // log2(num_sets)
constexpr int CACHE_LINE_TAG_BITS = MEMORY_ADDRESS_BITS - \
    (CACHE_LINE_OFFSET_BITS + CACHE_LINE_INDEX_BITS); // remaining bits in the address are used for the tag

enum class ReplacementPolicy {
    LRU,
    FIFO
};

enum class WritePolicy {
    WRITE_BACK,
    WRITE_THROUGH
};

void print_datum(MemoryDataType datum, bool hex = true) {
    if(hex) std::cout << std::hex << std::to_integer<int>(datum) << " ";
    else std::cout << std::to_integer<int>(datum) << " ";
}
