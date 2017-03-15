/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTTDYN_H
#define STRUCTTDYN_H

/// \brief Tessellation dynamics structure
/// The structure gathers information required by net_polycomp for
/// tessellation dynamics computation (fast tessellation update)
  struct TDYN
  {
    // GENERAL INFORMATION -----------------------------------------------

    char *algoneigh;		// algorithm to compute the neighbours

    // GENERAL INFORMATION -----------------------------------------------

    int iter;			// iteration number

    // GENERAL TESSELLATION INFORMATION ----------------------------------

    int N;			// number of seeds
    int Nall;			// number of seeds (periodic)

    double avdiameq;		// average cell diameter

    // REFERENCE STATE INFORMATION ---------------------------------------

    double **neighrefcoo;
    double *neighrefw;

    // ITERATION STATE INFORMATION ---------------------------------------

    int varchangedqty;		// number of changed seed variables
    int seedchangedqty;		// current number of changed seeds
    int *seedchanged;		// current changed seeds
    int cellchangedqty;		// current number of changed cells
    int *cellchanged;		// current changed cells

    int domcellqty;		// current number of domain cells (0 or 1)
    int *domcells;		// current domain cells
    int cellvoidqty;		// current number of void cells
    int *cellvoid;		// current void cells

    // CURRENT STATE INFORMATION -----------------------------------------

    // coo-shift and weight-shift
    double *shift;
    double *shiftw;

    // mean and max coo-shifts and weight-shifts
    double shiftmax;
    double shiftmean;
    double shiftwmax;
    double shiftwmean;

    // seed neighbours information
    int **neighlist;
    int *neighqty;
    double **neighdist;

    // INDIVIDUAL POLY COMPUTATION INFORMATION -------------------------
    int *changedneighs;
    int changedneighqty;

    // TIME INFORMATION ------------------------------------------------

    char *logtime;	        // logtime string
    char *logvar;	        // logvar string
    char *logdis;	        // logdis string
    char *logval;	        // logval string

    char *body;			// base filename for log files
    FILE *logtime_fp;	        // time log file
    FILE *logvar_fp;	        // variable log file
    int logdis_qty;		// number of distributions log files
    FILE **logdis_fp;	        // distributions log file
    FILE *logval_fp;	        // objective function value log file

    double cell_init_dur; 	// initialization duration
    double cell_kdtree_dur; 	// KD-tree duration
    double cell_shift_dur;	// cell calculation shift duration
    double cell_neigh_dur;    	// cell calculation neighbour-search duration
    double cell_cell_dur;   	// cell calculation per se duration
    double cell_other_dur;   	// cell calculation per se duration
    double cell_total_dur;      // cell calculation total duration

    // guest durations
    double var_dur;			// x-filling duration
    double seed_dur;			// seedset-filling duration
    double val_dur;			// objective-function-value-computation duration
    double total_dur;			// total duration
    double cumtotal_dur;	        // cumulative total duration
  };
  typedef struct TDYN TDYN;

#endif				/* STRUCTTDYN_H */

#ifdef __cplusplus
}
#endif
