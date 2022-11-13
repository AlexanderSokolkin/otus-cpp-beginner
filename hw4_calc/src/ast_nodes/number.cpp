#include "ast_nodes/number.h"

Number::Number(int t_val) :
    ASTNode(std::to_string(t_val)), m_val(t_val)
{

}

int Number::value() const
{
    return m_val;
}
