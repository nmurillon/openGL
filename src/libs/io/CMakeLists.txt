set(IO_SOURCES ProgramPath.cpp)
set(IO_HEADERS ProgramPath.hpp)

logl_add_library(logl_io
    SOURCES
        ${IO_SOURCES}
    HEADERS
        ${IO_HEADERS}
    PUBLIC_INCLUDES
        ${PROJECT_SOURCE_DIR}/src
)