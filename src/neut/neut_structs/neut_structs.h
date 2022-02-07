/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_structs.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_STRUCTS_H
#define NEUT_STRUCTS_H

#include "neut_mesh_struct.h"
#include "neut_nodes_struct.h"
#include "neut_odf_struct.h" // nodes and mesh must be defined before
#include "neut_tesl_struct.h"
#include "neut_crys_struct.h"
#include "neut_reg_struct.h"
#include "neut_lloyd_struct.h"
#include "neut_tess_struct.h"
#include "neut_tesse_struct.h"
#include "neut_mtess_struct.h"
#include "neut_flatten_struct.h"
#include "neut_tesr_struct.h"
#include "neut_part_struct.h"
#include "neut_nset_struct.h"
#include "neut_meshpara_struct.h"
#include "neut_print_struct.h"
#include "neut_multim_struct.h"
#include "neut_seedset_struct.h"
#include "neut_data_struct.h"
#include "neut_poly_struct.h"
#include "neut_polymod_struct.h"
#include "neut_point_struct.h"
#include "neut_test_struct.h"
#include "neut_boundary_struct.h"

#include "neut_topt_struct.h"
#ifdef HAVE_NLOPT
#include "neut_sopt_struct.h"
#endif

#include "neut_tdyn_struct.h"
#include "neut_oopt_struct.h"

#include "neut_prim_struct.h"

#include "neut_fepxsim_struct.h"
#include "neut_sim_struct.h"
#include "neut_simres_struct.h"

#endif				/* NEUT_STRUCTS_H */

#ifdef __cplusplus
}
#endif
