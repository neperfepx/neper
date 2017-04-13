/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tdyn_struct.h"

#ifndef STRUCTTOPT_H
#define STRUCTTOPT_H

struct TOPT
{
  // TARGET MICROSTRUCTURE INFORMATION ---------------------------------

  int Dim;			// dimension

  // domain and general
  int CellQty;			// number of cells
  struct TESS Dom;		// domain (1-cell tessellation)
  struct TESS DomPer;		// 3x3x3-scale domain (for periodicity)

  // specified inputs
  int tarqty;			// number of target variables
  char **tarvar;		// variable
  char **tartype;		// type of specification: stat or cell
  char **tarexpr;		// variable expressions (normal, ..., file)
  int *tarexprdisqty;		// number of distributions in the variable expression
  double **tarexprdisprop;	// proportions of the distributions
  // can point to: distribution, cell-by-cell or
  // raster information (see below)
  double *tarrefval;		// variable reference value (average cell size, ...)

  // distribution information
  struct FCT *tarpdf0;		// target PDF, not smoothed
  struct FCT *tarpdf;		// target PDF, smoothed
  struct FCT *tarcdf0;		// target CDF, not smoothed, computed
  struct FCT *tarcdf;		// target CDF, smoothed

  int         *tarmodeqty;      // number of modes of the target PDF/CDF
  double     **tarmodefact;     // factors of the modes of the target PDF/CDF
  struct FCT **tarmodecdf0;     // modes of the target CDF, not smoothed

  // cell-by-cell information
  double ***tarcellval;		// variable cell value
  int *tarcellvalqty;		// number of cell values (1 for size, etc.)

  // raster microstructure information
  struct TESR tartesr;		// raster tessellation
  double *tartesrscale;		// scaling factor of the tess (for elongated grains)
  double  ***tarcellpts;	// list of points coo of a target cell [0...]
  int    *tarcellptqty;		// number of poiint of a target cell
  double *tarcellrefval;	// ref val of a target cell
  int  ***tarcellptscells;      // sorted subcells of a point
                                // [cell][pt][0...CellSCellQty[cell]-1]

  // CURRENT MICROSTRUCTURE DEFINITION ---------------------------------

  // seeds and tessellation
  struct SEEDSET SSet;		// Seed set
  struct POLY *Poly;		// set of polys

  // distribution information
  struct FCT *curpdf;		// current PDF, smoothed
  struct FCT *curcdf0;		// current CDF, not smoothed
  struct FCT *curcdf;		// current CDF, smoothed
  double *curpenalty;        	// current penalty

  // cell-by-cell information
  double *curcellpenalty;       // current cell penalty (if void)
  double ***curcellval;		// current cell values
  double *oldcellpenalty;       // old (previous) cell penalty (if void)
  double ***oldcellval;		// old (previous) cell values
  // (number given by varcellvalqty)

  // seedset information
  int* CellSCellQty;	   	// number of subcells of a cell [1...]
  int** CellSCellList;		// ids of the subcells of a cell [1...][0...]
  int* SCellCell;		// cell of a subcell [1...]

  int seedoptiqty;		// number of seeds used for optimization
  int *seedopti;		// seeds used for optimization
  int scellchangedqty;		// current number of changed subcells
  int *scellchanged;		// current changed subcells
  int cellchangedqty;		// current number of changed cells
  int *cellchanged;		// current changed cells

  // OPTIMIZATION INFORMATION -----------------------------------------

  // optimization variables
  int xqty;        		// total number of variables
  int *x_seed;     		// seed to which the variable is related
  int *x_var;     		// variable to which the variable is related
                                // 0: x, 1: y, 2: z, 3: w
  int **seedvar_x;     		// x to which the variable is related
  				// [1...CellQty][0..dim]
  double **x_pvar; 		// pointer to associated variables in
				// SSet (SeedCoo0 or SeedWeight)

  // degrees of freedom
  char *dof;			// degrees of freedom: x y z and w (,-combined)

  // constraints
  double dist;			// distance by which a seed can move
  double *boundl;		// resulting lower bounds
  double *boundu;		// resulting upper bounds

  // resolution
  char *algoname;		// name of the NLopt algorithm
#ifdef HAVE_NLOPT
  nlopt_algorithm algo;		// NLopt algorithm
#endif
  double inistep;		// typical delta used for gradient computation

  // distribution-specific parameters
  char **disgrid;		// grid information for discretization
  double dissmoothing;		// std dev of the Gaussian used for smoothing
  struct FCT *cvl;        	// convolution function

  // Termination criteria -------------------------------------------
  int itermax;			// maximum iteration number
  double val;			// objective function
  double dvalditer;	        // derivative of the objective function
                                // wrt the iteration number
  double eps;			// delta on the objective function
  double reps;			// relative delta on the objective function
  double xeps;			// delta on the solution vector
  double xreps;			// relative delta on the solution vector
  double time;			// CPU time

  // OPTIMIZATION INFORMATION ------------------------------------------
  char *objective;		// objective function (for statistics,
  				// it may be 'ad', etc.; for tesr, it
				// may be 'vol', 'surf', etc.)

  // value of the objective function
  double *curval0;		// current values, no smoothing
  double *curval;		// current values
  double objval;		// value of the objective function
  double* objvalmin;		// minimum value of the objective function
  int iter;			// number of iterations

  // development information
  int faceconn;			// face connectivity rule to define the
  				// target cell points

  // PROGRESS INFORMATION ----------------------------------------------

  char *message;		// message to print at each iteration
  struct timeval end_time;	// needed for tracking time taken

  struct TDYN TDyn;		// tessellation dynamics
#ifdef HAVE_NLOPT
  nlopt_opt opt;		/// NLopt object
#endif
};
typedef struct TOPT TOPT;

#endif /* STRUCTTOPT_H */
