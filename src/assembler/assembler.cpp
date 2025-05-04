#include "assembler.h"
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "memory/Memory.h"

/* helper: read next integer, tolerate a trailing “,” */
static uint64_t next_int(std::istringstream &iss)
{
    std::string tok;
    if (!(iss >> tok)) throw std::runtime_error("Assembler: missing / bad operand");

    if (tok.back()==',') tok.pop_back();      // drop comma
    return std::stoull(tok, nullptr, 0);      // auto 0x / decimal detect
}

Assembler::Assembler(Memory *m): mem(m) {}
Assembler::~Assembler() = default;

/* ─── assemble one line ─────────────────────────────────────────── */
uint64_t Assembler::assembleInstrxn(const std::string &line)
{
    std::istringstream iss(line);
    std::string op;  iss >> op;

    auto it = opcodeMap.find(op);
    if (it==opcodeMap.end()) throw std::runtime_error("Unknown instruction: "+op);

    uint64_t enc = uint64_t(it->second) << 58;   // opcode

    if (op=="LI") {                        //  LI rd, imm
        uint64_t rd  = next_int(iss);
        uint64_t imm = next_int(iss);
        enc |= (rd &7) << 55;
        enc |=  imm   & 0x007FFFFFFFFFFFFFULL;
    }
    else if (op=="ILOAD"||op=="FLOAD"||op=="SLOAD") {
        uint64_t rd   = next_int(iss);
        uint64_t addr = next_int(iss);
        uint64_t len  = next_int(iss);
        enc |= (rd &7) << 55;
        enc |= (addr &0xFFFFFFFFULL) << 23;
        enc |=  len &0xFF;
    }
    else if (op=="ISTORE"||op=="FSTORE"||op=="SSTORE") {
        uint64_t rs1  = next_int(iss);
        uint64_t addr = next_int(iss);
        uint64_t len  = next_int(iss);
        enc |= (rs1 &7) << 55;
        enc |= (addr &0xFFFFFFFFULL) << 23;
        enc |=  len &0xFF;
    }
    else if (op=="CMP"||op=="ADD"||op=="FADD"||
             op=="SUB"||op=="FSUB"||op=="MOD"||
             op=="AND"||op=="OR") {
        uint64_t rd  = next_int(iss);
        uint64_t rs2 = next_int(iss);
        enc |= (rd  &7)<<55;              // rd = rs1
        enc |= (rs2 &7)<<52;
    }
    else if (op=="MUL"||op=="FMUL"||op=="DIV"||op=="FDIV") {
        uint64_t rd  = next_int(iss);
        uint64_t rs1 = next_int(iss);
        uint64_t rs2 = next_int(iss);
        enc |= (rd  &7)<<55;
        enc |= (rs1 &7)<<52;
        enc |= (rs2 &7)<<49;
    }
    else if (op=="LSHIFT"||op=="RSHIFT"||op=="SETBIT"||op=="CLRIT") {
        uint64_t r1     = next_int(iss);
        uint64_t amount = next_int(iss);
        uint64_t flag   = (op=="LSHIFT"||op=="RSHIFT") ? next_int(iss) : 0;
        enc |= (r1 &7)<<55;
        enc |= (amount &0xFF)<<47;
        enc |= (flag &1)<<46;
    }
    else if (op=="GETBIT") {
        uint64_t r1  = next_int(iss);
        uint64_t bit = next_int(iss);
        uint64_t r2  = next_int(iss);
        enc |= (r1 &7)<<55;
        enc |= (bit &0xFF)<<47;
        enc |= (r2 &7)<<44;
    }
    else if (op=="POPCNT"||op=="NOT") {
        uint64_t r1 = next_int(iss);
        uint64_t r2 = (op=="POPCNT") ? next_int(iss) : 0;
        enc |= (r1 &7)<<55;
        enc |= (r2 &7)<<52;
    }

    return enc;
}

/* ─── load full file ────────────────────────────────────────────── */
void Assembler::loadProgram(const std::string &file)
{
    std::ifstream f(file);
    if (!f) throw std::runtime_error("Cannot open "+file);

    instrxnEntries.clear();
    std::string ln;
    while (std::getline(f,ln))
    {
        if (ln.empty()||ln[0]==';'||ln[0]=='#') 
            instrxnEntries.push_back({ln, std::nullopt});
        else 
            instrxnEntries.push_back({ln, assembleInstrxn(ln)});
    }
    f.close();

    int addr = 0;
    for (size_t i=0;i<instrxnEntries.size();++i)
        if (instrxnEntries[i].encoded.has_value())
            mem->write_data(addr++, instrxnEntries[i].encoded.value());
}
