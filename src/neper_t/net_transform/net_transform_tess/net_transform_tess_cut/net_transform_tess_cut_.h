/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"neut_t.h"

#include"net_tess/net_tess3d/net_tess3d_domain/net_tess3d_domain.h"
#include"net_reg/net_reg_merge/net_reg_merge_del/net_reg_merge_del_ff/net_reg_merge_del_ff.h"

#include"net_transform_tess_cut.h"

extern void net_transform_tess_cut_pre (struct TESS Tess, char *expr, struct TESS *pTessGen,
                                        struct PRIM **pPrim, int *pPrimQty);

extern void net_transform_tess_cut_seedset_interior (struct PRIM *Prim, int PrimQty,
    struct TESS Tess, struct SEEDSET *pSSet, int **pintseed_oldseed, int *pintseedqty);

extern void net_transform_tess_cut_seedset_mirror (struct PRIM *Prim, int PrimQty,
                                    struct TESS Tess, struct SEEDSET *pSSet,
                                    int **pcutcells, int *pcutcellqty,
                                    int **pmirrorseeds, int **pmirrorseedprims,
                                    int *pmirrorseedqty);

extern void net_transform_tess_cut_tess (struct TESS Dom, struct SEEDSET SSet,
                                         struct TESS TessGen, struct TESS *pTess);

extern void net_transform_tess_cut_clean (struct PRIM *Prim, int PrimQty,
                                          int *cutcells, int cutcellqty, int
                                          *mirrorseeds, int *mirrorseedprims,
                                          int mirrorseedqty, struct TESS
                                          *pTess);

extern void net_transform_tess_cut_post (struct TESS TessGen,
                                         int *intseed_oldseed, int intseedqty,
                                         struct TESS *pTess);

extern void net_transform_tess_cut_clean_remove (struct PRIM Prim, int *seeds,
                                                 int seedqty, int *cutcells,
                                                 int cutcellqty, int *pnewdomface,
                                                 struct TESS *pTess);

extern void net_transform_tess_cut_clean_proj (int *newdomfaces,
                                               int newdomfaceqty,
                                               struct TESS *pTess);

extern void net_transform_tess_cut_clean_faceequpdate (int *newdomfaces,
                                                       int newdomfaceqty,
                                                       struct TESS *pTess);

extern int net_transform_tess_cut_seedset_interior_seed (struct TESS Tess, int cell,
                                                          struct PRIM Prim);

extern void net_transform_tess_cut_post_update_domain (struct TESS *pTess);
