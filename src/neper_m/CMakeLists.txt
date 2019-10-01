# This file is part of the Neper software package.
# Copyright (C) 2003-2019, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 2.8)
project(neper_m C)

option (HAVE_GSL         "Use the GNU Scientific Library (GSL)" ON )
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)
option (HAVE_NLOPT       "Use NLopt"                            ON )
option (HAVE_PTHREAD     "Use pthread"                          OFF)
option (HAVE_OPENMP      "Use OpenMP"                           ON )
option (DEVEL_OPTIMIZATION "Enable optimization (ON for production use)" ON)
option (DEVEL_DEBUGGING_TEST "Enable debugging tests (OFF for production use)"   OFF)
option (DEVEL_DEBUGGING_FLAG "Enable debugging compilation flag (OFF for production use)"   OFF)
option (DEVEL_PROFILING   "Enable profiling (OFF for production use)" OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -Wno-unknown-pragmas)
if (DEVEL_OPTIMIZATION)
  add_definitions(-O3)
endif ()

if (DEVEL_DEBUGGING_FLAG)
  add_definitions(-g -Werror)
endif ()

if (DEVEL_PROFILING)
  add_definitions(-pg)
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif ()

add_library (neper_m
./nem_stat/nem_stat1.c
./nem_stat/nem_stat2.c
./nem_readmesh/nem_readmesh1.c
./nem_meshing/nem_meshing1.c
./nem_meshing/nem_meshing2.c
./nem_meshing_para/nem_meshing_para.c
./nem_meshing_para/nem_meshing_para_faceproj/nem_meshing_para_faceproj1.c
./nem_meshing_para/nem_meshing_para_faceproj/nem_meshing_para_faceproj2.c
./nem_meshing_para/nem_meshing_para_scale/nem_meshing_para_scale1.c
./nem_meshing_para/nem_meshing_para_cl/nem_meshing_para_cl1.c
./nem_meshing_para/nem_meshing_para_cl/nem_meshing_para_cl2.c
./nem_meshing_para/nem_meshing_para_cl/nem_meshing_para_cl3.c
./nem_meshing_para/nem_meshing_para_mesh3dclreps/nem_meshing_para_mesh3dclreps1.c
./nem_meshing_para/nem_meshing_para_param/nem_meshing_para_param1.c
./nem_meshing/nem_meshing_0D/nem_meshing_0D2.c
./nem_meshing/nem_meshing_0D/nem_meshing_0D1.c
./nem_meshing/nem_meshing_1D/nem_meshing_1D1.c
./nem_meshing/nem_meshing_1D/nem_meshing_1D2.c
./nem_meshing/nem_meshing_1D/nem_meshing_1D3.c
./nem_meshing/nem_meshing_1D/nem_meshing_1D4.c
./nem_meshing/nem_meshing_1D/nem_meshing_1D5.c
./nem_meshing/nem_meshing_1D/nem_meshing_1D_useg/nem_meshing_1D_useg1.c
./nem_meshing/nem_meshing_1D/nem_meshing_1D_useg/nem_meshing_1D_useg2.c
./nem_meshing/nem_meshing_2D/nem_meshing_2D1.c
./nem_meshing/nem_meshing_2D/nem_meshing_2D2.c
./nem_meshing/nem_meshing_2D/nem_meshing_2D3.c
./nem_meshing/nem_meshing_2D/nem_meshing_2D_face_mesh_gmsh/nem_meshing_2D_face_mesh_gmsh1.c
./nem_meshing/nem_meshing_2D/nem_meshing_2D_face_mesh_gmsh/nem_meshing_2D_face_mesh_gmsh2.c
./nem_meshing/nem_meshing_2D/nem_meshing_2D_face_mesh_gmsh/nem_meshing_2D_face_mesh_gmsh3.c
./nem_meshing/nem_meshing_3D/nem_meshing_3D1.c
./nem_meshing/nem_meshing_3D/nem_meshing_3D2.c
./nem_meshing/nem_meshing_3D/nem_meshing_3D3.c
./nem_meshing/nem_meshing_3D/nem_meshing_3D4.c
./nem_meshing/nem_meshing_pinching/nem_meshing_pinching1.c
./nem_meshing/nem_meshing_pinching/nem_meshing_pinching2.c
./nem_meshing/nem_meshing_pinching/nem_meshing_pinching3.c
./nem_interface/nem_interface1.c
./nem_interface/nem_interface_dup/nem_interface_dup1.c
./nem_interface/nem_interface_dup/nem_interface_dup2.c
./nem_interface/nem_interface_dup/nem_interface_dup3.c
./nem_interface/nem_interface_cohesive/nem_interface_cohesive1.c
./nem_order/nem_order1.c
./nem_order/nem_order2.c
./nem_transform/nem_transform1.c
./nem_transform/nem_transform2.c
./nem_cleaning/nem_cleaning1.c
./nem_input/nem_input1.c
./nem_input/nem_input2.c
./nem_input/nem_input3.c
./nem/nem_tess/nem_tess1.c
./nem/nem_tess/nem_tess2.c
./nem/nem_tess/nem_tess3.c
./nem_smoothing/nem_smoothing1.c
./nem_smoothing/nem_smoothing2.c
./nem_smoothing/nem_smoothing3.c
./nem/nem_mesh_gmsh/nem_mesh_gmsh3.c
./nem/nem_mesh_gmsh/nem_mesh_gmsh2.c
./nem/nem_mesh_gmsh/nem_mesh_gmsh1.c
./nem_transport/nem_transport1.c
./nem_transport/nem_transport2.c
./nem_transport/nem_transport3.c
./nem_partition/nem_partition5.c
./nem_partition/nem_partition1.c
./nem_partition/nem_partition4.c
./nem_partition/nem_partition3.c
./nem_partition/nem_partition2.c
./nem_reconstruct/nem_reconstruct_mesh/nem_reconstruct_mesh1.c
./nem_reconstruct/nem_reconstruct_mesh/nem_reconstruct_mesh2.c
./nem_reconstruct/nem_reconstruct_tesr/nem_reconstruct_tesr1.c
./nem_reconstruct/nem_reconstruct_tesr/nem_reconstruct_tesr2.c
./nem_writemesh/nem_writemesh1.c
./nem_nsets/nem_nsets1.c
./nem_nsets/nem_nsets2.c
./nem_elsets/nem_elsets1.c
./nem_elsets/nem_elsets2.c
./nem_meshing_str/nem_meshing_str1.c
./nem_meshing_str/nem_meshing_str2.c
./neper_m.c
)

set (EXTRA_LIBS ${EXTRA_LIBS} ut)
set (EXTRA_LIBS ${EXTRA_LIBS} neut)

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

if (HAVE_LIBSCOTCH)
  find_package (LIBSCOTCH REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories (${Scotch_INCLUDE_DIRS})
endif ()

if (HAVE_PTHREAD)
  find_package (Threads REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT})
endif ()

if (HAVE_NLOPT)
  find_package (NLOPT REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${NLOPT_LIBRARIES})
  include_directories (${NLOPT_INCLUDE_DIRS})
endif ()

if (HAVE_OPENMP)
  find_package (OpenMP REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${OpenMP_C_FLAGS})
  add_definitions(${OpenMP_C_FLAGS})
endif ()

target_link_libraries (neper_m ${EXTRA_LIBS})
