#include "Memory.h"
#include "system_constants.h"
#include <iostream>

Memory::Memory(MainWindow* mainWindow) {
    this->mainWindow = mainWindow;
}

MemoryDataType Memory::read_data(int address) {
    return this->data[address];
}

void Memory::write_data(int address, MemoryDataType data) {
    this->data[address] = data;

    // update_gui();
    mainWindow->memoryTable->setItem(address / 16, address % 16 + 1, new QTableWidgetItem(QString("%1").arg(data, 4, 16, QChar('0')).toUpper()));
    mainWindow->memoryTable->setCurrentCell(address / 16, address % 16 + 1);
    mainWindow->memoryTable->scrollToItem(mainWindow->memoryTable->currentItem());
    mainWindow->memoryTable->resizeColumnsToContents();
}

MemoryDataType& Memory::operator[](int address) {
    return this->data[address];
}
