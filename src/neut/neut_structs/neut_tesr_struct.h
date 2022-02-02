/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTTESR_H
#define STRUCTTESR_H

  struct TESR
  {
    // GENERAL INFORMATION -----------------------------------------------
    double *Origin;             // origin (absolute coordinates)
    int hasvoid;                // has void voxels? 0 or 1

    int Dim;			// Dimension
    int CellQty;		// number of polyhedra
    int *CellId;		// identifiers of the cells
    double **CellOri;		// crystal orientations of the cells
    char **CellOriDistrib;      // [1...CellQty], NULL if uninitialized
    char *CellOriDes;
    int ***CellBBox;		// bbox of a cell [cell][0...2][0,1]
    double **CellCoo;		// centroids of the cells (absolute coordinates)
    double *CellVol;		// volumes/areas of the cells
    double *CellConvexity;      // Convexity of the cells

    // Group of the cells [1...CellQty]
    int *CellGroup;

    // Crystal symmetry of the cells ("triclinic", "cubic" or
    // "hexagonal").  Defined once for all cells, but this could be
    // improved
    char *CellCrySym;

    int *size;			// number of points in the 3 directions of space
    double *vsize;		// size of points in the 3 directions of space

    // SEED INFORMATION --------------------------------------------------

    double **SeedCoo;		// SeedCoo[i][j] (j=0...2) are the 3 coordinates
    // of the center.
    double *SeedWeight;		// SeedWeight[i] is the Laguerre weight

    // DATA --------------------------------------------------------------

    int ***VoxCell;		// VoxPoly[i][j][k]: cell of point at
    // position i, j, k, with i in [1...size[0]],
    // j in [1...size[1]] and k in [1...size[2]]

    double ****VoxOri;		// VoxOri[i][j][k]: ori of point at
    // position i, j, k, with i in [1...size[0]],
    // j in [1...size[1]] and k in [1...size[2]]
  };
  typedef struct TESR TESR;

#endif				/* STRUCTTESR_H */

#ifdef __cplusplus
}
#endif
