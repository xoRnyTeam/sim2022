
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
    CSRRC,
    CSRRCI,
    CSRRS,
    CSRRSI,
    CSRRW,
    CSRRWI,
    DIV,
    DIVU,
    EBREAK,
    ECALL,
    FENCE,
    JAL,
    JALR,
    LB,
    LBU,
    LH,
    LHU,
    LUI,
    LW,
    MUL,
    MULH,
    MULHSU,
    MULHU,
    OR,
    ORI,
    REM,
    REMU,
    SB,
    SH,
    SLL,
    SLLI,
    SLT,
    SLTI,
    SLTIU,
    SLTU,
    SRA,
    SRAI,
    SRL,
    SRLI,
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
    {InstrId::CSRRC, "CSRRC"},
    {InstrId::CSRRCI, "CSRRCI"},
    {InstrId::CSRRS, "CSRRS"},
    {InstrId::CSRRSI, "CSRRSI"},
    {InstrId::CSRRW, "CSRRW"},
    {InstrId::CSRRWI, "CSRRWI"},
    {InstrId::DIV, "DIV"},
    {InstrId::DIVU, "DIVU"},
    {InstrId::EBREAK, "EBREAK"},
    {InstrId::ECALL, "ECALL"},
    {InstrId::FENCE, "FENCE"},
    {InstrId::JAL, "JAL"},
    {InstrId::JALR, "JALR"},
    {InstrId::LB, "LB"},
    {InstrId::LBU, "LBU"},
    {InstrId::LH, "LH"},
    {InstrId::LHU, "LHU"},
    {InstrId::LUI, "LUI"},
    {InstrId::LW, "LW"},
    {InstrId::MUL, "MUL"},
    {InstrId::MULH, "MULH"},
    {InstrId::MULHSU, "MULHSU"},
    {InstrId::MULHU, "MULHU"},
    {InstrId::OR, "OR"},
    {InstrId::ORI, "ORI"},
    {InstrId::REM, "REM"},
    {InstrId::REMU, "REMU"},
    {InstrId::SB, "SB"},
    {InstrId::SH, "SH"},
    {InstrId::SLL, "SLL"},
    {InstrId::SLLI, "SLLI"},
    {InstrId::SLT, "SLT"},
    {InstrId::SLTI, "SLTI"},
    {InstrId::SLTIU, "SLTIU"},
    {InstrId::SLTU, "SLTU"},
    {InstrId::SRA, "SRA"},
    {InstrId::SRAI, "SRAI"},
    {InstrId::SRL, "SRL"},
    {InstrId::SRLI, "SRLI"},
    {InstrId::SUB, "SUB"},
    {InstrId::SW, "SW"},
    {InstrId::XOR, "XOR"},
    {InstrId::XORI, "XORI"},

}; //! 


} //! namespace sim


#endif //! ISA_HPP

