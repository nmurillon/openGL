set(CORE_HEADERS Shader.hpp)

set(CORE_SRC Shader.cpp)

add_library(core ${CORE_HEADERS} ${CORE_SRC})
target_include_directories(core PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(core PUBLIC glad)