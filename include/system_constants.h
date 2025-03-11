#pragma once

#include<cstddef>

// Memory
constexpr int MEMORY_ADDRESS_BITS = 32;
constexpr int MEMORY_SIZE = 16 * 1024 * 1024; // 16 MB
using MemoryDataType = std::byte;

// Cache
constexpr int CACHE_SIZE = 16 * 1024; // 16 KB
constexpr int CACHE_LINE_SIZE = 64; // 64 bytes per block
constexpr int CACHE_ASSOCIATIVITY = 4; // 4 way set associative
constexpr int CACHE_NUM_SETS = CACHE_SIZE / CACHE_LINE_SIZE; // Number of sets in the cache

constexpr int CACHE_LINE_OFFSET_BITS = 6; // log2(block size)
constexpr int CACHE_LINE_INDEX_BITS = 6; // log2(num_sets)
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
