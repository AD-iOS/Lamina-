//
// Created by geguj on 2025/12/28.
//

#pragma once
#include <bitset>
#include <unordered_map>
#include <vector>

#include "../../include/lmx_export.hpp"
#include "../../include/opcode.hpp"

namespace lmx {
namespace runtime {
struct Op;
}
#define REG_COUNT 255
class LMC_API Allocator {
    std::bitset<REG_COUNT> bitset;
public:
    Allocator();
    size_t alloc();
    size_t alloc(size_t i);
    void free(size_t i);
    bool is_free(size_t i);
};
class LMC_API Generator {

public:
    std::unordered_map<std::string, std::pair<bool, size_t>> vars;  // name <mutable, index register>
    std::unordered_map<std::string, std::pair<size_t, size_t>> funcs;  // name <arg_count， address>
    std::string last_scope;
    std::string cur_scope{"global"};
    Allocator regs;
    Generator() = default;
    ~Generator() = default;

    std::vector<runtime::Op> ops;
    void write(runtime::Op& op);

    void new_scope(const std::string& new_scope);
    void free_scope(const std::string& original_scope);
    std::string make_scope(const std::string& name) const;

    std::vector<lmx::runtime::Op> get_ops();
};

}
