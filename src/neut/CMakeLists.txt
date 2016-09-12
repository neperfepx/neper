# This file is part of the Neper software package.
# Copyright (C) 2003-2016, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 2.8)
project(neut)

# should we use the optional libraries?
option (HAVE_GSL       "Use the GNU Scientific Library (GSL)" ON)
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)
option (HAVE_NLOPT       "Use NLopt"                            ON )
option (DEVEL_OPTIMIZATION "Enable optimization (ON for production use)" ON)
option (DEVEL_DEBUGGING_TEST "Enable debugging tests (OFF for production use)"   OFF)
option (DEVEL_DEBUGGING_FLAG "Enable debugging compilation flag (OFF for production use)"   OFF)
option (DEVEL_PROFILING   "Enable profiling (OFF for production use)" OFF)

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W)
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

add_library (neut 
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
./neut_tessdata/neut_tessdata.c
./neut_tesrdata/neut_tesrdata.c
./neut_meshdata/neut_meshdata.c
./neut_nodedata/neut_nodedata.c
./neut_pointdata/neut_pointdata.c
./neut_csysdata/neut_csysdata.c
./neut_part/neut_part.c
./neut_multim/neut_multim.c
./neut_elt/neut_elt.c
./neut_nodes/neut_nodes.c
./neut_reg/neut_reg.c
./neut_lloyd/neut_lloyd.c
./neut_tdyn/neut_tdyn_op/neut_tdyn_op1.c
./neut_tdyn/neut_tdyn_fprintf/neut_tdyn_fprintf1.c
./neut_boundary/neut_boundary.c
./neut_meshpara/neut_meshpara.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_fepx/neut_mesh_fscanf_fepx1.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_geof/neut_mesh_fscanf_geof1.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_gmsh/neut_mesh_fscanf_gmsh1.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_gmsh/neut_mesh_fscanf_gmsh2.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_gmsh/neut_mesh_fscanf_gmsh3.c
./neut_mesh/neut_mesh_fscanf/neut_mesh_fscanf_gmsh/neut_mesh_fscanf_gmsh4.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_gmsh/neut_mesh_fprintf_gmsh1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_per/neut_mesh_fprintf_per1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_vtk/neut_mesh_fprintf_vtk1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_fepx/neut_mesh_fprintf_fepx1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_fepx/neut_mesh_fprintf_fepx2.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_fepx/neut_mesh_fprintf_fepx3.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_geof/neut_mesh_fprintf_geof1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_geof/neut_mesh_fprintf_geof2.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_geof/neut_mesh_fprintf_geof3.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_inp/neut_mesh_fprintf_inp1.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_inp/neut_mesh_fprintf_inp2.c
./neut_mesh/neut_mesh_fprintf/neut_mesh_fprintf_scotch/neut_mesh_fprintf_scotch1.c
./neut_mesh/neut_mesh_op/neut_mesh_op.c
./neut_mesh/neut_mesh_geom/neut_mesh_geom.c
./neut_mesh/neut_mesh_topo/neut_mesh_topo.c
./neut_mesh/neut_mesh_test/neut_mesh_test.c
./neut_mesh/neut_mesh_gen/neut_mesh_gen.c
./neut_mesh/neut_mesh_part/neut_mesh_part.c
./neut_mesh/neut_mesh_str/neut_mesh_str.c
./neut_gmsh/neut_gmsh1.c
./neut_debug/neut_debug.c
./neut_seedset/neut_seedset1.c
./neut_seedset/neut_seedset2.c
./neut_povray/neut_povray.c
./neut_nset/neut_nset.c
./neut_print/neut_print.c
./neut_test/neut_test.c
./neut_point/neut_point.c
./neut_point/neut_point_fscanf/neut_point_fscanf1.c
./neut_tess/neut_tess_merge/neut_tess_merge1.c
./neut_tess/neut_tess_merge/neut_tess_merge2.c
./neut_tess/neut_tess_merge/neut_tess_merge_edges/neut_tess_merge_edges1.c
./neut_tess/neut_tess_merge/neut_tess_merge_faces/neut_tess_merge_faces1.c
./neut_tess/neut_tess_merge/neut_tess_merge_faces/neut_tess_merge_faces2.c
./neut_tess/neut_tess_merge/neut_tess_merge_polys/neut_tess_merge_polys1.c
./neut_tess/neut_tess_merge/neut_tess_merge_polys/neut_tess_merge_polys2.c
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
./neut_tess/neut_tess_fscanf/neut_tess_fscanf_1p9/neut_tess_fscanf2_1p9.c
./neut_tess/neut_tess_fscanf/neut_tess_fscanf_1p9/neut_tess_fscanf1_1p9.c
./neut_tess/neut_tess_fscanf/neut_tess_fscanf_1p10/neut_tess_fscanf2_1p10.c
./neut_tess/neut_tess_fscanf/neut_tess_fscanf_1p10/neut_tess_fscanf1_1p10.c
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
./neut_mtess/neut_mtess_fprintf/neut_mtess_fprintf1.c
./neut_mtess/neut_mtess_fprintf/neut_mtess_fprintf2.c
./neut_mtess/neut_mtess_fscanf/neut_mtess_fscanf1.c
./neut_mtess/neut_mtess_fscanf/neut_mtess_fscanf2.c
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
./neut_tesr/neut_tesr_gen/neut_tesr_gen1.c
./neut_topt/neut_topt.c
./neut_sopt/neut_sopt.c
)

## Link libraries

set (EXTRA_LIBS ${EXTRA_LIBS} ut)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake
			${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

if (HAVE_GSL)
  find_package(GSL REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
  include_directories(${GSL_INCLUDE_DIRS})
endif (HAVE_GSL)

if (HAVE_LIBSCOTCH)
  find_package (LIBSCOTCH REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories (${Scotch_INCLUDE_DIRS})
endif ()

if (HAVE_NLOPT)
  find_package (NLOPT REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${NLOPT_LIBRARIES})
  include_directories (${NLOPT_INCLUDE_DIRS})
endif ()

target_link_libraries (neut ${EXTRA_LIBS})
