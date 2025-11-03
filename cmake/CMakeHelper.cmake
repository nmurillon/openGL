#
# Function for managing the resources (create the correct folder, copy, intall)
#
# Usage:
#   logl_manage_resources(
#            target_name name
#            resources [item1...])
#
# target_name the name of the target
# resources the list of resources
#
function(logl_manage_resources target_name resources)
    list(LENGTH resources _len)
    
    if(${_len} GREATER 0)
        get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

        string(TOUPPER ${target_name} target_name_upper)
        set(RESOURCES_FOLDER ${target_name}_resources)
        set(resources_folder_definition "${target_name_upper}_RESOURCES_FOLDER")

        message("Resources folder for target ${target_name} : ${resources_folder_definition}")

        target_compile_definitions(${target_name} PRIVATE ${resources_folder_definition}="${RESOURCES_FOLDER}")

        set(COPY_TO "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}$<$<BOOL:${GENERATOR_IS_MULTI_CONFIG}>:/$<CONFIG>>/${RESOURCES_FOLDER}")
        foreach(RESOURCE ${_params_RESOURCES})
            set(DEST_FILE "${COPY_TO}/${RESOURCE}")

            add_custom_command(
                OUTPUT "${DEST_FILE}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE}" "${DEST_FILE}"
                COMMENT "Copying resource: ${RESOURCE} for ${target_name}"
                DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE}"
            )
            list(APPEND RESOURCE_OUTPUTS "${DEST_FILE}")
        endforeach()

        add_custom_target(${target_name}_resources
            DEPENDS ${RESOURCE_OUTPUTS}
        )
        add_dependencies(${target_name} ${target_name}_resources)

    endif()
endfunction()

#
# Create a new library
#
# Usage:
#   logl_add_library(
#                SOURCES [item1...]
#                HEADERS [item1...]
#                [PUBLIC_INCLUDES [item1...] ]
#                [PRIVATE_INCLUDES [item1...] ]
#                [PUBLIC_LINKS [item1...] ]
#                [PRIVATE_LINKS [item1...] ]
#                [PUBLIC_DEFINITIONS  [item1 ...]]
#                [PRIVATE_DEFINITIONS [item1 ...]]
#                [RESOURCES [item1..]]
#
# SOURCES are the sources of the library
# HEADERS are the headers files associated to the library
# PUBLIC_INCLUDES an optional list of include directories that are public dependencies
# PRIVATE_INCLUDES an optional list of include directories that are private dependencies
# PUBLIC_LINKS an optional list of targets/libraries to link with as public dependencies
# PRIVATE_LINKS an optional list of targets/libraries to link with as private dependencies
# PUBLIC_DEFINITIONS an optional list of public definitions to add to the target
# PRIVATE_DEFINITIONS an optional list of private definitions to add to the target
# RESOURCES an optional list of resource files to export with the library
#
#
function(logl_add_library target_name)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs SOURCES HEADERS PUBLIC_INCLUDES PRIVATE_INCLUDES PUBLIC_LINKS PRIVATE_LINKS PUBLIC_DEFINITIONS PRIVATE_DEFINITIONS RESOURCES)

    cmake_parse_arguments(_params "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    string(REPLACE "_" "/" _projectPath ${target_name})
    string(TOUPPER ${target_name} PROJECT_NAME_UPPERCASE)

    if(NOT _params_SOURCES)
        add_library(${target_name} INTERFACE ${_params_SOURCES} ${_params_HEADERS} ${_params_RESOURCES})
        target_link_libraries(${target_name} INTERFACE ${_params_PUBLIC_LINKS} ${_params_PRIVATE_LINKS})
        target_include_directories(${target_name} INTERFACE ${_params_PUBLIC_INCLUDES} ${_params_PRIVATE_INCLUDES})
        set(compile_option_visibility INTERFACE)
    else()
        add_library(${target_name} ${_params_SOURCES} ${_params_HEADERS} ${_params_RESOURCES})
        target_link_libraries(${target_name} PUBLIC  ${_params_PUBLIC_LINKS} PRIVATE ${_params_PRIVATE_LINKS})
        target_include_directories(${target_name} PUBLIC ${_params_PUBLIC_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} PRIVATE ${_params_PRIVATE_INCLUDES})
        target_compile_definitions(${target_name} PUBLIC ${_params_PUBLIC_DEFINITIONS} PRIVATE ${_params_PRIVATE_DEFINITIONS})
        set(compile_option_visibility PRIVATE)

        include(GenerateExportHeader)
        generate_export_header(${target_name}
            EXPORT_FILE_NAME "${_projectPath}/export.h"
            EXPORT_MACRO_NAME "${PROJECT_NAME_UPPERCASE}_EXPORT"
            DEPRECATED_MACRO_NAME "${PROJECT_NAME_UPPERCASE}_DEPRECATED"
                # TODO ADD OTHER MACROS
        )
    endif()

    set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)

    target_compile_options(${target_name} ${compile_option_visibility}
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wmisleading-indentation
        -Wdouble-promotion
        -Wformat=2
        -Wno-format-y2k
        -Werror
        -Wno-unused-parameter
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
    )

    logl_manage_resources(${target_name} "${_params_RESOURCES}")
endfunction()

#
# Create a new executable
#
# Usage:
#   logl_add_executable(
#                SOURCES [item1...]
#                HEADERS [item1...]
#                [INCLUDE_DIRS [item1...] ]
#                [PRIVATE_INCLUDES [item1...] ]
#                [LINKS [item1...] ]
#                [DEFINITIONS [item1...] ]
#
# SOURCES are the sources of the executable
# HEADERS are the headers files associated to the executable
# INCLUDE_DIRS an optional list of include directories that are private dependencies
# LINKS an optional list of targets/libraries to link with as private dependencies
# DEFINITIONS an optional list of public definitions to add to the target
#
#
function(logl_add_executable target_name)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs SOURCES HEADERS INCLUDE_DIRS LINKS DEFINITIONS RESOURCES)

    cmake_parse_arguments(_params "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    string(REPLACE "_" "/" _projectPath ${target_name})
    string(TOUPPER ${target_name} PROJECT_NAME_UPPERCASE)

    add_executable(${target_name} ${_params_SOURCES} ${_params_HEADERS})

    target_link_libraries(${target_name} PRIVATE ${_params_LINKS})

    target_include_directories(${target_name} PRIVATE ${_params_INCLUDE_DIRS})

    target_compile_options(${target_name} PRIVATE
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wmisleading-indentation
        -Wdouble-promotion
        -Wformat=2
        -Wno-format-y2k
        -Werror
        -Wno-unused-parameter
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
    )

    target_compile_definitions(${target_name} PRIVATE ${_params_DEFINITIONS} "-D${PROJECT_NAME_UPPERCASE}_EXPORTS")

    logl_manage_resources(${target_name} "${_params_RESOURCES}")
endfunction()