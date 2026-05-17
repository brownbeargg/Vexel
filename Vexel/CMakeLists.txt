cmake_minimum_required(VERSION 3.21)
project(Vexel)

file(GLOB_RECURSE VEXEL_SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Vexel/*.cpp
)

if(WIN32)
    file(GLOB_RECURSE VEXEL_SRC_FILES_PLATFORM CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/Windows/*.cpp
    )
else()
    message(FATAL_ERROR "VEXEL ENGINE DOES NOT SUPPORT THIS PLATFORM")
endif()

add_library(Vexel STATIC
    ${VEXEL_SRC_FILES}
    ${VEXEL_SRC_FILES_PLATFORM}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Vendor/VexelUtils)
target_link_libraries(Vexel
    PUBLIC
    VexelUtils
)

target_include_directories(Vexel
    PUBLIC

    # CLIENT
    ${CMAKE_CURRENT_SOURCE_DIR}/Include

    # ENGINE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)
