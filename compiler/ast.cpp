//
// Created by geguj on 2025/12/28.
//

#include "ast.hpp"
#include <cmath>
#include <iostream>
#include <ostream>

#include "generator/generator.hpp"
#include "generator/emit.hpp"

namespace lmx {


void node_error(const char* msg) {
    std::cerr << msg << std::endl;
    node_has_error = true;
}
int64_t ProgramASTNode::eval() const {
    int64_t result = 0;
    for (const auto& child : children) {
        result = child->eval();
    }
    return result;
}

int64_t NumberNode::eval() const {
    return std::stoll(num);
}

int64_t BinaryNode::eval() const {
    switch (op[0]) {
        case '+': 
            return left->eval() + right->eval();
        case '-': 
            return left->eval() - right->eval();
        case '*': 
            return left->eval() * right->eval();
        case '/': 
            return left->eval() / right->eval();
        case '^': 
            return std::pow(left->eval(), right->eval());
        case '%': 
            return left->eval() % right->eval();

        case '<': {
            if (op[1] == '=') 
                return left->eval() <= right->eval();
            return left->eval() < right->eval();
        }
        case '>': {
            if (op[1] == '=') 
                return left->eval() >= right->eval();
            return left->eval() > right->eval();
        }
        case '=': {
            if (op[1] == '=') 
                return left->eval() == right->eval();
        }
        case '!': {
            if (op[1] == '=') 
                return left->eval() != right->eval();
        }
        default: 
            return 0;
    }
}

int64_t UnaryNode::eval() const {
    switch (op[0]) {
        case '+': 
            return +operand->eval();
        case '-': 
            return -operand->eval();
        case '!': 
            return !operand->eval();
        default: 
            return 0;
    }
}

int64_t RPNExprNode::eval() const {
    std::vector<int64_t> stack;
    for (const auto& token : tokens) {
        if (token.type == RPNTokenType::Number) {
            stack.push_back(std::stoll(token.text));
        } else if (token.type == RPNTokenType::Operator) {
            int64_t b = stack.back();
            stack.pop_back();
            int64_t a = stack.back();
            stack.pop_back();
            switch (token.text[0]) {
                case '+': 
                    stack.push_back(a + b); 
                    break;
                case '-': 
                    stack.push_back(a - b); 
                    break;
                case '*': 
                    stack.push_back(a * b); 
                    break;
                case '/': 
                    stack.push_back(a / b); 
                    break;
                case '^': 
                    stack.push_back(std::pow(a, b)); 
                    break;
                case '%': 
                    stack.push_back(a % b); 
                    break;
                default: 
                    break;
            }
        }
    }
    return stack.back();
}

int64_t VarRefNode::eval() const {
    return 0;
}

int64_t VarDeclNode::eval() const {
    return 0;
}

int64_t IfStmtNode::eval() const {
    if (condition->eval()) {
        return thenBlock->eval();
    } else if (elseBlock) {
        return elseBlock->eval();
    }
    return 0;
}

// -----------------------------------------------------------------------------//

// gen methods

// -----------------------------------------------------------------------------//

size_t RPNExprNode::gen(Generator& gener) const {
    size_t result = 0;
    return 0;
}

size_t NumberNode::gen(Generator& gener) const {
    const size_t result = gener.regs.alloc();
    LMXOpcodeEmitter::emit_mov_ri(gener.ops, result, std::stoll(num));
    return result;
}

size_t UnaryNode::gen(Generator& gener) const {
    const auto operand_reg = operand->gen(gener);
    
    switch (op[0]) {
        case '+': 
            return operand_reg;
        case '-': {
            const size_t result = gener.regs.alloc();
            LMXOpcodeEmitter::emit_mov_ri(gener.ops, result, 0);
            LMXOpcodeEmitter::emit_sub(gener.ops, result, result, operand_reg);
            gener.regs.free(operand_reg);
            return result;
        }
        case '!': {
            const size_t result = gener.regs.alloc();
            LMXOpcodeEmitter::emit_mov_ri(gener.ops, result, 0);
            LMXOpcodeEmitter::emit_mov_rr(gener.ops, result, operand_reg);
            gener.regs.free(operand_reg);
            return result;
        }
    default:
            node_error((std::string("unknown operator") + op).c_str());
            return 0;
    }
}

size_t BinaryNode::gen(Generator& gener) const {
    const auto result = gener.regs.alloc();
    const auto lr = left->gen(gener);
    const auto rr = right->gen(gener);

    switch (op[0]) {
        case '+': 
            LMXOpcodeEmitter::emit_add(gener.ops, result, lr, rr);
            break;
        case '-': 
            LMXOpcodeEmitter::emit_sub(gener.ops, result, lr, rr);
            break;
        case '*': 
            LMXOpcodeEmitter::emit_mul(gener.ops, result, lr, rr);
            break;
        case '/': 
            LMXOpcodeEmitter::emit_div(gener.ops, result, lr, rr);
            break;
        case '^': 
            LMXOpcodeEmitter::emit_pow(gener.ops, result, lr, rr);
            break;
        case '%': 
            LMXOpcodeEmitter::emit_mod(gener.ops, result, lr, rr);
            break;
        case '>': {
            if (op[1] == '=') LMXOpcodeEmitter::emit_cmp_ge(gener.ops, result, lr, rr);
            else LMXOpcodeEmitter::emit_cmp_gt(gener.ops, result, lr, rr);
            break;
        }
        case '<': {
            if (op[1] == '=') LMXOpcodeEmitter::emit_cmp_le(gener.ops, result, lr, rr);
            else LMXOpcodeEmitter::emit_cmp_lt(gener.ops, result, lr, rr);
            break;
        }
        case '=': {
            if (op[1] == '=') LMXOpcodeEmitter::emit_cmp_eq(gener.ops, result, lr, rr);
            else node_error((std::string("unknown operator") + op).c_str());
            break;
        }
        case '!': {
            if (op[1] == '=') LMXOpcodeEmitter::emit_cmp_ne(gener.ops, result, lr, rr);
            else node_error((std::string("unknown operator") + op).c_str());
            break;
        }
        default: {
            node_error((std::string("unknown operator") + op).c_str());
            break;
        }
    }
    if (left->kind != ASTKind::VarDecl && left->kind != ASTKind::VarRef)
        gener.regs.free(lr);
    if (right->kind != ASTKind::VarDecl && right->kind != ASTKind::VarRef)
        gener.regs.free(rr);
    return result;
}

size_t VarDeclNode::gen(Generator& gener) const {
    if (const auto it = gener.vars.find(gener.make_scope(name)); it != gener.vars.end() && !it->second.first) {
        node_error(std::string("Generate Error: the var `" + name + "` not mutable").c_str());
        return -1;
    }
    auto result = value->gen(gener);
    gener.vars[gener.make_scope(name)] = std::pair(is_mut, result);
    return result;
}

size_t VarRefNode::gen(Generator& gener) const {
    const auto it = gener.vars.find(gener.make_scope(name));
    if (it == gener.vars.end()) {
        node_error(std::string("Generate Error: undefined var `" + name + "`").c_str());
        return -1;
    }
    return it->second.second;
}

size_t FuncCallExprNode::gen(Generator& gener) const {
    const auto it1 = gener.funcs.find(gener.make_scope(name));
    const auto it2 = gener.funcs.find(gener.last_scope + '@' + name);

    auto it = it1;
    if (it1 == gener.funcs.end()) {
        if (it2 == gener.funcs.end()) {
            std::cerr << "Generate Error: undefined function `" << name << "`" << std::endl;
            return -1;
        }
        it = it2;
    }
    size_t i = REG_COUNT - 1;
    for (const auto& arg : args) {
        const auto re = arg->gen(gener);
        LMXOpcodeEmitter::emit_mov_rr(gener.ops, i--, re);
        gener.regs.free(re);
    }
    
    LMXOpcodeEmitter::emit_fcall(gener.ops, it->second.second);
    return 0;  // 返回值存储在寄存器0中
}

size_t ReturnStmtNode::gen(Generator& gener) const {
    const auto re = expr->gen(gener);
    LMXOpcodeEmitter::emit_mov_rr(gener.ops, 0, re);
    gener.regs.free(re);
    LMXOpcodeEmitter::emit_fret(gener.ops);
    return 0;
}

size_t BlockStmtNode::gen(Generator& gener) const {
    for (const auto& child : children) {
        child->gen(gener);
    }
    return gener.ops.size();
}

size_t FuncDeclNode::gen(Generator& gener) const {
    LMXOpcodeEmitter::emit_jmp(gener.ops, 0); // 暂时填零
    auto jump_point = gener.ops.size() - 1;
    /* 记录跳转点， 后面需要在这里填充跳转位置
        跳转位置 = 函数体生成位置 + 1
        函数加载逻辑即jmp跳过函数体
    */
    
    const auto it = gener.funcs.find(gener.make_scope(name));
    if (it != gener.funcs.end()) {
        std::cerr << "Generate Error: redefined function `" << name << "`" << std::endl;
        return -1;
    } // 检查是否重定义 
    
    const auto copy_scope = gener.last_scope;
    gener.new_scope(name); // 新建函数作用域

    // 记录函数地址和参数数量
    gener.funcs[gener.cur_scope] = std::make_pair(args.size(), jump_point + 1);
    
    // 记录参数寄存器
    size_t i = REG_COUNT - 1;
    for (const auto& ps : args) {
        gener.regs.alloc(i);
        gener.vars[gener.make_scope(ps)] = std::make_pair(true, i--);
    }

    // 生成函数体
    const auto jump_pos = body->gen(gener) + 1; // block->gen()返回size
    if (node_has_error)return -1;

    LMXOpcodeEmitter::emit_fret(gener.ops);

    // 填充跳转位置
    memcpy(gener.ops[jump_point].operands, &jump_pos, sizeof(size_t));

    // 恢复参数寄存器
    i = REG_COUNT - 1;
    for (const auto& ps : args) {
        gener.vars.erase(gener.make_scope(ps));
        gener.regs.free(i--);
    }
    
    // 恢复作用域
    gener.free_scope(copy_scope);
    return -1;  
}

size_t IfStmtNode::gen(Generator& gener) const {
    const auto cond_reg = condition->gen(gener);
    LMXOpcodeEmitter::emit_if_true(gener.ops, cond_reg, 0); // 后续填充
    gener.regs.free(cond_reg);
    auto point1 = gener.ops.size() - 1;
    LMXOpcodeEmitter::emit_jmp(gener.ops, 0);   //后续填充
    auto point2 = gener.ops.size() - 1;

    auto addr1 = gener.ops.size();
    memcpy(gener.ops[point1].operands + 1, &addr1, sizeof(size_t));

    auto addr2 = thenBlock->gen(gener) ;
    memcpy(gener.ops[point2].operands, &addr2, sizeof(size_t));

    return -1;
}

size_t ProgramASTNode::gen(Generator &gener) const {
    for (const auto& child : children) {
        child->gen(gener);
    }
    return 0;
}
} // namespace lmx
