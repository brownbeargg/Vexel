cmake_minimum_required(VERSION 3.21)
project(Vexis)

file(GLOB_RECURSE VEXIS_SRC_FILES CONFIGURE_DEPENDS
    Source/*.cpp
)

add_executable(Vexis ${VEXIS_SRC_FILES})

target_include_directories(Vexis 
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(Vexis
    PRIVATE
    Vexel
)
