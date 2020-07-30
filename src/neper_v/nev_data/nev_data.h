/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_data (char **, int *, struct SIM Sim,
                      struct TESS *pTess, struct TESR *pTesr,
                      struct NODES *, struct MESH **,
                      struct POINT *,
		      struct DATA *, struct DATA *,
		      struct DATA *, struct DATA *,
		      struct DATA *, struct DATA *);

#include "nev_data_tess/nev_data_tess.h"
#include "nev_data_tesr/nev_data_tesr.h"
#include "nev_data_nodes/nev_data_nodes.h"
#include "nev_data_csys/nev_data_csys.h"
#include "nev_data_mesh/nev_data_mesh.h"
#include "nev_data_points/nev_data_points.h"
