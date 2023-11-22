cmake_minimum_required(VERSION 3.10)
project(Menu)

file(GLOB APP_SRC ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
list(APPEND APP_SRC ${CMAKE_CURRENT_LIST_DIR}/Menu.rc)

add_executable(${PROJECT_NAME} WIN32 ${APP_SRC})

target_link_libraries(${PROJECT_NAME} W32LIB)