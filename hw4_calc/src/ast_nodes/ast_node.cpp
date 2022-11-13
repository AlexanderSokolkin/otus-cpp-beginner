#include "ast_nodes/ast_node.h"

ASTNode::ASTNode(const std::string& t_repr) :
    m_repr(t_repr), m_lhs{ nullptr }, m_rhs{ nullptr }
{

}

ASTNode::ASTNode(const std::string& t_repr, ASTNode* t_lhs, ASTNode* t_rhs) :
    m_repr(t_repr), m_lhs{ t_lhs }, m_rhs{ t_rhs }
{

}

ASTNode::~ASTNode()
{
    delete m_lhs;
    delete m_rhs;
}

std::string ASTNode::repr() const
{
    return m_repr;
}

void ASTNode::print(std::ostream& t_out) const
{
    inner_print(t_out, 0);
}

void ASTNode::inner_print(std::ostream& t_out, size_t t_indent) const
{
    if (m_lhs) {
        m_lhs->inner_print(t_out, t_indent + 1);
    }
    for (size_t i = 0; i < t_indent; ++i) {
        t_out << "    ";
    }
    t_out << m_repr << '\n';
    if (m_rhs) {
        m_rhs->inner_print(t_out, t_indent + 1);
    }
}
