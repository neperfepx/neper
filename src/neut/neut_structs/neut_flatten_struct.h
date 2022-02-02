/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTFLATTEN_H
#define STRUCTFLATTEN_H

  struct FLATTEN
  {
    // Two tessellations will be refered to in the following:
    // dtess: domain tessellation, in which flattening is done
    // ftess: result tessellation (output of flattening)

    int DVerQty;		// number of vertices of dtess
    int DEdgeQty;		// number of edges    of dtess
    int DFaceQty;		// number of faces    of dtess
    int DPolyQty;		// number of faces    of dtess

    int VerQty;			// number of vertices of ftess
    int **VerDom;		// [0,1] = type and nb of the entity of dtess the ver belongs to

    int EdgeQty;		// number of edges of ftess
    int **EdgeDom;		// [0,1] = type and nb of the entity of dtess the edge belongs to

    int FaceQty;		// number of faces of ftess
    int **FaceDom;		// [0,1] = type and nb of the entity of dtess the face belongs to

    int PolyQty;		// number of polys of ftess
    int **PolyDom;		// [0,1] = type and nb of the entity of dtess the poly belongs to

    int *DomVerTessVerNb;
    int *DomEdgeBTessVerQty;
    int **DomEdgeBTessVerNb;
    int *DomEdgeTessVerQty;
    int **DomEdgeTessVerNb;
    int *DomFaceBTessVerQty;
    int **DomFaceBTessVerNb;
    int *DomFaceTessVerQty;
    int **DomFaceTessVerNb;
    int *DomPolyBTessVerQty;
    int **DomPolyBTessVerNb;

    int *DomEdgeTessEdgeQty;
    int **DomEdgeTessEdgeNb;
    int *DomFaceBTessEdgeQty;
    int **DomFaceBTessEdgeNb;
    int *DomFaceTessEdgeQty;
    int **DomFaceTessEdgeNb;
    int *DomPolyBTessEdgeQty;
    int **DomPolyBTessEdgeNb;

    int *DomFaceTessFaceQty;
    int **DomFaceTessFaceNb;
    int *DomPolyBTessFaceQty;
    int **DomPolyBTessFaceNb;

    int *VerTessVerQty;
    int ***VerTessVerNb;
    int *EdgeTessEdgeQty;
    int ***EdgeTessEdgeNb;
  };
  typedef struct FLATTEN FLATTEN;

#endif				/* STRUCTFLATTEN_H */

#ifdef __cplusplus
}
#endif
