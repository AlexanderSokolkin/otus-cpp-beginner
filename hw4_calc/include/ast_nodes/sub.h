#pragma once

#include "ast_node.h"


class Sub : public ASTNode
{
public:
	Sub(ASTNode* t_lhs, ASTNode* t_rhs);
};
