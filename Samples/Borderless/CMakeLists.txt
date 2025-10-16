project(Borderless)

file(GLOB APP_SRC ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
add_executable(${PROJECT_NAME} WIN32 ${APP_SRC})