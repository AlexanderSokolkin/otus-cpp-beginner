#include "parser.h"
#include "ast_nodes/number.h"
#include "ast_nodes/variable.h"
#include "ast_nodes/add.h"
#include "ast_nodes/sub.h"
#include "ast_nodes/mul.h"
#include "ast_nodes/div.h"
#include "calc_exception.h"


using Token = Lexer::Token;

Parser::Parser(Lexer& t_lexer) :
    m_lexer(t_lexer), m_tok(Token::Unknown), m_isError(false)
{

}



void Parser::next_token()
{
    m_tok = m_lexer.next_token();
}



ASTNode* Parser::parse()
{
    try {
        return expr();
    }
    catch (CalcException e) {
        m_isError = true;
        m_errorStr = e.error();
        return nullptr;
    }
}

bool Parser::isError() const
{
    return m_isError;
}

std::string Parser::errorStr() const
{
    return m_errorStr;
}


ASTNode* Parser::expr(bool is_nestedExpr)
{
    // parse addition and subsctruction
    ASTNode* root = term();
    for (;;) {
        switch (m_tok) {
            case Token::Operator: {
                std::string op = m_lexer.get_operator();
                switch (op.front()) {
                    case '+':
                         root = new Add(root, term());
                        break;
                    case '-':
                        root = new Sub(root, term());
                        break;
                    default:
                        return root;
                    }
                break;
            }
            case Token::Rbrace:
                if (!is_nestedExpr) {
                    delete root;
                    throw CalcException("In the given expression missing left brace '('");
                }
                return root;
            case Token::Number:
            case Token::Name:
            case Token::Lbrace:
                delete root;
                throw CalcException("Invalid expression");
            default:
                return root;
        }
    }
}

ASTNode* Parser::term()
{
    // parse multiplication and division
    ASTNode* root = prim();
    for (;;) {
        switch (m_tok) {
            case Token::Operator: {
                std::string op = m_lexer.get_operator();
                switch (op.front()) {
                    case '*':
                        root = new Mul(root, prim());
                        break;
                    case '/':
                        root = new Div(root, prim());
                        break;
                    default:
                        return root;
                }
                break;
            }
            default:
                return root;
        }
    }
}

ASTNode* Parser::prim()
{
    // parse numbers and names
    ASTNode* node = nullptr;
    next_token();
    switch (m_tok) {
        case Token::Number:
            node = new Number(m_lexer.get_number());
            break;
        case Token::Name:
            node = new Variable(m_lexer.get_name());
            break;
        case Token::Lbrace:
            node = expr(true);
            if (m_tok != Token::Rbrace) {
                delete node;
                throw CalcException("In the given expression missing right brace ')'");
            }
            break;
        default:
            if (!node) {
                throw CalcException("Invalid expression");
            }
            break;
    }

    next_token();
    return node;
}
