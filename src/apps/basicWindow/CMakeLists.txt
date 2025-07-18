set(BASIC_WINDOW_SOURCES main.cpp)

logl_add_executable(basic_window
    SOURCES
        ${BASIC_WINDOW_SOURCES}
    LINKS
        glad
        glfw
        glm
        stb_image
        logl_core
        logl_io
    RESOURCES
        textures/kaamelott.jpg
        textures/totk.jpg
)