#include "lexer.h"
#include "calc_exception.h"

#include <cctype>



 Lexer::Lexer(std::istream& t_in) :
     m_state(State::Empty), m_isError(false), m_number(0), m_in(t_in)
{
    next_char();
}



Lexer::Token Lexer::next_token() {
    for (;;) {
        switch (m_state) {
            case State::End:
                return Token::End;

            case State::ReadNumber:
                m_errorNumber += m_ch;
                if (end()) {
                    if (m_isError) {
                        m_isError = false;
                        throw CalcException("Invalid variable - " + m_errorNumber);
                    }
                    m_state = State::End;
                    return Token::Number;
                }
                if (std::isdigit(m_ch)) {
                    m_number = 10 * m_number + (m_ch - '0');
                    m_state = State::ReadNumber;
                    next_char();
                    break;
                }
                if (std::isalpha(m_ch)) {
                    m_isError = true;
                    m_state = State::ReadNumber;
                    next_char();
                    break;
                }
                if (m_isError) {
                    m_isError = false;
                    throw CalcException("Invalid variable - " + m_errorNumber);
                }
                m_state = State::Empty;
                return Token::Number;

            case State::ReadName:
                if (end()) {
                    m_state = State::End;
                    return Token::Name;
                }
                if (std::isalpha(m_ch) || std::isdigit(m_ch)) {
                    m_name += m_ch;
                    next_char();
                    break;
                }
                m_state = State::Empty;
                return Token::Name;

            case State::Empty:
                if (end()) {
                    m_state = State::End;
                    return Token::End;
                }
                else if (std::isspace(m_ch)) {
                    next_char();
                    break;
                }
                else if (isoperator(m_ch)) {
                    m_operator = m_ch;
                    next_char();
                    return Token::Operator;
                }
                else if (m_ch == '(') {
                    next_char();
                    return Token::Lbrace;
                }
                else if (m_ch == ')') {
                    next_char();
                    return Token::Rbrace;
                }
                else if (std::isdigit(m_ch)) {
                    m_errorNumber = m_ch;
                    m_number = m_ch - '0';
                    m_state = State::ReadNumber;
                    next_char();
                    break;
                }
                else if (std::isalpha(m_ch)) {
                    m_name = m_ch;
                    m_state = State::ReadName;
                    next_char();
                    break;
                }
                else {
                    std::string errStr = "Unknown character - ";
                    errStr.append(std::initializer_list<char>{m_ch});
                    throw CalcException(errStr);
                }
        }
    }
}

int Lexer::get_number() const
{
    return m_number;
}

std::string Lexer::get_operator() const
{
    return m_operator;
}

std::string Lexer::get_name() const
{
    return m_name;
}



bool Lexer::isbrace(char t_ch) const
{
    return t_ch == '(' || t_ch == ')';
}

bool Lexer::isoperator(char t_ch) const
{
    return t_ch == '+' || t_ch == '-' || t_ch == '*' || t_ch == '/';
}



char Lexer::next_char()
{
    m_in.get(m_ch);
    return m_ch;
}

bool Lexer::end() const
{
    return m_in.eof() || m_ch == '\n';
}
