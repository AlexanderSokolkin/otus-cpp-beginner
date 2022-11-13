#include "ast_nodes/add.h"


Add::Add(ASTNode* t_lhs, ASTNode* t_rhs) :
	ASTNode("+", t_lhs, t_rhs)
{
	
}