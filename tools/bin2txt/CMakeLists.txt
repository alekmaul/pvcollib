cmake_minimum_required (VERSION 3.1)

project(bin2txt C)

set(bin2txt_major_version 1)
set(bin2txt_minor_version 0)
set(bin2txt_path_version 0)
set(bin2txt_version "${bin2txt_major_version}.${bin2txt_minor_version}.${bin2txt_path_version}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

set(CMAKE_C_STANDARDS 11)

configure_file(config.h.in config.h)

set(bin2txt_sources
	bin2txt.c
)

set(bin2txt_headers
    config.h
)

add_executable(bin2txt ${bin2txt_sources} ${bin2txt_headers})
target_compile_options(bin2txt PRIVATE -Wall -Wshadow -Wextra)
target_include_directories(bin2txt PRIVATE ${CMAKE_BINARY_DIR})

install(TARGETS bin2txt RUNTIME DESTINATION bin)
