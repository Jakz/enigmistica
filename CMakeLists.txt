cmake_minimum_required(VERSION 3.5.0)

project (enigmistica)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/projects/cmake")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_COMPILER "$ENV{CROSS}g++" CACHE PATH "" FORCE)
set(CMAKE_C_COMPILER "$ENV{CROSS}gcc" CACHE PATH "" FORCE)
set(CMAKE_SYSROOT "/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(ZLIB REQUIRED)

add_compile_options(-Wno-unused-parameter -Wno-missing-field-initializers
  -Wno-sign-compare -Wno-parentheses -Wno-unused-variable
)

# add_compile_options(-g -O2 -W -Wall -Wextra)
add_compile_options(-g)
add_compile_options(
  $<$<COMPILE_LANGUAGE:CXX>:-Wno-reorder>
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
)

include_directories(src)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS})

set(SRC_ROOT "${CMAKE_SOURCE_DIR}/src")

file(GLOB SOURCES_ROOT "${SRC_ROOT}/*.cpp")
file(GLOB SOURCES_GFX "${SRC_ROOT}/gfx/*.cpp")
file(GLOB SOURCES_VIEWS "${SRC_ROOT}/gfx/views/*.cpp")
file(GLOB SOURCES_GAMES "${SRC_ROOT}/games/*.cpp")

set(SOURCES ${SOURCES_ROOT} ${SOURCES_VIEWS} ${SOURCES_GFX} ${SOURCES_GAMES})

add_executable(enigmistica ${SOURCES})

target_link_libraries(enigmistica ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${ZLIB_LIBRARIES})
