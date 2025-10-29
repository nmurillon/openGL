set(RENDERER_HEADERS
    Camera.hpp
    Shader.hpp
)

set(RENDERER_SRC
    Camera.cpp
    Shader.cpp
)

logl_add_library(logl_renderer
    SOURCES
        ${RENDERER_SRC}
    HEADERS
        ${RENDERER_HEADERS}
    PUBLIC_INCLUDES
        ${PROJECT_SOURCE_DIR}/src
    PUBLIC_LINKS
        glad
        glm
    PRIVATE_LINKS
        logl_io
    RESOURCES
        shaders/basicShader.frag
        shaders/basicShader.vert
        shaders/light.frag
        shaders/lightSource.frag
)