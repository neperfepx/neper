/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_print (char **, int *, struct PRINT,
		       struct TESS, struct TESSDATA,
		       struct TESR, struct TESRDATA,
		       struct NODES, struct MESH *,
		       struct NODEDATA, struct MESHDATA *,
		       int, struct NODES *, struct MESH *,
		       struct NODEDATA *, struct MESHDATA **,
		       int **SElt2dElt3d,
		       struct CSYSDATA, struct POINT, struct POINTDATA);

#include "nev_print_foot/nev_print_foot.h"
#include "nev_print_header/nev_print_header.h"
#include "nev_print_csys/nev_print_csys.h"
#include "nev_print_mesh/nev_print_mesh.h"
#include "nev_print_tess/nev_print_tess.h"
#include "nev_print_tesr/nev_print_tesr.h"
#include "nev_print_utils/nev_print_utils.h"
