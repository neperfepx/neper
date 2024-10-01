/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_data (struct IN_V In, struct SIM Sim,
                      struct TESS *pTess, struct TESR *pTesr,
                      struct NODES *, struct MESH **,
                      struct POINT **, int,
		      struct DATA *, struct DATA *,
		      struct DATA *, struct DATA **,
		      struct DATA *, struct DATA **);

#include "nev_print_data_tess/nev_print_data_tess.h"
#include "nev_print_data_tesr/nev_print_data_tesr.h"
#include "nev_print_data_nodes/nev_print_data_nodes.h"
#include "nev_print_data_csys/nev_print_data_csys.h"
#include "nev_print_data_mesh/nev_print_data_mesh.h"
#include "nev_print_data_points/nev_print_data_points.h"
