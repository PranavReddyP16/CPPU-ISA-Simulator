#include<iostream>
#include "memory/memory.h"
#include "memory/utils.h"
#include "memory/cache.h"
#include "system_constants.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout<<"Main program has started"<<endl;

    cout<<"Creating memory"<<endl;
    Memory mem;
    cout<<"Writing data to memory"<<endl;
    mem.write_data(5, int_to_mem_dtype(5));
    cout<<"Data at the written location is now: "<<mem_dtype_to_int(mem.read_data(5))<<endl;

    cout<<endl;
    cout<<endl;
    cout<<endl;

    Cache cache(mem, ReplacementPolicy::LRU);
    cout<<"Writing data to memory"<<endl;
    mem.write_data(500, int_to_mem_dtype(1));
    cout<<"Fetching from cache"<<endl;
    cout<<mem_dtype_to_int(cache.read_data(500))<<endl;

    cout<<endl;
    cout<<endl;
    cout<<endl;

    cout<<"Writing to address already loaded in cache"<<endl;
    cache.write_data(500, int_to_mem_dtype(2));
    int cache_data = mem_dtype_to_int(cache.read_data(500));
    cout<<"Data in cache is now: "<<cache_data<<endl;
    cout<<"However, data in memory is still "<<mem_dtype_to_int(mem.read_data(500))<<endl;

    cout<<endl;
    cout<<endl;
    cout<<endl;

    cout<<"Now we will get address 500 evicted"<<endl;
    cout<<"We will read from 4 more addresses (564, 628, 692, 756) that will clash with 500 and get it evicted"<<endl;
    cache.read_data(564);
    cache.read_data(628);
    cache.read_data(692);
    cache.read_data(756);
}