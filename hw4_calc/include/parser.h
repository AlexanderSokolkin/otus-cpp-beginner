#pragma once

#include <istream>

#include "lexer.h"

class ASTNode;


class Parser {
public:

    explicit Parser(Lexer& t_lexer);
    ~Parser() = default;

    Parser(const Parser& t_other) = delete;
    Parser& operator=(const Parser& t_other) = delete;


    ASTNode* parse();
    bool isError() const;
    std::string errorStr() const;

private:
    void next_token();

    ASTNode* expr(bool is_nestedExpr = false);
    ASTNode* term();
    ASTNode* prim();

    Lexer&          m_lexer;
    Lexer::Token    m_tok;

    bool            m_isError;
    std::string     m_errorStr;
};
