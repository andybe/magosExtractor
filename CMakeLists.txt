cmake_minimum_required(VERSION 3.0.0)
project(map-extracto VERSION 0.1.0)

add_executable(map-extractor MAPExtractor.cpp ExtractorCommon.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
