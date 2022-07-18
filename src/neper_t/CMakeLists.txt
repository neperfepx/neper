# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 3.0)
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(neper_t)

# for nanoflann #######
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

add_library(neper_t
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
./net_group/net_group1.c
./net_group/net_group_tess/net_group_tess1.c
./net_group/net_group_tesr/net_group_tesr1.c
./net_transform/net_transform1.c
./net_transform/net_transform_tess/net_transform_tess1.c
./net_transform/net_transform_tess/net_transform_tess2.c
./net_transform/net_transform_tess/net_transform_tess_cut/net_transform_tess_cut1.c
./net_transform/net_transform_tess/net_transform_tess_cut/net_transform_tess_cut2.c
./net_transform/net_transform_tess/net_transform_tess_cut/net_transform_tess_cut3.c
./net_transform/net_transform_tesr/net_transform_tesr1.c
./net_transform/net_transform_tesr/net_transform_tesr2.c
./net_transform/net_transform_tesr/net_transform_tesr_tesrmerge/net_transform_tesr_tesrmerge1.c
./net_transform/net_transform_tesr/net_transform_tesr_tesrmerge/net_transform_tesr_tesrmerge2.c
./net_transform/net_transform_tesr/net_transform_tesr_tesrmerge/net_transform_tesr_tesrmerge3.c
./net_transform/net_transform_tesr/net_transform_tesr_tesrmerge/net_transform_tesr_tesrmerge4.c
./net_transform/net_transform_tesr/net_transform_tesr_tesrtranslate/net_transform_tesr_tesrtranslate1.c
./net_transform/net_transform_tesr/net_transform_tesr_tesrtranslate/net_transform_tesr_tesrtranslate2.c
./net_transform/net_transform_tesr/net_transform_tesr_tesrtranslate/net_transform_tesr_tesrtranslate_bounds/net_transform_tesr_tesrtranslate_bounds1.c
./net_sort/net_sort1.c
./net_sort/net_sort2.c
./net_utils/net_utils1.c
./net_utils/net_utils2.c
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
./net_tess/net_tess_tocta/net_tess_tocta1.c
./net_tess/net_tess_tocta/net_tess_tocta2.c
./net_tess/net_tess_tocta//net_tess_tocta_seed/net_tess_tocta_seed1.c
./net_tess/net_tess_tocta//net_tess_tocta_seed/net_tess_tocta_seed2.c
./net_tess/net_tess_tocta//net_tess_tocta_seed/net_tess_tocta_seed3.c
./net_tess/net_tess_file/net_tess_file1.c
./net_tess/net_tess_file/net_tess_file2.c
./net_tess/net_tess_file/net_tess_file3.c
./net_tess/net_tess_opt/net_tess_opt1.cpp
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init1.cpp
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init2.cpp
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init3.cpp
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_tesrobj/net_tess_opt_init_tesrobj1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_tesrobj/net_tess_opt_init_tesrobj2.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_tesrobj/net_tess_opt_init_tesrobj3.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_pre/net_tess_opt_init_sset_pre1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_pre/net_tess_opt_init_sset_pre2.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_post/net_tess_opt_init_sset_post1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_post/net_tess_opt_init_sset_post2.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_weight/net_tess_opt_init_sset_weight1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_weight/net_tess_opt_init_sset_weight2.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_multiseed/net_tess_opt_init_sset_multiseed1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_coo/net_tess_opt_init_sset_coo1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_coo/net_tess_opt_init_sset_coo2.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_coo/net_tess_opt_init_sset_coo3.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp3.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp4.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_x/net_tess_opt_comp_x1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_x/net_tess_opt_comp_x2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_nlopt_init/net_tess_opt_comp_nlopt_init1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_nlopt_init/net_tess_opt_comp_nlopt_init2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective2.cpp
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval1.cpp
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_init/net_tess_opt_comp_objective_fval_init1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_tesr/net_tess_opt_comp_objective_fval_tesr1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_tesr/net_tess_opt_comp_objective_fval_tesr2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_cellpenalty/net_tess_opt_comp_objective_fval_cellpenalty1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_cellpenalty/net_tess_opt_comp_objective_fval_cellpenalty1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_gen/net_tess_opt_comp_objective_fval_gen1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_gen/net_tess_opt_comp_objective_fval_gen2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_comp/net_tess_opt_comp_objective_fval_comp1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_gen/net_tess_opt_comp_objective_fval_gen_stat/net_tess_opt_comp_objective_fval_gen_stat1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_gen/net_tess_opt_comp_objective_fval_gen_stat/net_tess_opt_comp_objective_fval_gen_stat2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_gen/net_tess_opt_comp_objective_fval_gen_stat/net_tess_opt_comp_objective_fval_gen_stat3.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_gen/net_tess_opt_comp_objective_fval_gen_celldata/net_tess_opt_comp_objective_fval_gen_celldata1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_fval/net_tess_opt_comp_objective_fval_gen/net_tess_opt_comp_objective_fval_gen_celldata/net_tess_opt_comp_objective_fval_gen_celldata2.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_log/net_tess_opt_comp_objective_log1.c
./net_tess/net_tess_opt/net_tess_opt_comp/net_tess_opt_comp_objective/net_tess_opt_comp_objective_log/net_tess_opt_comp_objective_log2.c
./net_tess/net_tess_opt/net_tess_opt_post/net_tess_opt_post1.c
./net_tess/net_tess_opt/net_tess_opt_post/net_tess_opt_post2.c
./net_tess/net_tess_opt/net_tess_opt_post/net_tess_opt_post3.c
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
./net_res/net_res2.c
./net_stat/net_stat1.c
./net_stat/net_stat2.c
./net_res/net_res_ori/net_res_ori1.c
./net_tess/net_tess_opt/net_tess_opt_init/net_tess_opt_init_sset/net_tess_opt_init_sset_ori/net_tess_opt_init_sset_ori1.c
./net_ori/net_ori1.c
./net_ori/net_ori2.c
./net_ori/net_ori3.c
./net_ori/net_ori_uniform/net_ori_uniform1.cpp
./net_ori/net_ori_uniform/net_ori_uniform2.cpp
./net_ori/net_ori_uniform/net_ori_uniform3.cpp
./net_ori/net_ori_uniform/net_ori_uniform4.cpp
./net_ori/net_ori_uniform/net_ori_uniform5.cpp
./net_domain/net_domain1.c
./net_domain/net_domain2.c
./net_domain/net_domain3.c
./net_domain/net_domain_cube/net_domain_cube2.c
./net_domain/net_domain_cube/net_domain_cube1.c
./net_domain/net_domain_cube/net_domain_cube3.c
./net_dim/net_dim1.c
./neper_t.c
)

list (APPEND EXTRA_LIBS neut)
target_link_libraries(neper_t ${EXTRA_LIBS})
