set(BASIC_WINDOW_SOURCES main.cpp)

add_executable(basic_window ${BASIC_WINDOW_SOURCES})

target_link_libraries(basic_window PRIVATE glad glfw core)

# TODO: clean this to export resources with build
target_compile_definitions(basic_window PRIVATE -DSHADER_DIR="${PROJECT_SOURCE_DIR}/src/libs/core/shaders")