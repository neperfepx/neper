# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 3.0)
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(neut)

# for nanoflann(on some systems) =======
if(CMAKE_VERSION VERSION_LESS "3.1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else()
    set(CMAKE_CXX_STANDARD 11)
endif()
add_definitions(-Wno-deprecated-declarations)

add_definitions(-Wall -W -Wno-unknown-pragmas)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-copy")
endif()

# Set the build type for appropriate compile flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build: Debug Release" FORCE)
endif()

add_library(neut
./neut_tesl/neut_tesl.c
./neut_tesl/neut_tesl_tess/neut_tesl_tess3.c
./neut_tesl/neut_tesl_tess/neut_tesl_tess2.c
./neut_tesl/neut_tesl_tess/neut_tesl_tess1.c
./neut_poly/neut_poly_polymod/neut_poly_polymod1.c
./neut_poly/neut_poly_polymod/neut_poly_polymod2.c
./neut_poly/neut_poly_polymod/neut_poly_polymod3.c
./neut_poly/neut_poly_fprintf/neut_poly_fprintf.c
./neut_poly/neut_poly_geom/neut_poly_geom.c
./neut_poly/neut_poly_op/neut_poly_op.c
./neut_poly/neut_poly_topo/neut_poly_topo.c
./neut_polymod/neut_polymod.c
./neut_data/neut_data_fscanf/neut_data_fscanf2.c
./neut_data/neut_data_fscanf/neut_data_fscanf1.c
./neut_data/neut_data_gen/neut_data_gen1.cpp
./neut_data/neut_data_gen/neut_data_gen2.cpp
./neut_data/neut_data_op/neut_data_op.c
./neut_part/neut_part.c
./neut_multim/neut_multim.c
./neut_elt/neut_elt.c
./neut_crys/neut_crys.c
./neut_nodes/neut_nodes.c
./neut_reg/neut_reg.c
./neut_lloyd/neut_lloyd.c
./neut_tdyn/neut_tdyn_op/neut_tdyn_op1.c
./neut_tdyn/neut_tdyn_fprintf/neut_tdyn_fprintf1.c
./neut_boundary/neut_boundary1.c
./neut_boundary/neut_boundary2.c
./neut_meshpara/neut_meshpara.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_geof/neut_mesh_fscanf_geof1.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_msh/neut_mesh_fscanf_msh1.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_msh/neut_mesh_fscanf_msh2.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_msh/neut_mesh_fscanf_msh3.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_msh/neut_mesh_fscanf_msh4.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_msh/neut_mesh_fprintf_msh1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_msh/neut_mesh_fprintf_msh2.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_msh/neut_mesh_fprintf_msh3.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_ori/neut_mesh_fprintf_ori1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_phase/neut_mesh_fprintf_phase1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_bcs/neut_mesh_fprintf_bcs1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_per/neut_mesh_fprintf_per1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_vtk/neut_mesh_fprintf_vtk1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_geof/neut_mesh_fprintf_geof1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_geof/neut_mesh_fprintf_geof2.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_geof/neut_mesh_fprintf_geof3.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_inp/neut_mesh_fprintf_inp1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_inp/neut_mesh_fprintf_inp2.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_scotch/neut_mesh_fprintf_scotch1.c
./neut_mesh/neut_mesh_op/neut_mesh_op.c
./neut_mesh/neut_mesh_ori/neut_mesh_ori1.c
./neut_mesh/neut_mesh_geom/neut_mesh_geom1.cpp
./neut_mesh/neut_mesh_geom/neut_mesh_geom2.cpp
./neut_mesh/neut_mesh_topo/neut_mesh_topo.c
./neut_mesh/neut_mesh_test/neut_mesh_test.c
./neut_mesh/neut_mesh_gen/neut_mesh_gen.c
./neut_mesh/neut_mesh_slice/neut_mesh_slice.c
./neut_mesh/neut_mesh_part/neut_mesh_part.c
./neut_mesh/neut_mesh_str/neut_mesh_str.c
./neut_mesh/neut_mesh_sim/neut_mesh_sim.c
./neut_gmsh/neut_gmsh1.c
./neut_debug/neut_debug.c
./neut_seedset/neut_seedset1.cpp
./neut_seedset/neut_seedset2.cpp
./neut_povray/neut_povray.c
./neut_nset/neut_nset.c
./neut_print/neut_print.c
./neut_test/neut_test.c
./neut_point/neut_point.c
./neut_point/neut_point_fscanf/neut_point_fscanf1.c
./neut_tess/neut_tess_merge/neut_tess_merge1.c
./neut_tess/neut_tess_merge/neut_tess_merge2.c
./neut_tess/neut_tess_merge/neut_tess_edges_merge/neut_tess_edges_merge1.c
./neut_tess/neut_tess_merge/neut_tess_edges_merge/neut_tess_edges_merge2.c
./neut_tess/neut_tess_merge/neut_tess_faces_merge/neut_tess_faces_merge1.c
./neut_tess/neut_tess_merge/neut_tess_faces_merge/neut_tess_faces_merge2.c
./neut_tess/neut_tess_merge/neut_tess_polys_merge/neut_tess_polys_merge1.c
./neut_tess/neut_tess_merge/neut_tess_polys_merge/neut_tess_polys_merge2.c
./neut_tess/neut_tess_compress/neut_tess_compress1.c
./neut_tess/neut_tess_compress/neut_tess_compress2.c
./neut_tess/neut_tess_compress/neut_tess_compress3.c
./neut_tess/neut_tess_fprintf/neut_tess_fprintf3.c
./neut_tess/neut_tess_fprintf/neut_tess_fprintf2.c
./neut_tess/neut_tess_fprintf/neut_tess_fprintf1.c
./neut_tess/neut_tess_tess/neut_tess_tess3.c
./neut_tess/neut_tess_tess/neut_tess_tess2.c
./neut_tess/neut_tess_tess/neut_tess_tess1.c
./neut_tess/neut_tess_extrude/neut_tess_extrude1.c
./neut_tess/neut_tess_extrude/neut_tess_extrude2.c
./neut_tess/neut_tess_fscanf/neut_tess_fscanf3.c
./neut_tess/neut_tess_fscanf/neut_tess_fscanf2.c
./neut_tess/neut_tess_fscanf/neut_tess_fscanf1.c
./neut_tess/neut_tess_op/neut_tess_op1.c
./neut_tess/neut_tess_op/neut_tess_op2.c
./neut_tess/neut_tess_op/neut_tess_op3.c
./neut_tess/neut_tess_test/neut_tess_test1.c
./neut_tess/neut_tess_test/neut_tess_test_ver/neut_tess_test_ver1.c
./neut_tess/neut_tess_test/neut_tess_test_ver/neut_tess_test_ver2.c
./neut_tess/neut_tess_test/neut_tess_test_face/neut_tess_test_face1.c
./neut_tess/neut_tess_test/neut_tess_test_face/neut_tess_test_face2.c
./neut_tess/neut_tess_test/neut_tess_test_edge/neut_tess_test_edge1.c
./neut_tess/neut_tess_test/neut_tess_test_edge/neut_tess_test_edge2.c
./neut_tess/neut_tess_test/neut_tess_test_poly/neut_tess_test_poly2.c
./neut_tess/neut_tess_test/neut_tess_test_poly/neut_tess_test_poly1.c
./neut_tess/neut_tess_test/neut_tess_test_dom/neut_tess_test_dom1.c
./neut_tess/neut_tess_test/neut_tess_test_dom/neut_tess_test_dom2.c
./neut_tess/neut_tess_topo/neut_tess_topo.c
./neut_tess/neut_tess_geom/neut_tess_geom.c
./neut_tess/neut_tess_gen/neut_tess_gen.c
./neut_tesse/neut_tesse_op/neut_tesse_op1.c
./neut_tess/neut_tess_sim/neut_tess_sim.c
./neut_mtess/neut_mtess_op/neut_mtess_op1.c
./neut_mtess/neut_mtess_topo/neut_mtess_topo1.c
./neut_mtess/neut_mtess_gen/neut_mtess_gen1.c
./neut_tesr/neut_tesr_fscanf/neut_tesr_fscanf1.c
./neut_tesr/neut_tesr_fscanf/neut_tesr_fscanf2.c
./neut_tesr/neut_tesr_fscanf/neut_tesr_fscanf3.c
./neut_flatten/neut_flatten_op/neut_flatten_op1.c
./neut_flatten/neut_flatten_topo/neut_flatten_topo1.c
./neut_tesr/neut_tesr_fprintf/neut_tesr_fprintf1.c
./neut_tesr/neut_tesr_fprintf/neut_tesr_fprintf2.c
./neut_tesr/neut_tesr_op/neut_tesr_op1.c
./neut_tesr/neut_tesr_op/neut_tesr_op2.c
./neut_tesr/neut_tesr_op/neut_tesr_op3.c
./neut_tesr/neut_tesr_geom/neut_tesr_geom1.c
./neut_tesr/neut_tesr_geom/neut_tesr_geom2.c
./neut_tesr/neut_tesr_ori/neut_tesr_ori1.c
./neut_tesr/neut_tesr_ori/neut_tesr_ori2.c
./neut_tesr/neut_tesr_gen/neut_tesr_gen1.c
./neut_crys/neut_crys.c
./neut_topt/neut_topt.c
./neut_sopt/neut_sopt.c
./neut_oopt/neut_oopt.c
./neut_ori/neut_ori1.c
./neut_ori/neut_ori2.c
./neut_oset/neut_oset1.cpp
./neut_oset/neut_oset2.cpp
./neut_prim/neut_prim.c
./neut_sim/neut_sim_op/neut_sim_op.c
./neut_sim/neut_sim_gen/neut_sim_gen1.c
./neut_sim/neut_sim_simres/neut_sim_simres1.c
./neut_sim/neut_sim_simres/neut_sim_simres2.c
./neut_sim/neut_sim_fscanf/neut_sim_fscanf1.c
./neut_sim/neut_sim_fscanf/neut_sim_fscanf2.c
./neut_sim/neut_sim_fscanf/neut_sim_fscanf_report/neut_sim_fscanf_report1.c
./neut_sim/neut_sim_fprintf/neut_sim_fprintf1.c
./neut_sim/neut_sim_fprintf/neut_sim_fprintf2.c
./neut_fepxsim/neut_fepxsim_sim/neut_fepxsim_sim.c
./neut_fepxsim/neut_fepxsim_fscanf/neut_fepxsim_fscanf1.c
./neut_fepxsim/neut_fepxsim_op/neut_fepxsim_op.c
./neut_fepxsim/neut_fepxsim_gen/neut_fepxsim_gen.c
./neut_odf/neut_odf1.cpp
./neut_odf/neut_odf2.cpp
./neut_odf/neut_odf3.cpp
)

## Link libraries

list (APPEND EXTRA_LIBS ut)
list (APPEND EXTRA_LIBS orilib)
list (APPEND EXTRA_LIBS neper_t)
target_link_libraries(neut ${EXTRA_LIBS})
