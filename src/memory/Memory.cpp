#include "Memory.h"
#include "system_constants.h"

Memory::Memory(MainWindow* mainWindow) {
    this->mainWindow = mainWindow;
}

MemoryDataType Memory::read_data(int address) {
    return this->data[address];
}

void Memory::write_data(int address, MemoryDataType data) {
    this->data[address] = data;

    // update_gui();
    
}

MemoryDataType& Memory::operator[](int address) {
    return this->data[address];
}
