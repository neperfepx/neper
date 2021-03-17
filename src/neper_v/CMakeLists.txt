# This file is part of the Neper software package.
# Copyright(C) 2003-2020, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 2.8)
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(neper_v C)

option(HAVE_GSL         "Use the GNU Scientific Library(GSL)" ON )
option(HAVE_NLOPT       "Use NLopt"                            ON )
option(HAVE_LIBSCOTCH   "Use libscotch"                        OFF)
option(HAVE_OPENMP      "Use OpenMP"                           ON )
option(DEVEL_OPTIMIZATION "Enable optimization(ON for production use)" ON)
option(DEVEL_DEBUGGING_TEST "Enable debugging tests(OFF for production use)"   OFF)
option(DEVEL_DEBUGGING_FLAG "Enable debugging compilation flag(OFF for production use)"   OFF)
option(DEVEL_PROFILING   "Enable profiling(OFF for production use)" OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -Wno-unknown-pragmas)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-copy")
endif()
if(DEVEL_OPTIMIZATION)
  add_definitions(-O3)
endif()

if(DEVEL_DEBUGGING_FLAG)
  add_definitions(-g -Werror)
endif()

if(DEVEL_PROFILING)
  add_definitions(-pg)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()

add_library(neper_v
./nev_show/nev_show_gen/nev_show_gen1.c
./nev_show/nev_show_utils/nev_show_utils1.c
./nev_show/nev_show_tess/nev_show_tess1.c
./nev_show/nev_show_tesr/nev_show_tesr1.c
./nev_show/nev_show_mesh/nev_show_mesh1.c
./nev_show/nev_show_point/nev_show_point1.c
./nev_show/nev_show1.c
./nev_slice/nev_slice1.c
./nev_include/nev_include1.c
./nev_image/nev_image1.c
./neper_v.c
./nev_sim/nev_sim1.c
./nev_space/nev_space1.c
./nev_outdir/nev_outdir1.c
./nev_load/nev_load1.c
./nev_load/nev_load2.c
./nev_data/nev_data1.c
./nev_data/nev_data_tess/nev_data_tess1.c
./nev_data/nev_data_tess/nev_data_tess2.c
./nev_data/nev_data_tesr/nev_data_tesr1.c
./nev_data/nev_data_tesr/nev_data_tesr2.c
./nev_data/nev_data_mesh/nev_data_mesh1.c
./nev_data/nev_data_mesh/nev_data_mesh2.c
./nev_data/nev_data_mesh/nev_data_mesh3.c
./nev_data/nev_data_nodes/nev_data_nodes1.c
./nev_data/nev_data_points/nev_data_points1.c
./nev_data/nev_data_csys/nev_data_csys1.c
./nev_camera/nev_camera1.c
./nev_povray/nev_povray1.c
./nev_print/nev_print1.c
./nev_print/nev_print_init/nev_print_init1.c
./nev_print/nev_print_init/nev_print_init2.c
./nev_print/nev_print_init/nev_print_init3.c
./nev_print/nev_print_init/nev_print_init4.c
./nev_print/nev_print_png/nev_print_png1.c
./nev_print/nev_print_png/nev_print_png2.c
./nev_print/nev_print_png/nev_print_png_utils/nev_print_png_utils1.c
./nev_print/nev_print_png/nev_print_png_utils/nev_print_png_utils2.c
./nev_print/nev_print_png/nev_print_png_foot/nev_print_png_foot1.c
./nev_print/nev_print_png/nev_print_png_csys/nev_print_png_csys1.c
./nev_print/nev_print_png/nev_print_png_header/nev_print_png_header1.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh1.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_nodes/nev_print_png_mesh_nodes1.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_0d/nev_print_png_mesh_0d1.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_1d/nev_print_png_mesh_1d1.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_2d/nev_print_png_mesh_2d1.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_2d/nev_print_png_mesh_2d2.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_2d/nev_print_png_mesh_2d3.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_3d/nev_print_png_mesh_3d1.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_3d/nev_print_png_mesh_3d2.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_3d/nev_print_png_mesh_3d3.c
./nev_print/nev_print_png/nev_print_png_tess/nev_print_png_tess1.c
./nev_print/nev_print_png/nev_print_png_tesr/nev_print_png_tesr1.c
./nev_print/nev_print_png/nev_print_png_point/nev_print_png_point1.c
./nev_print/nev_print_vtk/nev_print_vtk1.c
)

set(EXTRA_LIBS ${EXTRA_LIBS} ut)
set(EXTRA_LIBS ${EXTRA_LIBS} neut)
set(EXTRA_LIBS ${EXTRA_LIBS} orilib)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library(MATHLIB m)
if(MATHLIB)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif()

if(HAVE_GSL)
  find_package(GSL REQUIRED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
  include_directories(${GSL_INCLUDE_DIRS})
endif()

if(HAVE_LIBSCOTCH)
  find_package(LIBSCOTCH REQUIRED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories(${Scotch_INCLUDE_DIRS})
endif()

if(HAVE_NLOPT)
  find_package(NLOPT REQUIRED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${NLOPT_LIBRARIES})
  include_directories(${NLOPT_INCLUDE_DIRS})
endif()

if(HAVE_OPENMP)
  if(APPLE AND XCODE)
      set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include" CACHE INTERNAL "OpenMP flags for Xcode toolchain.")
      set(OpenMP_CXX_LIB_NAMES "omp" CACHE INTERNAL "OpenMP lib name for Xcode toolchain.")
      set(OpenMP_omp_LIBRARY "/usr/local/opt/libomp/lib/libomp.dylib" CACHE INTERNAL "OpenMP lib name for Xcode toolchain.")
  endif()

  find_package(OpenMP REQUIRED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${OpenMP_C_FLAGS})
  add_definitions(${OpenMP_C_FLAGS})
endif()

target_link_libraries(neper_v ${EXTRA_LIBS})
