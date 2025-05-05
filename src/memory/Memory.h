#pragma once
#include <QObject>
#include "system_constants.h"

class Memory : public QObject{
    Q_OBJECT
public:
    Memory();
    void reset();

    MemoryDataType read_data(int address);
    void write_data(int address, MemoryDataType data);

    MemoryDataType& operator[](int address);

    bool delay_en = true;

private:
    MemoryDataType data[MEMORY_SIZE];

signals:
    void memoryAccessed();
};
