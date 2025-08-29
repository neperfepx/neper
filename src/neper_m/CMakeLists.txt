# This file is part of the Neper software package.
# Copyright (C) 2003-2024, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 3.0...3.10)
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(neper_m C)

add_definitions(-Wall -W -Wno-unknown-pragmas)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-copy")
endif()

# Set the build type for appropriate compile flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build: Debug Release" FORCE)
endif()

add_library(neper_m
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
./nem_meshing/nem_meshing_0d/nem_meshing_0d2.c
./nem_meshing/nem_meshing_0d/nem_meshing_0d1.c
./nem_meshing/nem_meshing_1d/nem_meshing_1d1.c
./nem_meshing/nem_meshing_1d/nem_meshing_1d2.c
./nem_meshing/nem_meshing_1d/nem_meshing_1d3.c
./nem_meshing/nem_meshing_1d/nem_meshing_1d4.c
./nem_meshing/nem_meshing_1d/nem_meshing_1d5.c
./nem_meshing/nem_meshing_1d/nem_meshing_1d_useg/nem_meshing_1d_useg1.c
./nem_meshing/nem_meshing_1d/nem_meshing_1d_useg/nem_meshing_1d_useg2.c
./nem_meshing/nem_meshing_2d/nem_meshing_2d1.c
./nem_meshing/nem_meshing_2d/nem_meshing_2d2.c
./nem_meshing/nem_meshing_2d/nem_meshing_2d3.c
./nem_meshing/nem_meshing_2d/nem_meshing_2d_face_mesh_gmsh/nem_meshing_2d_face_mesh_gmsh1.c
./nem_meshing/nem_meshing_2d/nem_meshing_2d_face_mesh_gmsh/nem_meshing_2d_face_mesh_gmsh2.c
./nem_meshing/nem_meshing_2d/nem_meshing_2d_face_mesh_gmsh/nem_meshing_2d_face_mesh_gmsh3.c
./nem_meshing/nem_meshing_3d/nem_meshing_3d1.c
./nem_meshing/nem_meshing_3d/nem_meshing_3d2.c
./nem_meshing/nem_meshing_3d/nem_meshing_3d3.c
./nem_meshing/nem_meshing_3d/nem_meshing_3d4.c
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
./nem_order/nem_order3.c
./nem_transform/nem_transform1.c
./nem_transform/nem_transform2.c
./nem_sort/nem_sort1.c
./nem_sort/nem_sort2.c
./nem_cleaning/nem_cleaning1.c
./nem_utils/nem_utils.c
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
./nem_partition/nem_partition1.c
./nem_partition/nem_partition4.c
./nem_partition/nem_partition3.c
./nem_partition/nem_partition2.c
./nem_ori/nem_ori1.c
./nem_reconstruct/nem_reconstruct_mesh/nem_reconstruct_mesh1.c
./nem_reconstruct/nem_reconstruct_mesh/nem_reconstruct_mesh2.c
./nem_reconstruct/nem_reconstruct_tesr/nem_reconstruct_tesr1.c
./nem_reconstruct/nem_reconstruct_tesr/nem_reconstruct_tesr2.c
./nem_writemesh/nem_writemesh1.c
./nem_writemesh/nem_writemesh2.c
./nem_nsets/nem_nsets1.c
./nem_nsets/nem_nsets2.c
./nem_elsets/nem_elsets1.c
./nem_meshing_str/nem_meshing_str1.c
./nem_meshing_str/nem_meshing_str2.c
./neper_m.c
)

list (APPEND EXTRA_LIBS neut)
if(HAVE_LIBSCOTCH)
  list (APPEND EXTRA_LIBS ${Scotch_LIBRARIES})
endif()
target_link_libraries(neper_m ${EXTRA_LIBS})
