#pragma once

#include "ast_node.h"


class Div : public ASTNode
{
public:
	Div(ASTNode* t_lhs, ASTNode* t_rhs);
};
