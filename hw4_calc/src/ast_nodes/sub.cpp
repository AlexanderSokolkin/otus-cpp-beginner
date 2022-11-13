#include "ast_nodes/sub.h"


Sub::Sub(ASTNode* t_lhs, ASTNode* t_rhs) :
	ASTNode("-", t_lhs, t_rhs)
{

}