#include <iostream>
#include "system_constants.h"

void print_data(MemoryDataType data, bool hex = true) {
    if(hex) std::cout << std::hex << std::to_integer<int>(data) << " ";
    else std::cout << std::to_integer<int>(data) << " ";
}

int mem_dtype_to_int(MemoryDataType data) {
    return std::to_integer<int> (data);
}

MemoryDataType int_to_mem_dtype(int data) {
    return static_cast<MemoryDataType> (data);
}