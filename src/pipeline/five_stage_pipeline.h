#pragma once

#include "system_constants.h"
#include "pipeline_registers.h"
#include "memory/Register.h"
#include "memory/Memory.h"
#include "memory/Cache.h"

// Five-stage pipeline core (no GUI dependencies)
class five_stage_pipeline {
public:
    // Constructor: only needs the cache reference
    explicit five_stage_pipeline(Cache& cache);

    // Register file and cache reference
    Register    registers;
    Cache&      cache;

    // Pipeline registers: IF→ID→EX→MEM→WB
    pipeline_reg ifr,   next_ifr;
    pipeline_reg idr,   next_idr;
    pipeline_reg exr,   next_exr;
    pipeline_reg memr,  next_memr;
    pipeline_reg wbr,   next_wbr;

    // Valid and control flags
    bool halted    = false;
    bool if_valid  = false;
    bool id_valid  = false;
    bool ex_valid  = false;
    bool mem_valid = false;
    bool wb_valid  = false;

    // Stage methods
    void fetch();
    void decode();
    void execute();
    void mem_stage();
    void writeback();

    // Single-cycle tick
    void clock_cycle();

    // Run continuously until the pipeline drains or halts
    void run_pipeline();
};
