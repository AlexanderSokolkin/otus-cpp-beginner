#pragma once

#include <istream>
#include <string>

class Lexer {
public:
	enum class Token {
		Number,
		Operator,
		End,
		Lbrace,
		Rbrace,
		Name,

		Unknown
	};

	explicit Lexer(std::istream& t_in);

	Lexer(const Lexer& t_other) = delete;
	Lexer& operator=(const Lexer& t_other) = delete;


	Token next_token();

	int get_number() const;
	std::string get_operator() const;
	std::string get_name() const;

protected:
	bool isbrace(char t_ch) const;
	bool isoperator(char t_ch) const;

private:
	enum class State {
		Empty,
		ReadNumber,
		ReadName,
		End,
	};
	char next_char();
	bool end() const;


	State			m_state;
	std::string		m_name;
	std::string		m_errorNumber; // В данное поле аккумулируем числа, чтобы вернуть, в случае ошибки (например: 123ab)
	bool			m_isError;
	int				m_number;
	std::string		m_operator;
	char			m_ch;
	std::istream&	m_in;
};
