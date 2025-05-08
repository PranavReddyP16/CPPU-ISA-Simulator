#include "Memory.h"

Memory::Memory() {
    reset();
}

void Memory::reset() {
    for (int i = 0; i < MEMORY_SIZE; ++i) {
        data[i] = 0;
    }
}

MemoryDataType Memory::read_data(int address) {
    if (delay_en) emit memoryAccessed();
    return data[address];
}

void Memory::write_data(int address, MemoryDataType value) {
    if (delay_en) emit memoryAccessed();
    data[address] = value;
}

MemoryDataType& Memory::operator[](int address) {
    return data[address];
}
