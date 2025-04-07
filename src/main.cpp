#include <iostream>
#include "system_constants.h"
#include "memory/Memory.h"
#include "memory/utils.h"
#include "memory/Cache.h"
#include "pipeline/five_stage_pipeline.h"

#include <QApplication>
#include <QTimer>
#include <QtConcurrent>
#include "gui/mainwindow.h"

using namespace std;

int main(int argc, char* argv[]) {
    // cout<<"Main program has started"<<endl;

    // cout<<"Creating memory"<<endl;
    // Memory mem;
    // cout<<"Writing data to memory"<<endl;
    // mem.write_data(5, int_to_mem_dtype(5));
    // cout<<"Data at the written location is now: "<<mem_dtype_to_int(mem.read_data(5))<<endl;

    // cout<<endl;
    // cout<<endl;
    // cout<<endl;

    // Cache cache(mem, ReplacementPolicy::LRU);
    // cout<<"Writing data to memory at address 500"<<endl;
    // mem.write_data(500, int_to_mem_dtype(1));
    // cout<<"Fetching from cache"<<endl;
    // mem.write_data(100, int_to_mem_dtype(1));
    // cout<<"Memory access attempt made while another request is still in progress! Wait!"<<endl;
    // mem.write_data(100, int_to_mem_dtype(1));
    // cout<<"Memory access attempt made while another request is still in progress! Wait!"<<endl;
    // mem.write_data(100, int_to_mem_dtype(1));
    // cout<<"Memory access attempt made while another request is still in progress! Wait!"<<endl;
    // mem.write_data(100, int_to_mem_dtype(1));
    // cout<<"Memory access attempt made while another request is still in progress! Wait!"<<endl;
    // cout<<mem_dtype_to_int(cache.read_data(500))<<endl;

    // cout<<endl;
    // cout<<endl;
    // cout<<endl;

    // cout<<"Writing to address already loaded in cache"<<endl;
    // cache.write_data(500, int_to_mem_dtype(2));
    // int cache_data = mem_dtype_to_int(cache.read_data(500));
    // cout<<"Data in cache is now: "<<cache_data<<endl;
    // cout<<"However, data in memory is still "<<mem_dtype_to_int(mem.read_data(500))<<endl;

    // cout<<endl;
    // cout<<endl;
    // cout<<endl;

    // cout<<"Now we will get address 500 evicted"<<endl;
    // cout<<"We will read from 4 more addresses (564, 628, 692, 756) that will clash with 500 and get it evicted"<<endl;
    // cache.read_data(564);
    // cache.write_data(564, int_to_mem_dtype(10));
    // cache.read_data(628);
    // cache.write_data(628, int_to_mem_dtype(11));
    // cache.read_data(692);
    // cache.write_data(692, int_to_mem_dtype(12));
    // cache.read_data(756);
    // cache.write_data(756, int_to_mem_dtype(13));
    // cout<<"Now let's take a look at the full cache"<<endl;
    // cache.show_cache();
    // cout<<"Now, since the cache line was evicted and it had the dirty bit set, the memory at address 500 has been written and is: "<<mem_dtype_to_int(mem.read_data(500))<<endl;

    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    QTimer::singleShot(1, [&mainWindow]() { QtConcurrent::run([&mainWindow]() {
        cout << "Main program has started" << endl;
        cout << "Creating memory" << endl;
        Memory mem(&mainWindow);
        
        cout << "Creating cache" << endl;
        Cache cache(mem, ReplacementPolicy::LRU,&mainWindow);
        int instruction = 63;
        cache.write_data(1, int_to_mem_dtype(instruction));
        cache.read_data(0);
        cache.read_data(1);
        cache.read_data(2);
        cache.read_data(3);
        cout << "Initializing and running pipeline..." << endl;
        five_stage_pipeline pipeline_1(cache,&mainWindow);
        pipeline_1.run_pipeline();
        cout << "Pipeline execution completed." << endl;

        // Cache cache(mem, ReplacementPolicy::LRU);
        instruction = 127;
        cache.write_data(1, int_to_mem_dtype(instruction));
        cache.read_data(0);
        cache.read_data(1);
        cache.read_data(2);
        cache.read_data(3);
        cout << "Initializing and running pipeline..." << endl;
        // five_stage_pipeline pipeline_2(cache);
        pipeline_1.run_pipeline();
        cout << "Pipeline execution completed." << endl;

        instruction = 191;
        cache.write_data(1, int_to_mem_dtype(instruction));
        cache.read_data(0);
        cache.read_data(1);
        cache.read_data(2);
        cache.read_data(3);
        cout << "Initializing and running pipeline..." << endl;
        // five_stage_pipeline pipeline_3(cache);
        pipeline_1.run_pipeline();
        cout << "Pipeline execution completed." << endl;
        // instruction = 2;
        // cache.write_data(2, int_to_mem_dtype(instruction));
        // cout << "Initializing and running pipeline..." << endl;
        // five_stage_pipeline pipeline_2(cache);
        // pipeline_2.run_pipeline();
        // cout << "Pipeline execution completed." << endl;


        // instruction = std::stoi(argv[1]);
        // instruction = 3;
        // cache.write_data(3, int_to_mem_dtype(instruction));
        // cout << "Initializing and running pipeline..." << endl;
        // five_stage_pipeline pipeline_3(cache);
        // pipeline_3.run_pipeline();
        // cout << "Pipeline execution completed." << endl;


        // Write instruction to memory at address 0
        // cout << "Writing instruction " << instruction<<":"<<mem_dtype_to_int(int_to_mem_dtype(instruction)) << " to memory address 0x1000" << endl;
        // cache.write_data(4096, int_to_mem_dtype(instruction));
        // std::cout << "After write: " << mem_dtype_to_int(cache.read_data(4096)) << std::endl;

        // Instantiate pipeline and run it
        
        // five_stage_pipeline pipeline(cache);
        // pipeline.run_pipeline();
        cache.read_data(0);
        cache.read_data(1);
        cache.read_data(2);
        cache.read_data(3);
        cout << "Pipeline execution completed." << endl;
    }); });
 
    return app.exec();
}