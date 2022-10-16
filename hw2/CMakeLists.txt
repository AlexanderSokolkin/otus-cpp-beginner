cmake_minimum_required(VERSION 3.12)

project(homeWork2)

add_executable(homeWork2 
    move_me_out.h
    example.h


    hello_world.cpp
    move_me_out.cpp
    example.cpp
)

set_target_properties(homeWork2 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)