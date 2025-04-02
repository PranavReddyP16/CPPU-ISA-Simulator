#include <iostream>
#include "Cache.h"
#include "../../include/system_constants.h"
#include "Memory.h"
#include "utils.h"

int clock_cycle = 0;

Cache::Cache(Memory& main_memory, ReplacementPolicy rp) 
: main_memory(main_memory), replacement_policy(rp) {
    // Setting everything equal to 0 for printing purposes
    for (CacheSet& cache_set : this->cache_sets) {
        for (CacheLine& cache_line : cache_set) {
            for (MemoryDataType &data : cache_line.data) {
                data = int_to_mem_dtype(0);
            }
        }
    }
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

        for (CacheLine cur_cache_line : this->cache_sets[set_index]) {
            std::cout << "    Tag: "<<cur_cache_line.tag << ", Data: ";
            for (MemoryDataType datum : cur_cache_line.data) {
                print_data(datum);
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }
}

int Cache::find_cache_line_to_evict_from_set(CacheSet cache_set) {
    // TODO uncomment this when we implement FIFO replacement policy
    // if (this->replacement_policy == ReplacementPolicy::LRU) {
        int least_recently_used_time = 1e9+7;
        int least_recently_used_index = 0;
        for (int cache_line_index = 0; cache_line_index < CACHE_ASSOCIATIVITY; cache_line_index++) {
            // Always prefer removing an invalid cache line first before checking LRU
            if (cache_set[cache_line_index].valid == false) {
                least_recently_used_index = cache_line_index;
                break;
            }

            if(cache_set[cache_line_index].last_used < least_recently_used_time) {
                least_recently_used_time = cache_set[cache_line_index].last_used;
                least_recently_used_index = cache_line_index;
            }
        }

        return least_recently_used_index;
    // }
}

void Cache::evict_and_replace_cache_line(CacheSet& cache_set, int cache_set_index, int starting_memory_address) {
    int eviction_index = find_cache_line_to_evict_from_set(cache_set);
    std::cout<<"Cache line number "<<eviction_index<<" being evicted"<<std::endl;
    std::cout<<"Starting address of new line is: "<<starting_memory_address<<std::endl;

    CacheLine& eviction_line = cache_set[eviction_index];

    // Write back if dirty bit is set
    if (eviction_line.dirty) {
        int starting_wb_address = (eviction_line.tag << (CACHE_LINE_INDEX_BITS + CACHE_LINE_OFFSET_BITS))\
            | (cache_set_index << CACHE_LINE_OFFSET_BITS);
        
        // Writing back
        for (int i = 0; i < CACHE_LINE_SIZE; i++) {
            this->main_memory.write_data(starting_wb_address + i, eviction_line[i]);
        }
    }

    // Evict and repopulate
    std::cout<<"Printing new line after evicting"<<std::endl;
    for (int i = 0; i < CACHE_LINE_SIZE; i++) {
        eviction_line[i] = this->main_memory.read_data(starting_memory_address + i);
        std::cout<<mem_dtype_to_int(eviction_line[i])<<" ";
    }
    std::cout<<std::endl;
    std::cout<<std::endl;
    eviction_line.tag = extract_tag(starting_memory_address);
    eviction_line.dirty = false;
    eviction_line.valid = true;
    eviction_line.last_used = clock_cycle;
    clock_cycle++;
}

MemoryDataType Cache::read_data(int address) {
    // TODO Redundant code in read/write functions
    int address_set_index = extract_index(address);
    int address_tag = extract_tag(address);
    int address_offset = extract_offset(address);

    // Searching the set for a matching tag
    for (CacheLine &cur_cache_line : this->cache_sets[address_set_index]) {
        // Cache hit
        if (cur_cache_line.valid && address_tag == cur_cache_line.tag) {
            std::cout<<"Cache hit for address: "<<address<<std::endl;
            std::cout << "Value at address " << address << " in cache: "<< mem_dtype_to_int(cur_cache_line[address_offset]) << std::endl;
            cur_cache_line.last_used = clock_cycle;
            clock_cycle++;
            return cur_cache_line[address_offset];
        }
    }

    // Cache miss
    std::cout<<"Cache miss for address: "<<address<<std::endl;
    int starting_memory_address = address - address_offset;

    evict_and_replace_cache_line(this->cache_sets[address_set_index], address_set_index, starting_memory_address);

    // Calling the function again should now return the newly populated cache value
    return read_data(address);
}

void Cache::write_data(int address, MemoryDataType data) {
     // TODO Redundant code in read/write functions
    std::cout<<"Address: "<<address<<" "<<"Data: "<<mem_dtype_to_int(data)<<std::endl;
    int address_set_index = extract_index(address);
    int address_tag = extract_tag(address);
    int address_offset = extract_offset(address);

    // Searching the set for a matching tag
    for (CacheLine& cur_cache_line : this->cache_sets[address_set_index]) {
        // Cache hit
        if (cur_cache_line.valid && address_tag == cur_cache_line.tag) {
            cur_cache_line.last_used = clock_cycle;
            clock_cycle++;
            cur_cache_line[address_offset] = data;
            cur_cache_line.dirty = true;
            return;
        }
    }

    // Cache miss
    int starting_memory_address = address - address_offset;

    evict_and_replace_cache_line(this->cache_sets[address_set_index], address_set_index, starting_memory_address);

    // Calling the function again should now write the newly populated cache value
    write_data(address, data);
   
}