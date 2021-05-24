cmake_minimum_required(VERSION 3.0.0)
project(mangosExtractor VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(map-extractor MAPExtractor.cpp ExtractorCommon.cpp ExtractorCommon.hh)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
