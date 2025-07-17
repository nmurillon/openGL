set(STB_IMAGE_SOURCES src/stb_image.cpp)

set(STB_IMAGE_HEADERS include/stb_image/stb_image.h)

add_library(stb_image SHARED ${STB_IMAGE_SOURCES} ${STB_IMAGE_HEADERS})

target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)