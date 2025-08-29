# This file is part of the Neper software package.
# Copyright (C) 2003-2024, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 3.0...3.10)
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(neper_v C)

add_definitions(-Wall -W -Wno-unknown-pragmas)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-copy")
endif()

# Set the build type for appropriate compile flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build: Debug Release" FORCE)
endif()

add_library(neper_v
./nev_print/nev_print_show/nev_print_show_utils/nev_print_show_utils1.c
./nev_print/nev_print_show/nev_print_show_tess/nev_print_show_tess1.c
./nev_print/nev_print_show/nev_print_show_tesr/nev_print_show_tesr1.c
./nev_print/nev_print_show/nev_print_show_mesh/nev_print_show_mesh1.c
./nev_print/nev_print_show/nev_print_show_point/nev_print_show_point1.c
./nev_print/nev_print_show/nev_print_show1.c
./neper_v.c
./nev_load/nev_load1.c
./nev_load/nev_load2.c
./nev_print/nev_print_data/nev_print_data1.c
./nev_print/nev_print_data/nev_print_data_tess/nev_print_data_tess1.c
./nev_print/nev_print_data/nev_print_data_tess/nev_print_data_tess2.c
./nev_print/nev_print_data/nev_print_data_tesr/nev_print_data_tesr1.c
./nev_print/nev_print_data/nev_print_data_tesr/nev_print_data_tesr2.c
./nev_print/nev_print_data/nev_print_data_mesh/nev_print_data_mesh1.c
./nev_print/nev_print_data/nev_print_data_mesh/nev_print_data_mesh2.c
./nev_print/nev_print_data/nev_print_data_mesh/nev_print_data_mesh3.c
./nev_print/nev_print_data/nev_print_data_nodes/nev_print_data_nodes1.c
./nev_print/nev_print_data/nev_print_data_points/nev_print_data_points1.c
./nev_print/nev_print_data/nev_print_data_csys/nev_print_data_csys1.c
./nev_print/nev_print1.c
./nev_print/nev_print_init/nev_print_init1.c
./nev_print/nev_print_init/nev_print_init2.c
./nev_print/nev_print_init/nev_print_init3.c
./nev_print/nev_print_init/nev_print_init4.c
./nev_print/nev_print_utils/nev_print_utils1.c
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
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_1d/nev_print_png_mesh_1d2.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_2d/nev_print_png_mesh_2d1.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_2d/nev_print_png_mesh_2d2.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_2d/nev_print_png_mesh_2d3.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_3d/nev_print_png_mesh_3d1.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_3d/nev_print_png_mesh_3d2.c
./nev_print/nev_print_png/nev_print_png_mesh/nev_print_png_mesh_3d/nev_print_png_mesh_3d3.c
./nev_print/nev_print_png/nev_print_png_tess/nev_print_png_tess1.c
./nev_print/nev_print_png/nev_print_png_tesr/nev_print_png_tesr1.c
./nev_print/nev_print_png/nev_print_png_point/nev_print_png_point1.c
./nev_print/nev_print_scale/nev_print_scale1.c
./nev_print/nev_print_scale/nev_print_scale2.c
./nev_print/nev_print_vtk/nev_print_vtk1.c
./nev_print/nev_print_vtk/nev_print_vtk2.c
./nev_print/nev_print_tree/nev_print_tree1.c
./nev_print/nev_print_pf/nev_print_pf1.c
./nev_print/nev_print_pf/nev_print_pf2.c
./nev_print/nev_print_pf/nev_print_pf3.c
./nev_print/nev_print_pf/nev_print_pf4.c
./nev_print/nev_print_pf/nev_print_pf_ptsprint_density/nev_print_pf_ptsprint_density1.c
./nev_print/nev_print_pf/nev_print_pf_ptsprint_density/nev_print_pf_ptsprint_density2.c
./nev_print/nev_print_pf/nev_print_pf_ptsprint_density/nev_print_pf_ptsprint_density3.c
./nev_input/nev_input1.c
./nev_input/nev_input2.c
./nev_input/nev_input3.c
)

list (APPEND EXTRA_LIBS neut)
target_link_libraries(neper_v ${EXTRA_LIBS})
