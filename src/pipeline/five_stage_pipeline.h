#pragma once
#include "system_constants.h"
#include "pipeline_registers.h"
#include "memory/Register.h"
#include "memory/Memory.h"
#include "memory/Cache.h"
#include "gui/mainwindow.h"

class five_stage_pipeline{
    public:
        Register registers;
        Cache& cache;
        pipeline_reg ifr,next_ifr,idr,next_idr,exr,next_exr,memr,next_memr,wbr,next_wbr;
        bool halted = false;
        bool if_valid = false, id_valid = false, ex_valid = false, mem_valid = false, wb_valid = false;

        five_stage_pipeline(Cache& cache, MainWindow* mainWindow);

        

        void fetch();
        void decode();
        void execute();
        void mem_stage();
        void writeback();
        void clock_cycle();

        void run_pipeline();
    private:
        MainWindow* mainWindow;
};