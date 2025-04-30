#include "Memory.h"

Memory::Memory() {
    reset();
}

void Memory::reset() {
    for (int i = 0; i < MEMORY_SIZE; ++i) {
        data[i] = 0;
    }
}

MemoryDataType Memory::read_data(int address) const {
    return data[address];
}

void Memory::write_data(int address, MemoryDataType value) {
    data[address] = value;
}

MemoryDataType& Memory::operator[](int address) {
    return data[address];
}
