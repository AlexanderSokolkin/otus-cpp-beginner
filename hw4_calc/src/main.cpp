#include "ast_nodes/ast_node.h"
#include "lexer.h"
#include "parser.h"
#include "calc_exception.h"

#include <iostream>


int main()
{
    Lexer lexer(std::cin);
    Parser parser(lexer);

    ASTNode* ast = parser.parse();
    
    if (parser.isError()) {
        std::cout << parser.errorStr();
        return 1;
    }

    if (ast) {
        ast->print(std::cout);
    }

    return 0;
}
