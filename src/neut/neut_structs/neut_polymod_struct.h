/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTPOLYMOD_H
#define STRUCTPOLYMOD_H

/* DEFINITION OF STRUCTURE POLYMOD
 * It contains the exhaustive description of one polyhedron during its
 * construction.
 */

  struct POLYMOD
  {
    /* All entities of POLYMOD structure are the same than those of POLY
     * structure. VerUse and FaceUse are used in POLYMOD to simplify the
     * polyhedron construction.
     */

    /* GENERAL INFORMATION ***************************************************
     */
    int VerQty;			/* Polyhedron quantities of vertex and face */
    int FaceQty;

    /* SUMMIT INFORMATION ****************************************************
     */
    /* For vertex i (i=1...VerQty): */
    int **VerFace;		/* VerFace[i][j] (j=0...2) are the numbers of the 3 */
    /* parent faces  */
    double **VerCoo;		/* VerCoo[i][j] (j=0...2) are the 3 coordinates. */
    int *VerUse;		/* VerUse[i] indicates if the vertex is useful for */
    /* the polyhedron description: 1 if useful, 0 else. */

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
    int *FaceUse;		/* FaceUse[i] indicates if the face is useful for */
    /* the polyhedron description: 1 if useful, 0 else. */
  };
  typedef struct POLYMOD POLYMOD;

#endif				/* STRUCTPOLYMOD_H */

#ifdef __cplusplus
}
#endif
