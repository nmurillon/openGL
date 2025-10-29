set(CORE_HEADERS
    Application.hpp
    Layer.hpp
    LayerStack.hpp
    Window.hpp
)

set(CORE_SRC
    Application.cpp
    Layer.cpp
    LayerStack.cpp
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
        logl_events
    PRIVATE_LINKS
        imgui
)