/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTSOPT_H
#define STRUCTSOPT_H

struct SOPT
{
  int CellQty;			// Number of cells

  int* CellSCellQtyMin;   	// minimum number of subcells of a cell [1..]
  int* CellSCellQtyMax;   	// maximum number of subcells of a cell [1...]
  int* CellSCellQty;   		// effective number of subcells of a cell [1...]

  double ***CellSSeedCoo; 	// coo of the subcells of a cell [cell][scell][0..2]
  double  **CellSSeedWei; 	// weight of the subcells of a cell [cell][scell]
  double  *CellSCellOptiThr;  	// overlap. threshold C [0,1] used for sset optimization [cell]
  struct POINT SCellOptiPts; 	// list of (raster) points used for sset optimization
  int SCellOptiCellId;       	// id of the cell of the sset which is currently optimized
};
typedef struct SOPT SOPT;

#endif /* STRUCTSOPT_H */
