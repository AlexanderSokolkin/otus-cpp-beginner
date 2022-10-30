cmake_minimum_required(VERSION 3.12)

project(guess_the_number)

set(HEADERS 
		config.h
		constants.h
		structs.h
		utils.h)

set(SOURCES
		config.cpp	
		main.cpp
		utils.cpp)

add_executable(guess_the_number ${HEADERS} ${SOURCES})

set_target_properties(guess_the_number 
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON)