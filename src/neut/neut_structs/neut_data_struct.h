/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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
    char *Entity;               // Entity: cell, voxel, ...

    char *Value; // ori, dir, ...

    double **Data;     // data itself, needed in -space pf
    char *DataType;
    char *DataName;
    int DataSize;

    // needed by: csys mesh nodes point tesr tess
    double **ColData;		// Colour data, indexed 1
    char *ColDataType;		// Type of color data (col, ori, ...), indexed 1
    char *ColDataName;          // Name of the color data, indexed 1
    int ColDataSize;            // size of the color data, indexed 1
    char *ColScheme;		// ColourScheme (legend), indexed 1
    int **Col;			// Colour, indexed 1
    int *ColDataDef;		// 1 if data defined, 0 otherwise <-- tesr, indexed 1

    // needed by: point tesr tess
    double **TrsData;		// Transparency data, indexed 1
    char *TrsDataType;		// Type of transparency data, indexed 1
    char *TrsScheme;		// Transparency scheme, indexed 1
    double *Trs;		// Transparency, indexed 1

    // needed by: csys mesh nodes point tess
    double **RadData;		// Radius data, indexed 1
    char *RadDataType;		// Type of radius data, indexed 1
    double *Rad;		// Radius, indexed 1
    char *RadDataName;          // Name of the rad data, indexed 1

    // needed by: csys mesh nodes point tess
    char **SymbolData;          // Symbol data, indexed 1
    char *SymbolDataType;	// Type of symbol data, indexed 1
    char **Symbol;		// Symbol, indexed 1
    char *SymbolDataName;       // Name of the symbol data, indexed 1

    // needed by: csys nodes point
    double **CooData;           // indexed 1
    char *CooDataType;          // indexed 1
    char *CooDataName;          // Name of the coo data
    double **Coo;               // indexed 1
    double CooFact;             // indexed 1

    // needed by: csys
    double **WeightData;        // indexed 1
    char *WeightDataType;       // indexed 1
    double *Weight;             // indexed 1

    // needed by: csys
    double **LengthData;        // indexed 1
    char *LengthDataType;       // indexed 1
    double *Length;             // indexed 1

    // needed by: mesh tesr tess
    int *BCol;			// Element boundary color
    double BRad;		// Element boundary radius

    // needed by tesr
    int *VoidCol;               // Void color

    // needed by: mesh nodes point tesr tess
    char *Scale;		// Scale
    char *ScaleTitle;		// Scale title

    // needed by: point
    double *Axes;

    // needed by: csys
    char **Label;
    double FontSize;
  };
  typedef struct DATA DATA;

#endif				/* STRUCTDATA_H */

#ifdef __cplusplus
}
#endif
