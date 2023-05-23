cmake_minimum_required(VERSION 3.10)
project(W32)

set(CMAKE_CXX_STANDARD 11)

set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/Source")
file(GLOB SOURCE ${SRC_DIR}/*.cpp)
add_library(W32LIB STATIC ${SOURCE})

include_directories(${SRC_DIR})
add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)

add_subdirectory(Samples/Borderless Borderless)
add_subdirectory(Samples/Controls   Controls)