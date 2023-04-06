/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTTESL_H
#define STRUCTTESL_H

  struct TESL
  {
    // GENERAL INFORMATION -----------------------------------------------
    char *morpho;		// morphology. ca be: poisson, cube, dodeca,
    // tocta, or custom.

    int Id;			// id of the tessellation (defining with PolyQty
    // the seed distribution)

    // VERTEX INFORMATION ------------------------------------------------

    int VerQty;			// number of vertices

    // For vertex i (i=1...VerQty):
    double **VerCoo;		// VerCoo[i][j] (j=0...2) are the 3 coordinates.

    int **VerEdge;		// VerEdge[i][j] (j=0...3) are the numbers of the
    // 4 parent edges.

    int **VerSeed;		// VerSeed[i][j] (j=0...3) are the numbers of the
    // 4 parent seeds.

    // EDGE INFORMATION --------------------------------------------------

    int EdgeQty;		// number of edges

    // For edge i (i=1...EdgeQty):
    int **EdgeVerNb;		// EdgeVerNb[i][j] (j=0,1) are the numbers of the
    // two vertices.

    double *EdgeLength;		// EdgeLength[i] is the length.

    // FACE INFORMATION --------------------------------------------------

    int FaceQty;		// number of faces

    // For face i (i=1...FaceQty):
    int **FacePoly;		// FacePoly[i][j] (j=0,1) are the numbers of the two
    // parent polys.

    double **FaceEq;		// FaceEq[i][j] (j=0...3) are the four equation
    // parameters:
    // FaceEq[i][1]*X1+FaceEq[i][2]*X2+FaceEq[i][3]*X3=FaceEq[i][0]

    int *FaceVerQty;		// FaceVerQty[i] is the quantity of face vertices
    int **FaceVerNb;		// FaceVerNb[i][j] (j=1...FaceVerQty[i]) are the
    // numbers of the vertices.

    int **FaceEdgeNb;		// FaceEdgeNb[i][j] (j=1...FaceVerQty[i]) are the
    // numbers of the edges.

    int **FaceEdgeOri;		// FaceEdgeOri[i][j] (j=1...FaceVerQty[i]) are the
    // orientations of the edges.

    // POLYHEDRON INFORMATION --------------------------------------------

    int PolyQty;		// number of polyhedra

    // For polyhedron i (i=1...PolyQty):

    double **SeedCoo;		// SeedCoo[i][j] (j=0...2) are the 3 coordinates
    // of the center.

    double *SeedWeight;		// SeedWeight[i] is the Laguerre weight

    int *PolyFaceQty;		// PolyFaceQty[i] is the quantity of faces.
    int **PolyFaceNb;		// PolyFaceNb[i][j] (j=1...PolyFaceQty[i]) are the
    // numbers of the faces.
    int **PolyFaceOri;		// PolyFaceOri[i][j] (j=1...PolyFaceQty[i]) are the
    // orientations of the faces: 1 if the face normal
    // is outgoing, -1 otherwise.

    int *PolyEdgeQty;		// PolyEdgeQty[i] is the quantity of edges.
    int **PolyEdgeNb;		// PolyEdgeNb[i][j] (j=1...PolyEdgeQty[i]) are the
    // numbers of the edges.

    int *PolyVerQty;		// PolyVerQty[i] is the quantity of vertices.
    int **PolyVerNb;		// PolyVerNb[i][j] (j=1...PolyVerQty[i]) are the
    // numbers of the vertices.

    // DOMAIN INFORMATION ------------------------------------------------

    int *VerOfDomain;		// VerOfDomain[0] = nb of ver of domain.
    // VerOfDomain[0...(VerOfDomain[0] - 1)] are the
    // number of the vertices which are vertices of
    // the cubic domain: they have only 3 parent
    // edges.
  };
  typedef struct TESL TESL;

#endif				/* STRUCTTESL_H */

#ifdef __cplusplus
}
#endif
