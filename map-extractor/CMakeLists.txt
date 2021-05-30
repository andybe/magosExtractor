cmake_minimum_required(VERSION 3.0.0)
project(map-extractor VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories("../shared")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(map-extractor MAPExtractor.cpp dbcfile.cpp)
target_link_libraries(map-extractor extractor loadlib)
