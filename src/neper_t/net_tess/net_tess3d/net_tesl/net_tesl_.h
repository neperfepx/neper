/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_t.h"

extern void IniTxNb (struct TESL, struct POLY *, int **, int **);
extern void IniTFNb (struct TESL, struct POLY *, int **);
extern void IniTVNb (struct TESL, struct POLY *, int **);
extern void TessAlloc (struct TESL *);
extern void IniTessFaceOr (struct TESL *, struct POLY *, int **);
extern void IniTessFaceVerQty (struct TESL *, struct POLY *, int **);
extern void IniTessFaceEq (struct TESL *, struct POLY *, int **);
extern void IniTessFacePoly (struct TESL *, struct POLY *, int **);
extern void IniTessFaceBound (struct TESL *, struct POLY *, int **, int **);
extern void IniTessVer (struct TESL *, struct POLY *, int **);
extern void IniTessVerSeed (struct TESL *, struct POLY *, int **);
extern void IniTessVerCoo (struct TESL *, struct POLY *, int **);
extern void IniTessPoly (struct TESL *, struct POLY *, int **, int **);
extern void IniTessPolyCenNXQty (struct SEEDSET, struct TESL *,
				 struct POLY *);
extern void IniTessPolyXNb (struct TESL *, struct POLY *, int **, int **);
extern int IniPolyFaceOri (struct TESL *, int, int);
extern void IniTessFaceEdgeOri (struct TESL *);
extern int TreatVer (struct POLY *, int, int);
extern int AddVer (int **, int, int, int);
extern int PrevPoly (struct POLY *, int, int);
extern int FindVer (struct POLY *, int, int, int, int **);
extern int PrevVer (struct POLY *, int, int, int);
extern void ParFacePoly (struct POLY *, int, int, int, int *);
extern int SearchInPrevP (struct POLY *, int, int *);
extern void SearchVer (struct POLY *, int, int, int **);
extern void UpdTVNb (int **, int, int, int, int);
extern int IniTessEdge (struct TESL *);
extern void IniTessVernEdge (struct TESL *);
extern int CommonSeeds (struct TESL, int, int, int *);
extern int BissFace (struct TESL, int, int);
extern int TreatFaceVer (struct TESL *, int, int, int);

extern void TessGenDet (struct SEEDSET, struct POLY *, struct TESL *);
extern int TFaceQty (struct SEEDSET, struct POLY *);
extern int TVerQty (struct SEEDSET, struct POLY *);
extern int TEdgeQty (struct SEEDSET, struct POLY *);
extern int TPolyQty (struct SEEDSET);
extern void IniTessPolyEdge (struct TESL *);
extern void Error (int);

extern int AddEdge (struct TESL *, int, int, int, int, int);
extern void RecEdgeVerNb (struct TESL *, int, int, int);
extern void RecFaceEdgeNb (struct TESL *, int, int, int);
extern void SearchEdge (struct TESL *, int, int, int, int, int);
extern void TessDes (struct TESL *);
extern int ut_num_min_int (int, int);
extern int RecPolyEdges (struct TESL *, int);
extern int FaceEdges2PolyEdges (struct TESL *, int, int, int);
extern int PrevFace (struct POLY *, int, int);
