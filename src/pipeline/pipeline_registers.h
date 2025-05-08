#pragma once
#include <cstdint>

/* ──────────────────────────────────────────────────────────────
   A single latch between pipeline stages.
   All register indices are now 32-bit ints (not uint8_t) so they
   print correctly and avoid “GPR[]” in the console.
   ──────────────────────────────────────────────────────────── */
struct pipeline_reg
{
    uint64_t instruction;     // full 64-bit word fetched
    int      PC;              // PC at this stage
    int      opcode;          // 6-bit opcode (extended to int)

    int      rs1;             // decoded register fields
    int      rs2;
    int      rd;

    bool     mem_write;       // is this stage a store ?
    uint32_t mem_address;     // effective address (load / store)
    uint32_t imm;             // 32-bit immediate (for LI / LSHIFT etc.)

    int      write_data;      // what will be written back / to memory
    bool     reg_write;       // should WB write a GPR ?
    int      alu_res;         // ALU result (arithmetic / logic)

    /* default-construct RESET state */
    pipeline_reg()
      : instruction(0),
        PC(0), opcode(0),
        rs1(0), rs2(0), rd(0),
        mem_write(false), mem_address(0),
        imm(0),
        write_data(0),
        reg_write(false),
        alu_res(0) {}
};
