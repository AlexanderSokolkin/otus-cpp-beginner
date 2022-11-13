#pragma once

#include <string>

#include "ast_node.h"


class Number : public ASTNode {
public:
    Number(int val);

    int value() const;

private:
    int m_val;
};
