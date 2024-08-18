cmake_minimum_required(VERSION 3.0)

project(gbc-multi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SRC src/*.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

set(GBC_SOURCES
    vendor/gamebro/libgbc/apu.cpp
    vendor/gamebro/libgbc/cpu.cpp
    vendor/gamebro/libgbc/debug.cpp
    vendor/gamebro/libgbc/gpu.cpp
    vendor/gamebro/libgbc/io.cpp
    vendor/gamebro/libgbc/machine.cpp
    vendor/gamebro/libgbc/mbc.cpp
    vendor/gamebro/libgbc/memory.cpp
)

add_library(gbc STATIC ${GBC_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    gbc
    ${SDL2_LIBRARIES}
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/vendor/gamebro/
    ${SDL2_INCLUDE_DIRS}
)
