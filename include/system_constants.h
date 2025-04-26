#pragma once

#include <cstddef>
#include <cmath>
#include <iostream>
#include <cstdint>

// Memory
constexpr int MEMORY_ADDRESS_BITS = 32;
// TODO increase memory
// constexpr int MEMORY_SIZE = 512 * 1024; // 512 KB
constexpr int MEMORY_SIZE = 1024; // 1 KB for demo
using MemoryDataType = std::int64_t;

// Cache
// Note: keep all of these powers of 2 to make it easier
// constexpr int CACHE_SIZE = 16 * 1024; // 16 KB
// constexpr int CACHE_LINE_SIZE = 64; // 64 bytes per block
constexpr int CACHE_SIZE = 256; // 256 bytes for demo
constexpr int CACHE_LINE_SIZE = 8; // 8 bytes for demo
constexpr int CACHE_ASSOCIATIVITY = 4; // 4 way set associative
constexpr int CACHE_NUM_SETS = CACHE_SIZE / (CACHE_LINE_SIZE * CACHE_ASSOCIATIVITY); // Number of sets in the cache

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
