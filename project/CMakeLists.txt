cmake_minimum_required(VERSION 3.12)
project(sapper LANGUAGES CXX)

set(HEADERS
        button.h
        constants.h
        game.h
        field.h
        indicator.h
        utils.h)

set(SOURCES
        button.cpp
        constants.cpp
        game.cpp
        field.cpp
        main.cpp
        indicator.cpp
        utils.cpp)

file(GLOB RES_FILE "resource.rc")


find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
add_executable(sapper ${HEADERS} ${SOURCES} ${RES_FILE})
target_link_libraries(sapper sfml-system sfml-window sfml-graphics)


include_directories(sapper PRIVATE ${SFML_INCLUDE_DIR})
target_link_directories(sapper PRIVATE ${SFML_LIBRARIES})


set_target_properties(sapper
                        PROPERTIES
                            CXX_STANDARD 17
                            CXX_STANDARD_REQUIRED ON
                            RESOURCE ${RES_FILE})
