#pragma once

#include "cache_line.h"
#include "system_constants.h"

// A set of CACHE_ASSOCIATIVITY cache lines
class CacheSet {
public:
    // Underlying storage
    CacheLine cache_lines[CACHE_ASSOCIATIVITY];

    // Iterators
    CacheLine*             begin()       { return cache_lines; }
    CacheLine*             end()         { return cache_lines + CACHE_ASSOCIATIVITY; }
    const CacheLine*       begin() const { return cache_lines; }
    const CacheLine*       end()   const { return cache_lines + CACHE_ASSOCIATIVITY; }

    // Random access
    CacheLine&       operator[](int index)       { return cache_lines[index]; }
    const CacheLine& operator[](int index) const { return cache_lines[index]; }
};
