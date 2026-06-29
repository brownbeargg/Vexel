cmake_minimum_required(VERSION 3.21)
project(Vexel)

file(GLOB_RECURSE VEXEL_SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Vexel/*.cpp
)

if(WIN32)
    file(GLOB_RECURSE VEXEL_SRC_FILES_PLATFORM CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/Windows/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/Vulkan/*.cpp
    )
else()
    message(FATAL_ERROR "VEXEL ENGINE DOES NOT SUPPORT THIS PLATFORM")
endif()

add_library(Vexel STATIC
    ${VEXEL_SRC_FILES}
    ${VEXEL_SRC_FILES_PLATFORM}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Vendor/VexelUtils)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Vendor/GLFW)

find_package(Vulkan REQUIRED)

target_link_libraries(Vexel
    PUBLIC
    VexelUtils

    glfw
    Vulkan::Vulkan
)

target_include_directories(Vexel
    PUBLIC

    # CLIENT
    ${CMAKE_CURRENT_SOURCE_DIR}/Include

    # ENGINE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source

    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/GLFW/include
)

find_program(GLSLANG_VALIDATOR glslangValidator REQUIRED)

set(SHADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/Basic.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/Basic.frag
)

set(COMPILED_SHADERS)

foreach(SHADER ${SHADERS})
        get_filename_component(FILE_NAME ${SHADER} NAME)
        set(OUT_FILE ${CMAKE_BINARY_DIR}/Assets/Shaders/${FILE_NAME}.spv)
        

        add_custom_command(
            OUTPUT ${OUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Assets/Shaders
            COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER} -o ${OUT_FILE}
            DEPENDS ${SHADER}
            COMMENT "Compiling shader ${FILE_NAME}"
        )

        list(APPEND COMPILED_SHADERS ${OUT_FILE})
endforeach()

add_custom_target(CompileShaders ALL
    DEPENDS ${COMPILED_SHADERS}
)

add_dependencies(Vexel CompileShaders)

file(TOUCH ${CMAKE_BINARY_DIR}/vexel.build)
