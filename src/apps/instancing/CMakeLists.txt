logl_add_executable(instancing
    SOURCES
        main.cpp
        InstancingLayer.cpp
        InstancingLayer.hpp
        QuadViewport.cpp
        QuadViewport.hpp
    LINKS
        logl_core
        logl_io
        logl_renderer
        glm
    RESOURCES
        assets/shaders/quad.frag
        assets/shaders/quad.vert
        # assets/textures/planet/mars.png
        # assets/textures/planet/planet.mtl
        # assets/textures/planet/planet.obj
        # assets/textures/planet/source.txt
        # assets/textures/rock/rock.png
        # assets/textures/rock/rock.mtl
        # assets/textures/rock/rock.obj
)