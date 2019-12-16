cmake_minimum_required (VERSION 3.1)

project(gfx2col C)

set(gfx2col_major_version 1)
set(gfx2col_minor_version 0)
set(gfx2col_path_version 0)
set(gfx2col_version "${gfx2col_major_version}.${gfx2col_minor_version}.${gfx2col_path_version}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

set(CMAKE_C_STANDARDS 11)

configure_file(config.h.in config.h)

set(gfx2col_sources
	comptool.c
    gfx2col.c
    lodepng.c
    lodepic.c
)

set(gfx2col_headers
	comptool.h
    gfx2col.h
    lodepng.h
    lodepic.h
    config.h
)

add_executable(gfx2col ${gfx2col_sources} ${gfx2col_headers})
target_compile_options(gfx2col PRIVATE -Wall -Wshadow -Wextra)
target_include_directories(gfx2col PRIVATE ${CMAKE_BINARY_DIR})

install(TARGETS gfx2col RUNTIME DESTINATION bin)
