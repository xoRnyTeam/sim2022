
#ifndef ISA_HPP
#define ISA_HPP


#include <string>

#include <unordered_map>

namespace sim {

enum class InstrId : uint16_t {

    NONE = 0,
    ADD,
    ADDI,
    AND,
    ANDI,
    AUIPC,
    BEQ,
    BGE,
    BGEU,
    BLT,
    BLTU,
    BNE,
    EBREAK,
    ECALL,
    FENCE,
    FENCE_TSO,
    JAL,
    JALR,
    LB,
    LBU,
    LH,
    LHU,
    LUI,
    LW,
    OR,
    ORI,
    PAUSE,
    SB,
    SBREAK,
    SCALL,
    SH,
    SLL,
    SLT,
    SLTI,
    SLTIU,
    SLTU,
    SRA,
    SRL,
    SUB,
    SW,
    XOR,
    XORI,

}; //! enum class InstrId


inline std::unordered_map<InstrId, std::string> InstrId2str {

    {InstrId::ADD, "ADD"},
    {InstrId::ADDI, "ADDI"},
    {InstrId::AND, "AND"},
    {InstrId::ANDI, "ANDI"},
    {InstrId::AUIPC, "AUIPC"},
    {InstrId::BEQ, "BEQ"},
    {InstrId::BGE, "BGE"},
    {InstrId::BGEU, "BGEU"},
    {InstrId::BLT, "BLT"},
    {InstrId::BLTU, "BLTU"},
    {InstrId::BNE, "BNE"},
    {InstrId::EBREAK, "EBREAK"},
    {InstrId::ECALL, "ECALL"},
    {InstrId::FENCE, "FENCE"},
    {InstrId::FENCE_TSO, "FENCE_TSO"},
    {InstrId::JAL, "JAL"},
    {InstrId::JALR, "JALR"},
    {InstrId::LB, "LB"},
    {InstrId::LBU, "LBU"},
    {InstrId::LH, "LH"},
    {InstrId::LHU, "LHU"},
    {InstrId::LUI, "LUI"},
    {InstrId::LW, "LW"},
    {InstrId::OR, "OR"},
    {InstrId::ORI, "ORI"},
    {InstrId::PAUSE, "PAUSE"},
    {InstrId::SB, "SB"},
    {InstrId::SBREAK, "SBREAK"},
    {InstrId::SCALL, "SCALL"},
    {InstrId::SH, "SH"},
    {InstrId::SLL, "SLL"},
    {InstrId::SLT, "SLT"},
    {InstrId::SLTI, "SLTI"},
    {InstrId::SLTIU, "SLTIU"},
    {InstrId::SLTU, "SLTU"},
    {InstrId::SRA, "SRA"},
    {InstrId::SRL, "SRL"},
    {InstrId::SUB, "SUB"},
    {InstrId::SW, "SW"},
    {InstrId::XOR, "XOR"},
    {InstrId::XORI, "XORI"},

}; //! 


} //! namespace sim


#endif //! ISA_HPP

