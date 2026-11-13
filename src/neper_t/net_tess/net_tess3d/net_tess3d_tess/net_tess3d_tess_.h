/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_t.h"

#include"net_tess3d_tess.h"

extern void net_tess3d_tess_general (struct SEEDSET SSet, int level, int tessid,
                                     struct TESS *pTess);
extern void net_tess3d_tess_txnb (struct POLY *Poly, int **TFNb, int **TVNb, struct TESS *pTess);
extern void net_tess3d_tess_face (struct POLY *Poly, int **TFNb, int **TVNb, struct TESS *pTess);
extern void net_tess3d_tess_ver (struct POLY *Poly, int **TVNb, struct TESS *pTess,
                                 int ***pVerSeed);
extern void net_tess3d_tess_poly (struct POLY *Poly, int **TVNb, int **TFNb, struct TESS *pTess);
extern void net_tess3d_tess_edge (int **VerSeed, struct TESS *pTess);
extern void net_tess3d_tess_seed (struct SEEDSET SSet, struct TESS *pTess);

extern int net_tess3d_tess_txnb_tfnb (struct POLY *Poly, int PolyQty, int **TFNb);
extern int net_tess3d_tess_txnb_tvnb (struct POLY *Poly, int PolyQty, int **TVNb);
extern int net_tess3d_tess_face_eq (struct POLY *Poly, int i, int j, double *eq);
extern int net_tess3d_tess_poly_faceori (struct TESS *pTess, int poly, int i, double *centre);
extern int net_tess3d_tess_edge_commonseeds (int **VerSeed, int first, int second, int *nb);
extern void net_tess3d_tess_edge_faceedgeori (struct TESS *pTess);
extern void net_tess3d_tess_edge_edgeface (struct TESS *pTess);

extern int net_tess3d_tess_txnb_tvnb_treat (struct POLY *Poly, int PNb, int VNb);
extern void net_tess3d_tess_txnb_tvnb_search (struct POLY *Poly, int PNb, int VNb, int **TVNb);

extern int net_tess3d_tess_txnb_tvnb_search_prevpoly (struct POLY *Poly, int PNb, int VNb);
extern int net_tess3d_tess_txnb_tvnb_search_prevver (struct POLY *Poly, int PNb, int VNb,
                                                     int PrevP);

extern void net_tess3d_tess_txnb_tvnb_search_prevver_parfacepoly (struct POLY *Poly, int PNb,
                                                                  int VNb, int PrevP, int *PFG);
extern int net_tess3d_tess_txnb_tvnb_search_prevver_search (struct POLY *Poly, int PNb, int PrevP,
                                                            int *PFG);
