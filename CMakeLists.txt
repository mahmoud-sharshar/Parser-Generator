cmake_minimum_required(VERSION 2.6)
project(parser)


include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(parser ${SOURCES})

set_property(TARGET parser PROPERTY CXX_STANDARD 17)

target_compile_options(parser PRIVATE -Wall -Wextra)
