logl_add_library(logl_logger
    SOURCES
    HEADERS
        Logger.hpp
    PUBLIC_INCLUDES
        ${PROJECT_SOURCE_DIR}/src
    PUBLIC_LINKS

    PRIVATE_LINKS
    RESOURCES
)