/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTPOLY_H
#define STRUCTPOLY_H

/* DEFINITION OF STRUCTURE POLY
 * It contains the exhaustive description of one polyhedron.
 * POLY is a compacted form of structure POLYMOD.
 */

  struct POLY
  {
    // Dimension the poly aims to have (-1 by default, can be 2)
    int PseudoDim;

    // If PseudoDim == 2, size in the 3rd direction
    double PseudoSize;

    /* All entities of POLY structure are the same than those of POLYMOD
     * structure. VerUse and FaceUse do not exist in POLY.
     */

    /* GENERAL INFORMATION ***************************************************
     */
    int VerQty;			/* Polyhedron quantities of vertex and face */
    int FaceQty;

    /* VERTEX INFORMATION ****************************************************
     */
    /* For vertex i (i=1...VerQty): */
    int **VerFace;		/* VerFace[i][j] (j=0...2) are the numbers of the 3 */
    /* parent faces  */
    double **VerCoo;		/* VerCoo[i][j] (j=0...2) are the 3 coordinates. */

    /* FACE INFORMATION ******************************************************
     */
    /* For face i (i=1...FaceQty): */
    int *FacePoly;		/* FacePoly[i] is the number of the 2nd parent seed */
    /* (other than the polyhedron center). */
    double **FaceEq;		/* FaceEq[i][j] (j=0...3) are the four equation */
    /* parameters: */
    /* FaceEq[i][1]*X1+FaceEq[i][2]*X2+FaceEq[i][3]*X3=FaceEq[i][0] */
    int *FaceVerQty;		/* FaceVerQty[i] is the quantity of face vertices */
    int **FaceVerNb;		/* FaceVerNb[i][j] (j=1...FaceVerQty[i]) are the */
    /* numbers of the vertices. */
  };
  typedef struct POLY POLY;

#endif				/* STRUCTPOLY_H */

#ifdef __cplusplus
}
#endif
