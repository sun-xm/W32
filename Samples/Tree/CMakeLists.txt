project(Tree)

file(
    GLOB APP_SRC
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/*.rc")

add_executable(
    ${PROJECT_NAME}
    WIN32
    ${APP_SRC}
)