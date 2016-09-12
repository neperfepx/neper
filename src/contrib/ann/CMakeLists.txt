# This file is part of the Neper software package.
# Copyright (C) 2003-2016, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 2.6)
project(ann)

set (ANN_VERSION_MAJOR 1)
set (ANN_VERSION_MINOR 1)
set (ANN_VERSION_PATCH 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/ann_config.h.in"
  "${PROJECT_BINARY_DIR}/ann_config.h"
)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

add_definitions(-Wall -W -g -O3)
set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_library (ann
ANN.cpp
bd_fix_rad_search.cpp
bd_pr_search.cpp
bd_search.cpp
bd_tree.cpp
brute.cpp
kd_dump.cpp
kd_fix_rad_search.cpp
kd_pr_search.cpp
kd_search.cpp
kd_split.cpp
kd_tree.cpp
kd_util.cpp
perf.cpp
)

## Link libraries

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

target_link_libraries (ann ${EXTRA_LIBS})

# Installation of the library
# SET (CMAKE_INSTALL_PREFIX /usr/local/lib/ut)
# SET (DESTDIR /usr/local/lib/ut)

set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/bd_tree.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/kd_fix_rad_search.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/kd_pr_search.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/kd_search.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/kd_split.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/kd_tree.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/kd_util.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/pr_queue.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/pr_queue_k.h)
