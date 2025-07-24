# CPPU-ISA-Simulator

## Overview
CPU ISA Simulator designed with Competitive Programming heavy workloads in mind which include:
- Large string manipulation and operations
- Heavy I/O
- Fast and repeated math and bit operations

## Implementation
- Implemented the popular 5-stage architecture (Fetch, Decode, Execute, Memory, Writeback)
- Used multiple 4-way set associative caches with different eviction algorithms depending on the use case
- Direct Memory Access I/O
- Added ALU operations for certain bitwise operators
- Added an extra register specifically to store single repeated values (ex: large prime numbers used for modulo operations)
- Clock cycles were simulated based on real world metrics (ex: 1 cycle for cache hit, 100 cycles for cache miss / RAM access, etc.)

## Results
- Observed 30% - 50% increases in clock cycle performance when benchmarked on some popular competitive programming problems

Make sure to install the following packages using the method best suited for your operating system:
- C++ 17 (we are using some modern features like std::byte for example)
- Qt6
- GoogleTest (Not needed for now, just in case we need to write tests in the future)

Build instructions:
- From project root directory,
- mkdir build && cd build
- cmake ..
- make
- ./ISA_Simulator
