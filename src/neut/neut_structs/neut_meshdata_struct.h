/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTMESHDATA_H
#define STRUCTMESHDATA_H

  struct MESHDATA
  {
    int EltQty;			// Number of elements
    int Dimension;		// Dimension (0, 1, 2 or 3)

    char *ColDataType;		// Type of colour data (col, ori, ...)
    double **ColData;		// Colour data
    int **Col;			// Colour
    char *ColScheme;		// ColourScheme (legend)

    char *RadDataType;		// Type of radius data
    double **RadData;		// Radius data
    double *Rad;		// Radius

    char *Scale;		// Scale
    char *ScaleTitle;		// Scale title

    int *BCol;			// Element boundary colour
    double BRad;		// Element boundary radius
  };
  typedef struct MESHDATA MESHDATA;

#endif				/* STRUCTMESHDATA_H */

#ifdef __cplusplus
}
#endif
