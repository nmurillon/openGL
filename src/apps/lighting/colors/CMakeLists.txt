logl_add_executable(colors
    SOURCES
        colors.cpp
        ../mesh.hpp
        # Application layer
        application/ColorAppLayer.cpp
        application/ColorAppLayer.hpp
    LINKS
        logl_core
        logl_events
        logl_renderer
        glm
)