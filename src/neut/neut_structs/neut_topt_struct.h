/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
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
  char *DomType;                // type of domain (same as Dom.DomType)
  struct TESS Dom0;		// domain (1-cell tessellation), not modified by aspratio
  struct TESS Dom;		// domain (1-cell tessellation), modified by aspratio
  struct TESS DomPer;		// 3x3x3-scale domain (for periodicity), modified by aspratio
  struct POLY DomPoly;		// domain as a POLY, modified by aspratio
  double *aspratio;             // aspect ratio (lengths) along the 3 axes
  int *activedim;               // active dimension (columnar -> 1 zero, bamboo -> 2 zeros)

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
  int tavoxqtyini;		// initial number of target points.
  double *tartesrscale;		// scaling factor of the tess (for elongated grains)
  double  ***tarcellpts;	// list of points coo of a target cell [0...]
  double  **tarcellptweights;	// list of points weight of a target cell [0...]
  int    *tarcellptqty;		// number of point of a target cell
  double *tarcellfact;		// factor of a target cell
  int  ***tarcellptscells;      // sorted subcells of a point
                                // [cell][pt][0...CellSCellQty[cell]-1]
  double **tarcellptsdist;      // distance value of cell points

  // CURRENT MICROSTRUCTURE DEFINITION ---------------------------------

  // seeds and tessellation
  struct SEEDSET SSet;		// Seed set
  struct POLY *Poly;		// set of polys
  double *CellSize;             // Poly sum (volume or area) (!= NULL / stored if known)
  struct CRYS Crys;             // crystal
  double *DomParms;             // domain parameters

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
  char *optitype;               // type of variables: seeds or behavior
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
  int algoqty;			// number of algorithms
  char **algoname;		// names of the NLopt algorithms
  int algoid;			// id of the active algo: 0...algoqty-1
#ifdef HAVE_NLOPT
  nlopt_algorithm *algo;	// NLopt algorithms [0,...]
#endif
  int *algomaxiter;		// maximum allowed number of iterations without
  				// decreasing the objective function
  double inistep;		// typical delta used for gradient computation

  // distribution-specific parameters
  char **grid;			// discretization grid
  double *cvlsig;		// std dev of the Gaussians used for smoothing
  struct FCT *cvl;		// smoothing functions
  char *cvlmethod;		// convolution method, "numerical" or "analytical"

  // Termination criteria -------------------------------------------
  int itermax;			// maximum iteration number
  double val;			// objective function
  double eps;		        // delta on the objective function
  double reps;		        // relative delta on the objective function
  double nlopt_eps;		// delta on the objective function, nlopt-style
  double nlopt_reps;		// relative delta on the objective function, nlopt-style
  double xeps;			// delta on the solution vector
  double xreps;			// relative delta on the solution vector
  double time;			// CPU time
  double loopmax;		// maximum number of optimization loop

  // OPTIMIZATION INFORMATION ------------------------------------------
  char **tarobjective;		// target objective functions
                                // defined on a per-variable basis:
                                // - for statistics, it may be 'ad', etc.;
                                // - for tesr, it may be 'vol', 'surf', etc.
  char *objective;		// global objective functions:
  char *objective_tesrval;

  // value of the objective function
  double *curval0;		// current values, no smoothing
  double *curval;		// current values
  double objval;		// value of the objective function
  double* objvalmin;		// minimum value of the objective function
  double objval0;		// value of the objective function, no smoothing
  double* objvalmin0;		// minimum value of the objective function, no smoothing
  int iter;			// number of iterations
  int loop;			// number of loops
  int* loop_start;		// iterations at the start of the loops [loop - 1]
  int* loop_plateau_length;	// lengths of the loop plateaus [loop - 1]
  int* loop_status;		// status of the loop [loop - 1] (0: ok, -1: ended by a
  				// plateau, -2: is a plateau)

  // PROGRESS INFORMATION ----------------------------------------------

  char *message;		// message to print at each iteration
  struct timeval end_time;	// needed for tracking time taken

  struct TDYN TDyn;		// tessellation dynamics
#ifdef HAVE_NLOPT
  nlopt_opt opt;		// NLopt object
#endif

  void *pnf_cloud;              // pointer to the nanoflann cloud
  void *pnf_tree;               // pointer to the nanoflann tree
  int *ptid_seedid;             // seed corresponding to a point of the cloud
  int *seedid_ptid;             // point of the cloud corresponding to a seed
};
typedef struct TOPT TOPT;

#endif /* STRUCTTOPT_H */
