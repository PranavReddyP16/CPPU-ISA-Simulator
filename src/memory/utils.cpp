#include <iostream>
#include "utils.h"
// #include "../../include/system_constants.h"
#include "system_constants.h"

void print_data(MemoryDataType data, bool hex) {
    // if(hex) std::cout << std::hex << std::to_integer<int>(data) << " ";
    // else std::cout << std::to_integer<int>(data) << " ";
    if(hex) std::cout << std::hex <<(data) << " ";
    else std::cout <<(data) << " ";
}

int64_t mem_dtype_to_int(MemoryDataType data) {
    // return std::to_integer<int> (data);
    return data;
}

MemoryDataType int_to_mem_dtype(int64_t data) {
    // return static_cast<MemoryDataType> (data);
    return data;
}