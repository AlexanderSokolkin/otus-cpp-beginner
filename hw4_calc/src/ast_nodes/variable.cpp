#include "ast_nodes/variable.h"


Variable::Variable(const std::string& t_name) :
	ASTNode(t_name), m_name(t_name)
{

}
