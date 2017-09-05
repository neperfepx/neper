# This file is part of the Neper software package.
# Copyright (C) 2003-2017, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required (VERSION 2.8)
project (muparser CXX)

option (DEVEL_OPTIMIZATION "Enable optimization (ON for production use)" ON)
option (DEVEL_DEBUGGING_FLAG "Enable debugging compilation flag (OFF for production use)" OFF)

file (GLOB headers "include/*.h")
file (GLOB sources "src/*.cpp")

if (DEVEL_OPTIMIZATION)
  add_definitions(-O3)
endif ()

if (DEVEL_DEBUGGING_FLAG)
  add_definitions(-g)
endif ()

# disabling all compilation warnings
add_definitions(-w)

set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_library(muparser ${sources})

target_link_libraries (muparser  ${EXTRA_LIBS})

include_directories (include)
