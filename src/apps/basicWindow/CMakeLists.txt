set(BASIC_WINDOW_SOURCES main.cpp)

logl_add_executable(basic_window
    SOURCES
        ${BASIC_WINDOW_SOURCES}
    LINKS
        glad
        glfw
        stb_image
        logl_core
)