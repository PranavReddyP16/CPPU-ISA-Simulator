#include <stdexcept>
#include "cache_set.h"
#include "cache_line.h"

CacheLine& CacheSet::operator[](int index) {
    if(index >= CACHE_ASSOCIATIVITY) {
        throw std::runtime_error("Trying to access set index greater than cache associativity\n");
    }

    return this->cache_lines[index];
}