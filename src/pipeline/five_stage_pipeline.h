#pragma once
#include "../../include/system_constants.h"
#include "pipeline_registers.h"
#include "../memory/Register.h"
#include "../memory/Memory.h"
#include "../memory/Cache.h"

class five_stage_pipeline{
    public:
        Register registers;
        Cache& cache;
        if_id_reg if_id;
        id_ex_reg id_ex;
        ex_mem_reg ex_mem;
        mem_wb_reg mem_wb;

        five_stage_pipeline(Cache& cache);

        

        void fetch();
        void decode();
        void execute();
        void mem_stage();
        void writeback();

        void run_pipeline();
};