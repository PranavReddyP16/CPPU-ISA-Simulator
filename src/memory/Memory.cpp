#include "memory.h"
#include "system_constants.h"

MemoryDataType Memory::read_data(int address) {
    return this->data[address];
}

void Memory::write_data(int address, MemoryDataType data) {
    this->data[address] = data;
}

MemoryDataType& Memory::operator[](int address) {
    return this->data[address];
}