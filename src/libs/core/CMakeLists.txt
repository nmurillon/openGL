set(CORE_HEADERS Shader.hpp Camera.hpp)

set(CORE_SRC Shader.cpp Camera.cpp)

logl_add_library(logl_core
    SOURCES
        ${CORE_SRC}
    HEADERS
        ${CORE_HEADERS}
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
)