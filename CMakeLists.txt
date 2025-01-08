cmake_minimum_required(VERSION 3.10)

project(SafeSquid_Reporting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories(
    include
    thirdparty/pugixml-1.14/src
)

set(SOURCES
    src/main.cc
    src/log_parser.cc
    src/base_log_parser.cc
    src/database.cc
    thirdparty/pugixml-1.14/src/pugixml.cpp
)

set(CSP_SOURCES
    src/csp_converter.cpp
)

add_executable(log_parser ${SOURCES})
add_executable(csp_converter ${CSP_SOURCES})

target_link_libraries(log_parser pqxx pq z)
