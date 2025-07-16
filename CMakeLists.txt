cmake_minimum_required(VERSION 3.25)

project(openGLLearning VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

# build directory containing the generated files
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(_outputFolderName "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${_outputFolderName}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${_outputFolderName}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${_outputFolderName}/bin)

include(cmake/CMakeHelper.cmake)

add_subdirectory(external)
add_subdirectory(src)