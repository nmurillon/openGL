include(FetchContent)

############
## GLFW
############

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

############
## GLM
############

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 1.0.1
    OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(glm)
# Set compile options explicitly to build the project later with more restricitve build options
target_compile_options(glm PUBLIC -w)

############
## IMGUI
############
add_subdirectory(imgui)

############
## GLAD
############
add_subdirectory(glad)

############
## STB_IMAGE
############
add_subdirectory(stb_image)

############
## ASSIMP
############

set(ASSIMP_BUILD_ZLIB ON  CACHE BOOL "Build assimp's bundled zlib" FORCE)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.2
)

FetchContent_MakeAvailable(assimp)