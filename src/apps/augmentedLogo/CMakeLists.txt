find_package(OpenCV REQUIRED COMPONENTS opencv_core opencv_imgproc opencv_imgcodecs opencv_aruco)

logl_add_executable(augmented_logo
    SOURCES
        main.cpp
        application/AugmentedLogoLayer.cpp
        application/AugmentedLogoLayer.hpp
    LINKS
        glad
        glfw
        stb_image
        logl_core
        logl_events
        logl_renderer
        logl_io
        opencv_aruco
        opencv_video
    RESOURCES
        shaders/backgroundFrag.glsl
        shaders/backgroundVert.glsl
        # Texture images
        textures/totk.jpg
        
)