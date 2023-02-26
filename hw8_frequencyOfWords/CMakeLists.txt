cmake_minimum_required(VERSION 3.14)
project(frequencyOfWords LANGUAGES CXX)


set(HEADERS
		threadPool.h
        utils.h
        constants.h)

set(SOURCES
		threadPool.cpp
		utils.cpp
        main.cpp)


add_executable(frequencyOfWords ${HEADERS} ${SOURCES})

set_target_properties(frequencyOfWords
                        PROPERTIES
                            CXX_STANDARD 17
                            CXX_STANDARD_REQUIRED ON)


if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR
CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(frequencyOfWords PRIVATE
	-g
	-Wall
	-Wextra
	-Werror
	-pedantic
)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(frequencyOfWords PRIVATE
	/W4
	/WX
	)
endif()
						 