#pragma once

#include "ast_node.h"


class Add : public ASTNode
{
public:
	Add(ASTNode* t_lhs, ASTNode* t_rhs);
};
