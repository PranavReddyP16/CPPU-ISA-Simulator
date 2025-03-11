#include <iostream>
#include "cache.h"
#include "system_constants.h"

Cache::Cache(ReplacementPolicy rp, WritePolicy wp) {
    this->replacement_policy = rp;
    this->write_policy = wp;
}

int Cache::extract_offset(int address) {
    // Last bits of an address
    return address & ((1 << CACHE_LINE_OFFSET_BITS) - 1);
}

int Cache::extract_index(int address) {
    // Middle bits that weren't used for offset
    return (address >> CACHE_LINE_OFFSET_BITS) & ((1<<CACHE_LINE_INDEX_BITS) - 1);
}

int Cache::extract_tag(int address) {
    // First bits not used in index or offset
    return address >> (CACHE_LINE_INDEX_BITS + CACHE_LINE_OFFSET_BITS);
}

void Cache::show_cache() {
    for (int set_index = 0; set_index < CACHE_NUM_SETS; set_index++) {
        std::cout << "Set number " << set_index << ":" << std::endl;

        std::cout<<"    ";
        for (CacheLine cur_cache_line : this->cache_sets[set_index]) {
            std::cout << "Tag: "<<cur_cache_line.tag << ", Data: ";
            for (MemoryDataType datum : cur_cache_line.data) {
                print_datum(datum);
            }
        }
        std::cout<<std::endl;
    }
}

MemoryDataType Cache::read_data(int address) {
    int address_set_index = extract_index(address);
    int address_tag = extract_tag(address);
    int address_offset = extract_offset(address);

    for (CacheLine cur_cache_line : this->cache_sets[address_set_index]) {
        if (address_tag == cur_cache_line.tag) {
            return cur_cache_line[address_offset];
        }
    }
}