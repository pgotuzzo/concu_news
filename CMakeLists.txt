cmake_minimum_required(VERSION 3.8)
project(concu_news)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp)

add_executable(concu_news ${SOURCE_FILES})

configure_file(help ${CMAKE_CURRENT_BINARY_DIR}/help COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tmp)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/logs)