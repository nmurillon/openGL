find_package(OpenCV REQUIRED COMPONENTS opencv_core opencv_imgproc opencv_imgcodecs)

logl_add_executable(epipolar_lines
    SOURCES
        main.cpp
    LINKS
        logl_io
        opencv_highgui
        opencv_video
        imgui
    RESOURCES
        images/image_0007.jpg
        images/image_0011.jpg
        images/image_0001.jpg
        images/image_0006.jpg      
)