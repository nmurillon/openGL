logl_add_executable(colors
    SOURCES
        colors.cpp
        ../mesh.hpp
    LINKS
        logl_core
        logl_renderer
)