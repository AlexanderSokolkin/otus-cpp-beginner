cmake_minimum_required(VERSION 3.14)

project(hw7_googleTest)

find_package(GTest)


set(SOURCES
        sequence_container_test.cpp
        list_container_test.cpp
        main.cpp
)

add_executable(
    containers_tests
        ${SOURCES}
)

add_subdirectory(containers)
target_include_directories(
    containers_tests PRIVATE
    containers
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(
    containers_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)



set_target_properties(containers_tests
            PROPERTIES
                CXX_STANDARD 17
                CXX_STANDARD_REQUIRED ON)


if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
   CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR
   CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(containers_tests PRIVATE
    -g
    -Wall
    -Wextra
    -Werror
    -pedantic
)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(containers_tests PRIVATE
    /W4
    /WX
    )
endif()
