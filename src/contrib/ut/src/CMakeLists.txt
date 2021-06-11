# Copyright(C) 2003-2021, Romain Quey
# see the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 3.0)
if(POLICY CMP0012)
  cmake_policy(SET CMP0012 NEW)
endif()
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()
project(ut)

set(UT_VERSION_MAJOR 1)
set(UT_VERSION_MINOR 0)
set(UT_VERSION_PATCH 0)

# should we use the optional libraries?
option(HAVE_GSL      "Use the GNU Scientific Library(GSL)" ON)
option(HAVE_MUPARSER "Use muparser"                         OFF)
option(HAVE_OPENGJK  "Use openGJK"                          OFF)
option(HAVE_NLOPT    "Use NLopt"                            OFF)
option(HAVE_OPENMP   "Use OpenMP"                           ON)
option(HAVE_PNG      "Use the libpng library"               ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/ut_config.h.in"
  "${PROJECT_BINARY_DIR}/ut_config.h"
)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})

if(HAVE_MUPARSER)
  if(DEFINED MUPARSER_INCLUDE_DIR AND DEFINED MUPARSER_BINARY_DIR)
    include_directories(${MUPARSER_INCLUDE_DIR} ${MUPARSER_BINARY_DIR})
  elseif(EXISTS ${PROJECT_SOURCE_DIR}/contrib/muparser)
      include_directories(${PROJECT_SOURCE_DIR}/contrib/muparser
                          ${PROJECT_BINARY_DIR}/contrib/muparser)
      set(MUPARSER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/contrib/muparser")
      set(MUPARSER_BINARY_DIR "${PROJECT_BINARY_DIR}/contrib/muparser")
      set(MUPARSER_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/muparser/include")
      add_subdirectory(contrib/muparser)
    include_directories(${MUPARSER_INCLUDE_DIR} ${MUPARSER_BINARY_DIR})
  else()
    message(FATAL_ERROR "Failed to find muparser")
  endif()
endif()

if(HAVE_OPENGJK)
  include_directories(${OPENGJK_INCLUDE_DIR} ${OPENGJK_BINARY_DIR})
endif()

add_definitions(-Wall -W -Wno-unknown-pragmas -Wno-deprecated-declarations)

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

if(NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif()

add_library(ut
  ut_alloc/ut_alloc.c
  ut_arg/ut_arg.c
  ut_array/ut_array.c
  ut_color/ut_color.c
  ut_file/ut_file.c
  ut_free/ut_free.c
  ut_math/ut_math.cpp
  ut_fct/ut_fct.c
  ut_xfig/ut_xfig.c
  ut_stat/ut_stat.c
  ut_mat/ut_mat.c
  ut_linalg/ut_linalg.c
  ut_num/ut_num.c
  ut_print/ut_print.c
  ut_space/ut_space.c
  ut_space/ut_space_ellipse/ut_space_ellipse.c
  ut_string/ut_string.c
  ut_list/ut_list.c
  ut_sys/ut_sys.c
  ut_tensor/ut_tensor.c
  ut_time/ut_time.c
  ut_vector/ut_vector.c
  ut_png/ut_png.c
  ut_nlopt/ut_nlopt.c
)

## Link libraries

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library(MATHLIB m)
if(MATHLIB)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif(MATHLIB)

if(HAVE_GSL)
  find_package(GSL REQUIRED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
  include_directories(${GSL_INCLUDE_DIRS})
endif(HAVE_GSL)

if(HAVE_PNG)
  find_package(PNG REQUIRED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${PNG_LIBRARIES})
  include_directories(${PNG_INCLUDE_DIRS})
endif()

if(HAVE_OPENMP)
  if(APPLE AND XCODE)
      set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include" CACHE INTERNAL "OpenMP flags for Xcode toolchain.")
      set(OpenMP_CXX_LIB_NAMES "omp" CACHE INTERNAL "OpenMP lib name for Xcode toolchain.")
      set(OpenMP_omp_LIBRARY "/usr/local/opt/libomp/lib/libomp.dylib" CACHE INTERNAL "OpenMP lib name for Xcode toolchain.")
  endif()

  find_package(OpenMP REQUIRED)
  add_definitions(${OpenMP_C_FLAGS})
endif()

if(HAVE_MUPARSER)
  set(EXTRA_LIBS ${EXTRA_LIBS} muparser)
endif()

if(HAVE_OPENGJK)
  set(EXTRA_LIBS ${EXTRA_LIBS} openGJKlib)
endif()

target_link_libraries(ut ${EXTRA_LIBS})

# Installation of the library
SET(CMAKE_INSTALL_PREFIX /usr/local/lib/ut)
SET(DESTDIR /usr/local/lib/ut)

set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_alloc.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_arg.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_array.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_color.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_file.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_free.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_math.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_mat.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_linalg.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_num.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_print.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_space.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_string.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_sys.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_tensor.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_time.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_vector.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut_nlopt.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_SOURCE_DIR}/ut.h)
set(INCLUDE_FILES ${INCLUDE_FILES} ${PROJECT_BINARY_DIR}/ut_config.h)

