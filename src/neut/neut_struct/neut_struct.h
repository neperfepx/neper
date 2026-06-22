/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_struct.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_STRUCTS_H
#define NEUT_STRUCTS_H

#include "neut_struct_mesh.h"
#include "neut_struct_nodes.h"
#include "neut_struct_ospace.h" // nodes and mesh must be defined before
#include "neut_struct_odf.h" // ospace must be defined before
#include "neut_struct_tesl.h"
#include "neut_struct_crys.h"
#include "neut_struct_reg.h"
#include "neut_struct_lloyd.h"
#include "neut_struct_tess.h"
#include "neut_struct_tesse.h"
#include "neut_struct_mtess.h"
#include "neut_struct_flatten.h"
#include "neut_struct_tesr.h"
#include "neut_struct_part.h"
#include "neut_struct_nset.h"
#include "neut_struct_meshpara.h"
#include "neut_struct_print.h"
#include "neut_struct_multim.h"
#include "neut_struct_seedset.h"
#include "neut_struct_data.h"
#include "neut_struct_datainput.h"
#include "neut_struct_poly.h"
#include "neut_struct_polymod.h"
#include "neut_struct_point.h"
#include "neut_struct_test.h"
#include "neut_struct_boundary.h"

#include "neut_struct_topt.h"
#ifdef HAVE_NLOPT
#include "neut_struct_sopt.h"
#endif

#include "neut_struct_tdyn.h"
#include "neut_struct_oopt.h"

#include "neut_struct_prim.h"

#include "neut_struct_fepxsim.h"
#include "neut_struct_sim.h"
#include "neut_struct_simres.h"

#include "neut_struct_pf.h"
#include "neut_struct_ori.h"

#include "neut_struct_in_t.h"
#include "neut_struct_in_m.h"
#include "neut_struct_in_v.h"
#include "neut_struct_in_s.h"

#endif				/* NEUT_STRUCTS_H */

#ifdef __cplusplus
}
#endif
