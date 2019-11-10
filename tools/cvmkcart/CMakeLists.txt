cmake_minimum_required (VERSION 3.1)

project(cvmkcart C)

set(cvmkcart_major_version 1)
set(cvmkcart_minor_version 0)
set(cvmkcart_path_version 0)
set(cvmkcart_version "${cvmkcart_major_version}.${cvmkcart_minor_version}.${cvmkcart_path_version}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

set(CMAKE_C_STANDARDS 11)

configure_file(config.h.in config.h)

set(cvmkcart_sources
    cvmkcart.c
)

set(cvmkcart_headers
    cvmkcart.h
    config.h
)

add_executable(cvmkcart ${cvmkcart_sources} ${cvmkcart_headers})
target_compile_options(cvmkcart PRIVATE -Wall -Wshadow -Wextra)
target_include_directories(cvmkcart PRIVATE ${CMAKE_BINARY_DIR})

install(TARGETS cvmkcart RUNTIME DESTINATION bin)
