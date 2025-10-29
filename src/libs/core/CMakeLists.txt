set(CORE_HEADERS
    Application.hpp
    Window.hpp
)

set(CORE_SRC
    Application.cpp
    Window.cpp
)

logl_add_library(logl_core
    SOURCES
        ${CORE_SRC}
    HEADERS
        ${CORE_HEADERS}
    PUBLIC_INCLUDES
        ${PROJECT_SOURCE_DIR}/src
    PUBLIC_LINKS
        glad
        glfw
    PRIVATE_LINKS
        imgui
)