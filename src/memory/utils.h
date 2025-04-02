#pragma once

#include "../../include/system_constants.h"

extern void print_data(MemoryDataType data, bool hex=true);

extern int mem_dtype_to_int(MemoryDataType data);

extern MemoryDataType int_to_mem_dtype(int data);