set(BASIC_WINDOW_SOURCES main.cpp)

add_executable(basic_window ${BASIC_WINDOW_SOURCES})

target_link_libraries(basic_window PRIVATE glad glfw)