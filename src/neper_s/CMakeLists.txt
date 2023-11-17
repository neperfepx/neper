# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 3.0)
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(neper_s C)

add_definitions(-Wall -W -g -O3)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-copy")
endif()
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

# Set the build type for appropriate compile flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build: Debug Release" FORCE)
endif()

add_library(neper_s
neper_s.c
nes_utils/nes_utils1.c
nes_input/nes_input1.c
nes_input/nes_input2.c
nes_input/nes_input3.c
nes_merge/nes_merge1.c
nes_merge/nes_merge2.c
nes_convert/nes_convert1.c
nes_convert/nes_convert2.c
nes_convert/nes_convert_write/nes_convert_write1.c
nes_convert/nes_convert_write/nes_convert_write2.c
nes_convert/nes_convert_write/nes_convert_write3.c
nes_convert/nes_convert_write/nes_convert_write4.c
nes_orispace/nes_orispace1.c
nes_entity/nes_entity1.c
nes_pproc/nes_pproc1.c
nes_pproc/nes_pproc2.c
nes_pproc/nes_pproc3.c
nes_pproc/nes_pproc_entity/nes_pproc_entity1.c
nes_pproc/nes_pproc_entity/nes_pproc_entity2.c
nes_pproc/nes_pproc_entity/nes_pproc_entity_builtin/nes_pproc_entity_builtin1.c
nes_pproc/nes_pproc_entity/nes_pproc_entity_builtin/nes_pproc_entity_builtin2.c
nes_pproc/nes_pproc_entity/nes_pproc_entity_builtin/nes_pproc_entity_builtin3.cpp
nes_pproc/nes_pproc_entity/nes_pproc_entity_builtin/nes_pproc_entity_builtin4.c
)

list (APPEND EXTRA_LIBS neut)
target_link_libraries(neper_s ${EXTRA_LIBS})
