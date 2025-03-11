#pragma once

#include "cache_line.h"
#include "system_constants.h"

class CacheSet {
public:
    CacheLine cache_lines[CACHE_ASSOCIATIVITY];

    CacheLine& operator[](int index);
};