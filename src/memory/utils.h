#pragma once

#include "system_constants.h"

void print_data(MemoryDataType data, bool hex = true);

int mem_dtype_to_int(MemoryDataType data);

MemoryDataType int_to_mem_dtype(int data);