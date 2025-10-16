project(Menu)

file(GLOB APP_SRC ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
list(APPEND APP_SRC ${CMAKE_CURRENT_LIST_DIR}/Menu.rc)
add_executable(${PROJECT_NAME} WIN32 ${APP_SRC})