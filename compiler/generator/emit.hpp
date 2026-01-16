//
// Created by geguj on 2025/12/28.
//

#ifndef LMX_EMIT_HPP
#define LMX_EMIT_HPP
#include <cstdint>
#include <vector>

#include "../../include/opcode.hpp"


namespace lmx {

class LMXOpcodeEmitter {
    static void write_imm(uint8_t* dst, int64_t imm);
    template<class... Args>
    static void write_regs(uint8_t* dst, Args... args);
    public:
    static void emit_mov_ri(std::vector<lmx::runtime::Op>& ops, uint8_t r1, int64_t imm);
    static void emit_mov_rr(std::vector<lmx::runtime::Op>& ops, uint8_t r1, uint8_t r2);
    static void emit_mov_rm(std::vector<lmx::runtime::Op>& ops, uint8_t r1, uint8_t r2, int8_t offest);
    static void emit_mov_rc(std::vector<lmx::runtime::Op>& ops, uint8_t r1, uint64_t idx);

    static void emit_mov_mi(std::vector<lmx::runtime::Op>& ops, uint8_t r1, int8_t offest1, int64_t imm);
    static void emit_mov_mr(std::vector<lmx::runtime::Op>& ops, uint8_t r1, int8_t offest1, uint8_t r2);
    static void emit_mov_mm(std::vector<lmx::runtime::Op>& ops, uint8_t r1, int8_t offest1, uint8_t r2, int8_t offest2);
    static void emit_mov_mc(std::vector<lmx::runtime::Op>& ops, uint8_t r1, int8_t offest1, uint64_t idx);

    static void emit_add(std::vector<lmx::runtime::Op>& ops, uint8_t r1, uint8_t r2, uint8_t r3);
    static void emit_sub(std::vector<lmx::runtime::Op>& ops, uint8_t r1, uint8_t r2, uint8_t r3);
    static void emit_mul(std::vector<lmx::runtime::Op>& ops, uint8_t r1, uint8_t r2, uint8_t r3);
    static void emit_div(std::vector<lmx::runtime::Op>& ops, uint8_t r1, uint8_t r2, uint8_t r3);
    static void emit_mod(std::vector<lmx::runtime::Op>& ops, uint8_t r1, uint8_t r2, uint8_t r3);
    static void emit_pow(std::vector<lmx::runtime::Op>& ops, uint8_t r1, uint8_t r2, uint8_t r3);

    static void emit_halt (std::vector<lmx::runtime::Op>& ops);
    static void emit_fcall(std::vector<lmx::runtime::Op>& ops, uint64_t idx);
    static void emit_fret (std::vector<lmx::runtime::Op>& ops);

    static void emit_debug_log(std::vector<lmx::runtime::Op> &ops, uint64_t idx);

    static void emit_jmp(std::vector<lmx::runtime::Op> &ops, uint64_t idx);

    static void emit_cmp_gt(std::vector<lmx::runtime::Op> &ops, uint8_t r1, uint8_t r2, uint8_t r3);
    static void emit_cmp_ge(std::vector<lmx::runtime::Op> &ops, uint8_t r1, uint8_t r2, uint8_t r3);
    static void emit_cmp_lt(std::vector<lmx::runtime::Op> &ops, uint8_t r1, uint8_t r2, uint8_t r3);
    static void emit_cmp_le(std::vector<lmx::runtime::Op> &ops, uint8_t r1, uint8_t r2, uint8_t r3);
    static void emit_cmp_eq(std::vector<lmx::runtime::Op> &ops, uint8_t r1, uint8_t r2, uint8_t r3);
    static void emit_cmp_ne(std::vector<lmx::runtime::Op> &ops, uint8_t r1, uint8_t r2, uint8_t r3);

    static void emit_if_true(std::vector<lmx::runtime::Op> &ops, uint8_t r, uint64_t idx);

    static void emit_if_false(std::vector<lmx::runtime::Op> &ops, uint8_t r, uint64_t idx);
};

} // namespace lmx

#endif //LMX_EMIT_HPP