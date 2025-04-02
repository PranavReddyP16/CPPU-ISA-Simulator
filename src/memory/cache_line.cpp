#include <stdexcept>
#include "cache_line.h"
#include "../../include/system_constants.h"

CacheLine::CacheLine(){};

CacheLine::CacheLine(int tag) {
    this->tag = tag;
}

CacheLine::CacheLine(int tag, int last_used) {
    this->tag = tag;
    this->last_used = last_used;
}

MemoryDataType& CacheLine::operator[](int offset) {
    return this->data[offset];
}

void CacheLine::update_last_used(int time) {
    this->last_used = time;
}

template<typename T>
MemoryDataType CacheLine::read_data(T offset) {
    if(offset >= CACHE_LINE_SIZE) {
        throw std::runtime_error
            ("Cache line with tag %d accessed with offset %d greater than cache line size", \
            this->tag, offset);
    }
    return this->data[offset];
}

template<typename T>
void CacheLine::write_data(T offset, MemoryDataType data) {
    if(offset >= CACHE_LINE_SIZE) {
        throw std::runtime_error
            ("Cache line with tag %d accessed with offset %d greater than cache line size", \
            this->tag, offset);
    }
    this->data[offset] = data;
}