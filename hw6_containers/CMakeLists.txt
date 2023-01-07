cmake_minimum_required(VERSION 3.13)

project(hw6_containers LANGUAGES CXX)


set(HEADERS
        sequence_container.h
        list_container.h)

set(SOURCES
        main.cpp)


add_executable(hw6_containers ${HEADERS} ${SOURCES})


set_target_properties(hw6_containers
                        PROPERTIES
                            CXX_STANDARD 17
                            CXX_STANDARD_REQUIRED ON)
