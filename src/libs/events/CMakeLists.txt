set(EVENTS_SOURCES 
    Event.cpp
)
set(EVENTS_HEADERS 
    Event.hpp
    EventDispatcher.hpp
    WindowEvent.hpp
)

logl_add_library(logl_events
    SOURCES
        ${EVENTS_SOURCES}
    HEADERS
        ${EVENTS_HEADERS}
    PUBLIC_INCLUDES
        ${PROJECT_SOURCE_DIR}/src
)