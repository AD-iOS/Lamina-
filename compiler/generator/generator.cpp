//
// Created by geguj on 2025/12/28.
//

#include "generator.hpp"
#include <bitset>
#include <unordered_map>
#include <vector>

#include "../../include/opcode.hpp"

namespace lmx {

Allocator::Allocator() {
    bitset.reset();
    bitset.set(0);
}

size_t Allocator::alloc() {
    for (size_t i = 0; i < REG_COUNT; i++) {
        if (!bitset.test(i)) {
            bitset.set(i);
            return i;
        }
    }
    return -1;
}

size_t Allocator::alloc(size_t i) {
    bitset.set(i);
    return i;
}

void Allocator::free(size_t i) {
    if (i > 0 && bitset.test(i)) {
        bitset.reset(i);
    }
}

bool Allocator::is_free(size_t i) {
    return bitset.test(i);
}

void Generator::write(runtime::Op& op) {
    ops.push_back(op);
}

void Generator::new_scope(const std::string& new_scope) {
    last_scope = cur_scope;
    cur_scope = make_scope(new_scope);
}

void Generator::free_scope(const std::string& original_scope) {
    cur_scope = last_scope;
    last_scope = original_scope;
}

std::string Generator::make_scope(const std::string& name) const {
    return cur_scope + '@' + name;
}

std::vector<lmx::runtime::Op> Generator::get_ops() {
    if (ops.back().op != runtime::Opcode::HALT) {
        ops.emplace_back(lmx::runtime::Opcode::HALT);
    }
    return ops;
}

} // namespace lmx