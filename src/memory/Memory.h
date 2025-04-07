#pragma once

#include "system_constants.h"
#include "gui/mainwindow.h"

// TODO check public private stuff I'm making everything public for now
class Memory {
public:
    Memory(MainWindow* mainWindow);

    MemoryDataType read_data(int address);
    void write_data(int address, MemoryDataType data);

    MemoryDataType& operator[](int address);
    
private:
    MemoryDataType data[MEMORY_SIZE];
    MainWindow* mainWindow;
};