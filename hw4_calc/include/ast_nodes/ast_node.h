#pragma once

#include <cctype>
#include <iostream>
#include <string>


class ASTNode
{
public:
    explicit ASTNode(const std::string& t_repr);
    ASTNode(const std::string& t_repr, ASTNode* t_lhs, ASTNode* t_rhs);
    ~ASTNode();

    ASTNode(const ASTNode& t_other) = delete;
    ASTNode& operator=(const ASTNode& t_other) = delete;

    std::string repr() const;
    void print(std::ostream& t_out) const;

private:
    void inner_print(std::ostream& t_out, size_t t_indent) const;

    std::string     m_repr;
    ASTNode*        m_lhs;
    ASTNode*        m_rhs;
};
