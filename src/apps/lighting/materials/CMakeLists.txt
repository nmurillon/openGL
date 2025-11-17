logl_add_executable(materials
    SOURCES
        materials.cpp
        ../mesh.hpp
        # Application layer
        application/MaterialsLayer.cpp
        application/MaterialsLayer.hpp
    LINKS
        logl_core
        logl_io
        logl_events
        logl_renderer
        glm
    RESOURCES
        shaders/light.frag
        shaders/materials.frag
        shaders/materials.vert
)