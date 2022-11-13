#pragma once

#include "ast_node.h"


class Mul : public ASTNode
{
public:
	Mul(ASTNode* t_lhs, ASTNode* t_rhs);
};
