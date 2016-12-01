# This file is part of the Neper software package.
# Copyright (C) 2003-2016, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required (VERSION 2.8)
project (muparser CXX)

option (DEVEL_OPTIMIZATION "Enable optimization (ON for production use)"
ON)
option (DEVEL_DEBUGGING_FLAG "Enable debugging compilation flag (OFF for
production use)" OFF)

file (GLOB headers "include/*.h")
file (GLOB sources "src/*.cpp")

add_definitions(-g -O3)

set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_library(muparser ${sources})

target_link_libraries (muparser  ${EXTRA_LIBS})

include_directories (include)
