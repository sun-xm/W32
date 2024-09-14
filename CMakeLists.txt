cmake_minimum_required(VERSION 3.10)
project(W32)

set(CLANG FALSE)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CLANG TRUE)
endif()


if(MSVC AND MSVC_VERSION GREATER_EQUAL 1910)
    add_compile_options(/permissive-)
endif()

if(CLANG)
    add_compile_options(
        -Wno-missing-braces
        -Wno-reorder-ctor
    )
endif()

set(CMAKE_CXX_STANDARD 11)

set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/Source")
file(GLOB SOURCE ${SRC_DIR}/*.cpp)
add_library(W32LIB STATIC ${SOURCE})
target_include_directories(
    W32LIB
    PUBLIC
    ${SRC_DIR}
)

add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)

if(CLANG)
    include_directories(
        "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/shared"
        "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/um"
    )
endif()

add_subdirectory(Samples/Borderless Borderless)
add_subdirectory(Samples/Controls   Controls)
add_subdirectory(Samples/Layered    Layered)
add_subdirectory(Samples/Menu       Menu)