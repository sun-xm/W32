cmake_minimum_required(VERSION 3.10)
project(Layered)

file(GLOB APP_SRC ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
add_executable(${PROJECT_NAME} WIN32 ${APP_SRC})

target_link_libraries(${PROJECT_NAME} W32LIB)