/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_data (char **, int *, struct TESS Tess,
		      struct TESR Tesr, struct MESH *,
		      struct TESSDATA *, struct TESRDATA *,
		      struct NODEDATA *, struct MESHDATA *,
		      struct CSYSDATA *, struct POINTDATA *);

extern void nev_data_init (struct TESS, struct TESSDATA *,
			   struct TESR, struct TESRDATA *,
			   struct NODES, struct MESH *, struct POINT,
			   struct NODEDATA *, struct MESHDATA *,
			   struct CSYSDATA *, struct POINTDATA *);

#include "nev_tessdata/nev_tessdata.h"
#include "nev_tesrdata/nev_tesrdata.h"
#include "nev_nodedata/nev_nodedata.h"
#include "nev_csysdata/nev_csysdata.h"
#include "nev_meshdata/nev_meshdata.h"
#include "nev_pointdata/nev_pointdata.h"
