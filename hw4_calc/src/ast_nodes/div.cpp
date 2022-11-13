#include "ast_nodes/div.h"

Div::Div(ASTNode* t_lhs, ASTNode* t_rhs) :
	ASTNode("/", t_lhs, t_rhs)
{

}