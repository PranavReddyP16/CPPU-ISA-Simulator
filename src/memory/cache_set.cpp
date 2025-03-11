#include <stdexcept>
#include "cache_set.h"
#include "cache_line.h"
#include "system_constants.h"

CacheLine& CacheSet::operator[](int index) {
    if(index >= CACHE_ASSOCIATIVITY) {
        throw std::runtime_error("Trying to access set index greater than cache associativity\n");
    }

    return this->cache_lines[index];
}

CacheLine* CacheSet::begin() {
    return this->cache_lines;
}

CacheLine* CacheSet::end() {
    return this->cache_lines+CACHE_NUM_SETS;
}

const CacheLine* CacheSet::begin() const {
    return this->cache_lines;
}

const CacheLine* CacheSet::end() const {
    return this->cache_lines+CACHE_NUM_SETS;
}