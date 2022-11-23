cmake_minimum_required(VERSION 3.15)

project(statistics)

set(HEADERS
        constants.h
        utils.h)

set(SOURCES
        main.cpp
        utils.cpp)

add_executable(statistics
                ${HEADERS}
                ${SOURCES})

add_subdirectory(stats)

set_target_properties(statistics
                        PROPERTIES
                            CXX_STANDARD 17
                            CXX_STANDARD_REQUIRED ON)
