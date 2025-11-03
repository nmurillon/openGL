set(IMGUI_SOURCES 
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    # Backends
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
    # Misc
    imgui/imgui_stdlib.cpp)

set(IMGUI_HEADERS
    imgui/imconfig.h
    imgui/imgui_internal.h
    imgui/imgui.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    # Backends
    imgui/imgui_impl_glfw.h
    imgui/imgui_impl_opengl3_loader.h
    imgui/imgui_impl_opengl3.h
    # Misc
    imgui/imgui_stdlib.h)

add_library(imgui STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(imgui PRIVATE glfw)

if(NOT WIN32)
    find_package(X11)
    if(X11_FOUND)
        target_link_libraries(imgui PRIVATE X11::X11)
    endif()
endif()

target_compile_options(imgui PRIVATE -fPIC PUBLIC -w)