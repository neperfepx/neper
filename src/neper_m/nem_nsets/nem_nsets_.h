/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut_m.h"
#include"../structIn_m.h"

#include"nem_nsets.h"

extern void nem_nsets_2d_tess (struct TESS, struct MESH *, struct NSET *);
extern void nem_nsets_1d_tess (struct TESS, struct MESH *, struct NSET *);
extern void nem_nsets_0d_tess (struct TESS, struct MESH *, struct NSET *);
extern void nem_nsets_2dbody_tess (struct TESS, struct NSET *);
extern void nem_nsets_1dbody_tess (struct TESS, struct NSET *);

extern void nem_nsets_1d_tess_str (struct TESS Tess, struct NSET *NSet);
extern void nem_nsets_0d_tess_str (struct TESS Tess, struct NSET *NSet);

extern void nem_nsets_1d_str (struct NSET *NSet);
extern void nem_nsets_0d_str (struct NSET *NSet);
extern void nem_nsets_2dbody_str (struct NSET *NSet);
extern void nem_nsets_1dbody_str (struct NSET *NSet);

extern void nem_nset_addduplicates (struct NSET *pNSet, struct NODES
				    Nodes);
