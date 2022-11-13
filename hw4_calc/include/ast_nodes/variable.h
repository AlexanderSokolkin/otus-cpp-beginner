#pragma once

#include "ast_node.h"


class Variable : public ASTNode
{
public:
	Variable(const std::string& t_name);

private:
	std::string m_name;
};

