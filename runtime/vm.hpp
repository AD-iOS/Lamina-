//
// Created by geguj on 2025/12/27.
//
#pragma once
#include <array>
#include <cstdint>
#include <utility>
#include <vector>
#include "value/value.hpp"
#include "../include/opcode.hpp"

namespace lmx::runtime {

struct  LMXState {
    size_t pc{0};
    std::array<Value, 255> regs{};

    std::vector<size_t> ret_addr_stack;
    //void* const_pool_top;
    std::vector<Op>* program;
};
class VirtualCore {
    void* const_pool_top;
    LMXState ste;

    [[nodiscard]] Value *get_value_from_pool(const size_t offest) const;
public:
    VirtualCore();
    VirtualCore(const VirtualCore&) = delete;
    VirtualCore& operator=(const VirtualCore&) = delete;
    VirtualCore(VirtualCore&&) = delete;
    explicit VirtualCore(LMXState ste);
    explicit VirtualCore(LMXState ste, void* const_pool_top);
    int run();

    [[nodiscard]] std::vector<Op> *get_program() const { return ste.program; }
    void set_program(std::vector<Op> *program) { ste.pc = 0;ste.program = program; }
    int64_t look_register(const size_t r) const { return ste.regs[r].i64; }
};

}
