# This file is part of the Neper software package.
# Copyright (C) 2003-2017, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 2.8)
project(neper_t)

option (HAVE_GSL         "Use the GNU Scientific Library (GSL)" ON )
option (HAVE_NLOPT       "Use NLopt"                            ON )
option (HAVE_LIBSCOTCH   "Use libscotch"                        OFF)
option (DEVEL_OPTIMIZATION "Enable optimization (ON for production use)" ON)
option (DEVEL_DEBUGGING_TEST "Enable debugging tests (OFF for production use)"   OFF)
option (DEVEL_DEBUGGING_FLAG "Enable debugging compilation flag (OFF for production use)"   OFF)
option (DEVEL_PROFILING   "Enable profiling (OFF for production use)" OFF)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_BINARY_DIR}/contrib/ann")
include_directories("${PROJECT_SOURCE_DIR}/contrib/ann")

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

add_library (neper_t
./net_reg/net_reg_merge/net_reg_merge2.c
./net_reg/net_reg_merge/net_reg_merge1.c
./net_reg/net_reg_merge/net_reg_merge_del/net_reg_merge_del1.c
./net_reg/net_reg_merge/net_reg_merge_del/net_reg_merge_del2.c
./net_reg/net_reg_merge/net_reg_merge_del/net_reg_merge_del_ff/net_reg_merge_del_ff2.c
./net_reg/net_reg_merge/net_reg_merge_del/net_reg_merge_del_ff/net_reg_merge_del_ff1.c
./net_reg/net_reg_merge/net_reg_merge_del/net_reg_merge_del3.c
./net_reg/net_reg_merge/net_reg_merge_del/net_reg_merge_del5.c
./net_reg/net_reg_merge/net_reg_merge_del/net_reg_merge_del4.c
./net_reg/net_reg1.c
./net_reg/net_reg2.c
./net_transform/net_transform1.c
./net_transform/net_transform2.c
./net_sort/net_sort1.c
./net_sort/net_sort2.c
./net_utils/net_utils1.c
./net_utils/net_polymod_poly/net_polymod_poly1.c
./net_utils/net_polymod_poly/net_polymod_poly2.c
./net_utils/net_polymod_poly/net_polymod_poly3.c
./net_utils/net_legacy/net_legacy.c
./net_tess/net_tess1.c
./net_tess/net_tess2.c
./net_tess/net_tess_cube/net_tess_cube1.c
./net_tess/net_tess_cube/net_tess_cube2.c
./net_tess/net_tess_lam/net_tess_lam1.c
./net_tess/net_tess_lam//net_tess_lam_seed/net_tess_lam_seed1.c
./net_tess/net_tess_lam//net_tess_lam_seed/net_tess_lam_seed2.c
./net_tess/net_tess_lam//net_tess_lam_seed/net_tess_lam_seed3.c
./net_tess/net_tess_lam//net_tess_lam_seed/net_tess_lam_seed4.c
./net_tess/net_tess_opt/net_tess_opt1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init2.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init3.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init4.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_pre/net_tess_opt_init_sset_pre1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_pre/net_tess_opt_init_sset_pre2.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_pre/net_tess_opt_init_sset_pre3.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_post/net_tess_opt_init_sset_post1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_post/net_tess_opt_init_sset_post2.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_weight/net_tess_opt_init_sset_weight1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_weight/net_tess_opt_init_sset_weight2.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_multiseed/net_tess_opt_init_sset_multiseed1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_coo/net_tess_opt_init_sset_coo1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_coo/net_tess_opt_init_sset_coo2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_x/net_tess_opt_comp_x1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_nlopt_init/net_tess_opt_comp_nlopt_init1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_nlopt_init/net_tess_opt_comp_nlopt_init2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective2.cpp
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval1.cpp
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_init/net_tess_opt_comp_objective_fval_init1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_cellval/net_tess_opt_comp_objective_fval_cellval1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_cellval/net_tess_opt_comp_objective_fval_cellval2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_comp/net_tess_opt_comp_objective_fval_comp1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_comp/net_tess_opt_comp_objective_fval_comp_stat/net_tess_opt_comp_objective_fval_comp_stat1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_comp/net_tess_opt_comp_objective_fval_comp_stat/net_tess_opt_comp_objective_fval_comp_stat2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_comp/net_tess_opt_comp_objective_fval_comp_stat/net_tess_opt_comp_objective_fval_comp_stat3.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_comp/net_tess_opt_comp_objective_fval_comp_celldata/net_tess_opt_comp_objective_fval_comp_celldata1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_log/net_tess_opt_comp_objective_log1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_log/net_tess_opt_comp_objective_log2.c
./net_tess/net_tess_opt/net_tess_opt_post/net_tess_opt_post1.c
./net_tess/net_tess_opt/net_tess_opt_post/net_tess_opt_post2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_post/net_tess_opt_comp_post1.c
./net_tess/net_tess3d/net_tess3d1.cpp
./net_tess/net_tess3d/net_tess3d2.cpp
./net_tess/net_tess3d/net_tesl/net_tesl2.c
./net_tess/net_tess3d/net_tesl/net_tesl4.c
./net_tess/net_tess3d/net_tesl/net_tesl5.c
./net_tess/net_tess3d/net_tesl/net_tesl1.c
./net_tess/net_tess3d/net_tesl/net_tesl6.c
./net_tess/net_tess3d/net_tesl/net_tesl3.c
./net_tess/net_tess3d/net_polycomp/net_polycomp1.cpp
./net_tess/net_tess3d/net_polycomp/net_polycomp2.cpp
./net_tess/net_tess3d/net_polycomp/net_polycomp3.cpp
./net_tess/net_tess3d/net_polycomp/net_polycomp_poly/net_polycomp_poly1.cpp
./net_tess/net_tess3d/net_polycomp/net_polycomp_poly/net_polycomp_poly2.cpp
./net_tess/net_tess3d/net_polycomp/net_polycomp_poly/net_polycomp_poly3.c
./net_tess/net_tess3d/net_polycomp/net_polycomp_poly/net_polycomp_poly4.c
./net_tess/net_tess3d/net_polycomp/net_polycomp_poly/net_polycomp_poly5.c
./net_tess/net_tess3d/net_polycomp/net_polycomp_poly/net_polycomp_poly6.c
./net_tess/net_tess3d/net_tess3d_domain/net_tess3d_domain1.c
./net_tess/net_tess3d/net_tess3d_periodic/net_tess3d_periodic1.c
./net_tess/net_tess3d/net_tess3d_periodic/net_tess3d_periodic2.c
./net_tess/net_tess3d/net_tess3d_periodic/net_tess3d_periodic3.c
./net_flatten/net_flatten1.c
./net_flatten/net_mtess_flatten/net_mtess_flatten1.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_gen/net_mtess_flatten_gen1.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_ver/net_mtess_flatten_ver1.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_ver/net_mtess_flatten_ver2.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_edge/net_mtess_flatten_edge1.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_edge/net_mtess_flatten_edge2.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_edge/net_mtess_flatten_edge3.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_edge/net_mtess_flatten_edge4.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_face/net_mtess_flatten_face1.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_face/net_mtess_flatten_face2.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_face/net_mtess_flatten_face3.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_face/net_mtess_flatten_face4.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_poly/net_mtess_flatten_poly1.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_poly/net_mtess_flatten_poly2.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_cell/net_mtess_flatten_cell1.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_dom/net_mtess_flatten_dom1.c
./net_flatten/net_mtess_flatten/net_mtess_flatten_dom/net_mtess_flatten_dom2.c
./net_input/net_input1.c
./net_input/net_input2.c
./net_input/net_input3.c
./net_res/net_res1.c
./net_stat/net_stat1.c
./net_stat/net_stat2.c
./net_res/net_res_ori/net_res_ori1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_ori/net_tess_opt_init_sset_ori1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_ori/net_tess_opt_init_sset_ori2.c
./net_domain/net_domain1.c
./net_domain/net_domain2.c
./net_domain/net_domain_cube/net_domain_cube2.c
./net_domain/net_domain_cube/net_domain_cube1.c
./net_domain/net_domain_cube/net_domain_cube3.c
./net_dim/net_dim1.c
./neper_t.c
)

set (EXTRA_LIBS ${EXTRA_LIBS} ut)
set (EXTRA_LIBS ${EXTRA_LIBS} neut)
set (EXTRA_LIBS ${EXTRA_LIBS} orilib)
set (EXTRA_LIBS ${EXTRA_LIBS} ann)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library (MATHLIB m)
if (MATHLIB)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif ()

if (HAVE_NLOPT)
  find_package (NLOPT REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${NLOPT_LIBRARIES})
  include_directories (${NLOPT_INCLUDE_DIRS})
endif ()

if (HAVE_GSL)
  find_package(GSL REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
  include_directories(${GSL_INCLUDE_DIRS})
endif ()

if (HAVE_LIBSCOTCH)
  find_package (LIBSCOTCH REQUIRED)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories (${Scotch_INCLUDE_DIRS})
endif ()

target_link_libraries (neper_t ${EXTRA_LIBS})
