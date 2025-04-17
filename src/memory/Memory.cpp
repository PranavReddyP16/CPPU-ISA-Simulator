#include "Memory.h"
#include "system_constants.h"

Memory::Memory(MainWindow* mainWindow) {
    this->mainWindow = mainWindow;
    update_gui();
 }

MemoryDataType Memory::read_data(int address) {
    return this->data[address];
}

void Memory::write_data(int address, MemoryDataType data) {
    this->data[address] = data;
    update_gui();
}

MemoryDataType& Memory::operator[](int address) {
    return this->data[address];
}

void Memory::update_gui() {
    mainWindow->memoryTable->setRowCount(MEMORY_SIZE);
    for (int i = 0; i < MEMORY_SIZE; i++) {
        // Assuming MemoryDataType is int for simplicity, change accordingly
        std::int64_t value = this->data[i]; // or however you want to display it
        mainWindow->memoryTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        mainWindow->memoryTable->setItem(i, 1, new QTableWidgetItem(QString::number(static_cast<int>(value))));
    }
}