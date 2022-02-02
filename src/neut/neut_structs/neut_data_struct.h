/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTDATA_H
#define STRUCTDATA_H

  struct DATA
  {
    // needed by: mesh nodes point tesr tess
    int Qty;
    int Dim;
    char *type;                 // rename

    // needed by: csys mesh nodes point tesr tess
    double **ColData;		// Colour data
    char *ColDataType;		// Type of color data (col, ori, ...)
    char *ColDataName;          // Name of the color data
    int ColDataSize;           // size of the color data
    char *ColScheme;		// ColourScheme (legend)
    int **Col;			// Colour
    int *ColDataDef;		// 1 if data defined, 0 otherwise <-- tesr

    // needed by: point tesr tess
    double **TrsData;		// Transparency data
    char *TrsDataType;		// Type of transparency data
    char *TrsScheme;		// Transparency scheme
    double *Trs;		// Transparency

    // needed by: csys mesh nodes point tess
    double **RadData;		// Radius data
    char *RadDataType;		// Type of radius data
    double *Rad;		// Radius
    char *RadDataName;          // Name of the rad data

    // needed by: csys nodes point
    double **CooData;
    char *CooDataType;
    char *CooDataName;          // Name of the coo data
    double **Coo;
    double CooFact;

    // needed by: csys
    double **LengthData;
    char *LengthDataType;
    double *Length;

    // needed by: mesh tesr
    int *BCol;			// Element boundary color
    double BRad;		// Element boundary radius

    // needed by tesr
    int *VoidCol;               // Void color

    // needed by: mesh nodes point tesr tess
    char *Scale;		// Scale
    char *ScaleTitle;		// Scale title

    // needed by: point
    double *Axes;
    char *Space;

    // needed by: csys
    char **Label;
    double FontSize;
  };
  typedef struct DATA DATA;

#endif				/* STRUCTDATA_H */

#ifdef __cplusplus
}
#endif
