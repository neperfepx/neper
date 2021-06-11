# This file is part of the Neper software package.
# Copyright(C) 2003-2019, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 3.0)
project(muparser CXX)

file(GLOB headers "include/*.h")
file(GLOB sources "src/*.cpp")

# disabling all compilation warnings
add_definitions(-w -fPIC)

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_library(muparser ${sources})

target_link_libraries(muparser  ${EXTRA_LIBS})

include_directories(include)
