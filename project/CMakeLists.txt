cmake_minimum_required(VERSION 3.12)
project(sapper LANGUAGES CXX)

set(HEADERS
        constants.h
        field.h)

set(SOURCES
        field.cpp
        main.cpp)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
add_executable(sapper ${HEADERS} ${SOURCES})
target_link_libraries(sapper sfml-system sfml-window sfml-graphics sfml-audio)


target_include_directories(sapper PUBLIC "D:/otus-cpp-beginner/SFML_build/include")
target_link_directories(sapper PRIVATE "D:/otus-cpp-beginner/SFML_build/include")


set_target_properties(sapper
                        PROPERTIES
                            CXX_STANDARD 17
                            CXX_STANDARD_REQUIRED ON)

# if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
# CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR
# CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
# 	target_compile_options(sapper PRIVATE
# 	-g
# 	-Wall
# 	-Wextra
# 	-Werror
# 	-pedantic
# )
# elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
# 	target_compile_options(sapper PRIVATE
# 	/W4
# 	/WX
# 	)
# endif()
