//
// Created by geguj on 2025/12/28.
//

#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <string>

#include "generator/generator.hpp"

namespace lmx {
static bool node_has_error = false;
enum ASTKind {
    Program,
    Binary, Unary, NumLiteral, StringLiteral, Ident, BoolLiteral,
    RPNExpr,
    ExprStmt,
    BlockStmt,
    IfStmt,
    VarDecl,
    VarRef,
    FuncDecl,
    FuncCallExpr,
    Return,
};

struct ASTNode {
    ASTKind kind;
    
    virtual ~ASTNode() = default;
    explicit ASTNode(ASTKind kind) : kind(kind) {}
    
    [[nodiscard]] virtual int64_t eval() const = 0;
    virtual size_t gen(Generator& gener) const = 0;
};

struct TypeNode {
    std::string name;
    
    explicit TypeNode(std::string name) : name(std::move(name)) {}
    ~TypeNode() = default;
    
    TypeNode() = default;
};

struct ProgramASTNode final : public ASTNode {
    std::vector<std::shared_ptr<ASTNode>> children;
    
    explicit ProgramASTNode(
        std::vector<std::shared_ptr<ASTNode>> children
    ) : ASTNode(Program),
        children(std::move(children)) {}
    
    [[nodiscard]] int64_t eval() const override;
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct StmtNode : public ASTNode {
    explicit StmtNode(ASTKind kind) : ASTNode(kind) {}
    
    ~StmtNode() override = default;
    virtual int64_t eval() const = 0;
    virtual size_t gen(Generator& gener) const = 0;
};

struct ExprNode : public ASTNode {
    explicit ExprNode(ASTKind kind) : ASTNode(kind) {}
    
    ~ExprNode() override = default;
    virtual int64_t eval() const override = 0;
    virtual size_t gen(Generator& gener) const override = 0;
};

struct ExprStmt final : public StmtNode {
    std::shared_ptr<ExprNode> hs;
    
    explicit ExprStmt(std::shared_ptr<ExprNode> hs) 
        : StmtNode(ASTKind::ExprStmt), 
          hs(std::move(hs)) {}
    
    [[nodiscard]] int64_t eval() const override;
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct BlockStmtNode final : public StmtNode {
    std::vector<std::shared_ptr<ASTNode>> children;
    
    explicit BlockStmtNode(std::vector<std::shared_ptr<ASTNode>> children) 
        : StmtNode(ASTKind::BlockStmt), 
          children(std::move(children)) {}
    
    [[nodiscard]] int64_t eval() const override { return 0; }
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct IfStmtNode : public StmtNode {
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<BlockStmtNode> thenBlock;
    std::shared_ptr<BlockStmtNode> elseBlock;
    
    explicit IfStmtNode(
        std::shared_ptr<ExprNode> condition,
        std::shared_ptr<BlockStmtNode> thenBlock,
        std::shared_ptr<BlockStmtNode> elseBlock
    ) : StmtNode(ASTKind::IfStmt),
        condition(std::move(condition)),
        thenBlock(std::move(thenBlock)),
        elseBlock(std::move(elseBlock)) {}
    
    [[nodiscard]] int64_t eval() const override;
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct FuncDeclNode final : public ASTNode {
    std::string name;
    std::vector<std::string> args;
    std::shared_ptr<BlockStmtNode> body;
    
    explicit FuncDeclNode(
        std::string name,
        std::vector<std::string> args,
        std::shared_ptr<BlockStmtNode> body
    ) : ASTNode(ASTKind::FuncDecl),
        name(std::move(name)), 
        args(std::move(args)), 
        body(std::move(body)) {}
    
    [[nodiscard]] int64_t eval() const override { return 0; }
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct ReturnStmtNode final : public StmtNode {
    std::shared_ptr<ExprNode> expr;
    
    explicit ReturnStmtNode(std::shared_ptr<ExprNode> expr) 
        : StmtNode(ASTKind::Return), 
          expr(std::move(expr)) {}
    
    [[nodiscard]] int64_t eval() const override { return 0; }
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct FuncCallExprNode final : public ExprNode {
    std::string name;
    std::vector<std::shared_ptr<ExprNode>> args;
    
    FuncCallExprNode(
        std::string name,
        std::vector<std::shared_ptr<ExprNode>> args
    ) : ExprNode(ASTKind::FuncCallExpr), 
        name(std::move(name)), 
        args(std::move(args)) {}
    
    [[nodiscard]] int64_t eval() const override { return 0; }
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct VarDeclNode final : public ASTNode {
    std::string name;
    std::shared_ptr<ExprNode> value;
    bool is_mut;
    
    explicit VarDeclNode(
        std::string name,
        std::shared_ptr<ExprNode> value,
        bool is_mut = true
    ) : ASTNode(VarDecl), 
        name(std::move(name)), 
        value(std::move(value)), 
        is_mut(is_mut) {}
    
    [[nodiscard]] int64_t eval() const override;
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct VarRefNode final : public ExprNode {
    std::string name;
    
    explicit VarRefNode(std::string name) 
        : ExprNode(ASTKind::VarRef), 
          name(std::move(name)) {}
    
    [[nodiscard]] int64_t eval() const override;
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct NumberNode final : public ExprNode {
    std::string num;
    
    explicit NumberNode(std::string num) 
        : ExprNode(ASTKind::NumLiteral), 
          num(std::move(num)) {}
    
    ~NumberNode() override = default;
    [[nodiscard]] int64_t eval() const override;
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct BinaryNode final : public ExprNode {
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    std::string op;
    
    BinaryNode(
        std::shared_ptr<ASTNode> left,
        std::shared_ptr<ASTNode> right,
        std::string op
    ) : ExprNode(ASTKind::Binary), 
        left(std::move(left)), 
        right(std::move(right)), 
        op(std::move(op)) {}
    
    [[nodiscard]] int64_t eval() const override;
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct UnaryNode final : public ExprNode {
    std::string op;
    std::shared_ptr<ASTNode> operand;
    
    explicit UnaryNode(std::string op, std::shared_ptr<ASTNode> operand) 
        : ExprNode(ASTKind::Unary), 
          op(std::move(op)), 
          operand(std::move(operand)) {}
    
    [[nodiscard]] int64_t eval() const override;
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

struct RPNExprNode final : public ExprNode {
    enum RPNTokenType {
        Number,
        Operator,
    };
    
    struct RPNToken {
        RPNTokenType type;
        std::string text;
    };
    
    std::vector<RPNToken> tokens;
    
    explicit RPNExprNode(std::vector<RPNToken> tokens) 
        : ExprNode(ASTKind::RPNExpr), 
          tokens(std::move(tokens)) {}
    
    [[nodiscard]] int64_t eval() const override;
    [[nodiscard]] size_t gen(Generator& gener) const override;
};

} // namespace lmx
