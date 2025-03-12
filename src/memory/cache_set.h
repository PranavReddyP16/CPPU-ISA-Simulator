#pragma once

#include "cache_line.h"
#include "system_constants.h"

class CacheSet {
public:
    CacheLine cache_lines[CACHE_ASSOCIATIVITY];

    CacheLine* begin();
    CacheLine* end();
    const CacheLine* begin() const;
    const CacheLine* end() const;
    CacheLine& operator[](int index);
};