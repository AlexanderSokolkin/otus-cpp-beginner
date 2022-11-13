#include "ast_nodes/mul.h"


Mul::Mul(ASTNode* t_lhs, ASTNode* t_rhs) :
	ASTNode("*", t_lhs, t_rhs)
{

}