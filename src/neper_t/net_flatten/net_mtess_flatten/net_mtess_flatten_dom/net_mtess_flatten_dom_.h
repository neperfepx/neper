/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut_t.h"
#include"net_mtess_flatten_dom.h"

extern void net_mtess_flatten_dom_gen (struct TESS Tess, struct TESS *pFTess);
extern void net_mtess_flatten_dom_ver (struct TESS *Tess, int TessId,
				 struct TESS *pFTess, struct TESSE *TessE);
extern void net_mtess_flatten_dom_edge (struct TESS *Tess, int TessId,
				  struct TESS *pFTess, struct TESSE *TessE);
extern void net_mtess_flatten_dom_face (struct TESS *Tess, int TessId,
				  struct TESS *pFTess, struct TESSE *TessE);
