target_sources(calc
	PRIVATE
		calc_exception.cpp
		lexer.cpp
		parser.cpp
		main.cpp)

add_subdirectory(ast_nodes)
