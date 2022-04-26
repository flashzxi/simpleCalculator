cmake_minimum_required(VERSION 3.22)
project(calculator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CURRENT_SOURCE_DIR source)
add_executable(calculator ${CMAKE_CURRENT_SOURCE_DIR}/cli.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AST.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/parse.cpp)
