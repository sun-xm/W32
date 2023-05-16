cmake_minimum_required(VERSION 3.10)
project(Controls)

file(GLOB APP_SRC ${CMAKE_CURRENT_LIST_DIR}/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*.rc)
add_executable(${PROJECT_NAME} WIN32 ${APP_SRC})
target_link_libraries(${PROJECT_NAME} W32LIB)