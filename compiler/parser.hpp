//
// Created by geguj on 2025/12/28.
//

#pragma once
#include <iostream>
#include <memory>

#include "ast.hpp"
#include "lexer.hpp"

namespace lmx {
struct Token;

class Parser {
    bool has_err{false};
    std::vector<Token>& tokens;
    size_t pos{0};
    void advance();
    [[nodiscard]] Token& cur() const;
    [[nodiscard]] bool match(TokenType t) const;
    [[nodiscard]] bool is_eof() const;
    std::shared_ptr<ExprNode> expr();
    std::shared_ptr<ExprNode> term();



    std::shared_ptr<ExprNode> factor();

    std::shared_ptr<ExprNode> rpn_expr();

    std::shared_ptr<ExprNode> rpn_term();

    bool peek_match(TokenType type);

    void check_eof();

    void error(const std::string& msg);

    std::shared_ptr<BlockStmtNode> parse_block();

    std::shared_ptr<ASTNode> parse_if();
    std::shared_ptr<ExprNode> parse_expr();
    std::shared_ptr<ASTNode> parse_funcdecl();

public:
    explicit Parser(std::vector<Token>& tokens): tokens(tokens) {}

    std::shared_ptr<ASTNode> parse();


    std::shared_ptr<ProgramASTNode> parse_program();
    [[nodiscard]] bool error() const {return has_err;}
};

} // lmx