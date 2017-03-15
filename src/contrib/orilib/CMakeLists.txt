# This file is part of the 'orilib' software.
# Copyright (C) 2007-2009, 2012 Romain Quey
# see the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 2.6)
project(orilib C)

set (ORILIB_VERSION_MAJOR 2)
set (ORILIB_VERSION_MINOR 1)
set (ORILIB_VERSION_PATCH 1)
set (ORILIB_VERSION_DEVEL 2)

# should we use the optional libraries?
# option (HAVE_GSL      "Use the GNU Scientific Library (GSL)" ON)
option (DEVEL_OPTIMIZATION "Enable optimization (ON for production use)" ON)
option (DEVEL_DEBUGGING_FLAG "Enable debugging compilation flag (OFF for production use)" OFF)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/ol_config.h.in"
  "${PROJECT_BINARY_DIR}/ol_config.h"
)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_BINARY_DIR}/contrib/ut")
include_directories("${PROJECT_SOURCE_DIR}/contrib/ut")

add_definitions(-Wall -W)
if (DEVEL_OPTIMIZATION)
  add_definitions(-O3)
endif ()

if (DEVEL_DEBUGGING_FLAG)
  add_definitions(-g -Werror)
endif ()

set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_library (orilib
ol_cal/ol_com.c
ol_cal/ol_csys.c
ol_cal/ol_dis.c
ol_cal/ol_inv.c
ol_cal/ol_sym.c
ol_cal/ol_mis.c
ol_des/ol_des_gen.c
ol_des/ol_e.c
ol_des/ol_g.c
ol_des/ol_m.c
ol_des/ol_pole.c
ol_des/ol_q.c
ol_des/ol_R.c
ol_des/ol_rtheta.c
ol_gen/ol_nb_e.c
ol_gen/ol_label_g.c
ol_gen/ol_nb_rtheta.c
ol_set/ol_mean.c
ol_set/ol_set_alloc.c
ol_set/ol_set_aniso.c
ol_set/ol_set_sample.c
ol_set/ol_spread.c
)

## Link libraries
# this is for the GSL
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

if (HAVE_GSL)
  find_package(GSL REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
  include_directories (${GSL_INCLUDE_DIRS})
endif ()

target_link_libraries (orilib ${EXTRA_LIBS})

# INCLUDE_DIRECTORIES(${EXTRA_LIBS})

# Installation of the library
# SET (CMAKE_INSTALL_PREFIX /usr/local/lib)

set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ol_des.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ol_cal.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ol_gen.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ol_set.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ol_glo.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/ol_config.h)
set (INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/orilib.h)
