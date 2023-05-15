cmake_minimum_required(VERSION 3.10)
project(MainWindow)

file(GLOB APP_SRC ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_executable(${PROJECT_NAME} WIN32
               ${APP_SRC}
               ${SOURCE})