/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \mainpage  NEUT library: the Neper utility library
///
/// The library includes functions for:
/// <ul>
/// <li>
/// <li>
/// <li>
/// <li>
/// </ul>
///
/// The library is split up into several files, which can be accessed
/// by following the Files link above.
///
/// Functions are prefixed 'neut_'.
///
/// \see This documentation is generated using doxygen.
/// <ul>
/// <li> homepage: http://www.doxygen.org
/// <li> quick start: http://www.cs.cmu.edu/~410/doc/doxygen.html
/// <li> others: https://www.icts.uiowa.edu/confluence/display/ICTSit/Doxygen+Examples
/// </ul>
/// <p/> <br/>

/// \file neut.h
/// \brief Main header file which includes all other header files.
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_H
#define NEUT_H

#include "neper_config.h"
#include "ut.h"

#include "neut_glo/neut_glo.h"
#include "neut_structs/neut_structs.h"
#include "neut_tess/neut_tess.h"
#include "neut_crys/neut_crys.h"
#include "neut_tesse/neut_tesse.h"
#include "neut_mtess/neut_mtess.h"
#include "neut_flatten/neut_flatten.h"
#include "neut_tesr/neut_tesr.h"
#include "neut_elt/neut_elt.h"
#include "neut_crys/neut_crys.h"
#include "neut_mesh/neut_mesh.h"
#include "neut_nodes/neut_nodes.h"
#include "neut_meshpara/neut_meshpara.h"
#include "neut_debug/neut_debug.h"
#include "neut_gmsh/neut_gmsh.h"
#include "neut_seedset/neut_seedset.h"
#include "neut_tesl/neut_tesl.h"
#include "neut_poly/neut_poly.h"
#include "neut_polymod/neut_polymod.h"
#include "neut_part/neut_part.h"
#include "neut_print/neut_print.h"
#include "neut_point/neut_point.h"
#include "neut_data/neut_data.h"
#include "neut_multim/neut_multim.h"
#include "neut_nset/neut_nset.h"
#include "neut_povray/neut_povray.h"
#include "neut_test/neut_test.h"
#include "neut_reg/neut_reg.h"
#include "neut_lloyd/neut_lloyd.h"
#include "neut_tdyn/neut_tdyn.h"
#include "neut_boundary/neut_boundary.h"
#include "neut_crys/neut_crys.h"
#ifdef HAVE_NLOPT
#include "neut_topt/neut_topt.h"
#endif
#include "neut_sopt/neut_sopt.h"
#include "neut_oopt/neut_oopt.h"
#include "neut_ori/neut_ori.h"
#include "neut_prim/neut_prim.h"
#include "neut_sim/neut_sim.h"

#endif				/* NEUT_H */

#ifdef __cplusplus
}
#endif
