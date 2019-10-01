/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTIN_T_H
#define STRUCTIN_T_H

  struct IN_T
  {
    // global options (level-independent)
    int dim;				// dimension
    char *domain;			// domain
    char *periodicstring;		// periodicity
    char *tesrsizestring;		// size of the tesr

    char *nstring;			// number of seeds
    char **n;				//
    char *idstring;			// id of the seed distribution
    char **id;				//
    char *morphostring;		// morphology optimization
    char **morpho;			//
    char *morphooptialgostring;		// opti algorithm
    char **morphooptialgo;		//
    char *morphooptiinistring;		// opti algorithm
    char **morphooptiini;		//
    char *morphooptialgoneighstring;    // opti algorithm / neighbour search
    char **morphooptialgoneigh;		//
    char *morphooptialgomaxiterstring;  // max number of iterations without
    					// decreasing the objective function
    char **morphooptialgomaxiter;	//
    char *morphooptiobjectivestring;	// opti objective function
    char **morphooptiobjective;		//
    char *morphooptigridstring;		// grid definition
    char **morphooptigrid;		//
    char *morphooptismoothstring;	// smoothing parameter
    char **morphooptismooth;		//
    char *morphooptistopstring;		// opti stop criterion
    char **morphooptistop;		//
    char *morphooptidofstring;		// opti dofs
    char **morphooptidof;		//
    char *morphooptideltamaxstring;		// opti limit distance
    char **morphooptideltamax;		//
    char *morphooptiinistepstring;	// opti ini step
    char **morphooptiinistep;		//
    char *morphooptilogtimestring;      // time log
    char **morphooptilogtime;		//
    char *morphooptilogvarstring;       // variable log
    char **morphooptilogvar;		//
    char *morphooptilogdisstring;       // distribution log
    char **morphooptilogdis;		//
    char *morphooptilogtesrstring;      // tesr control points log
    char **morphooptilogtesr;		//
    char *morphooptilogvalstring;       // value log
    char **morphooptilogval;		//
    char *morphooptimultiseedstring;	// opti multiseed
    char **morphooptimultiseed;		//

    char *oristring;		        // orientation distribution string
    char **ori;                         // orientation distribution
    char *oricrysymstring;		// orientation crystal symmetry
    char **oricrysym;			//
    char *orioptistopstring;            // orientation opti / termination criterion
    char **orioptistop;                 //
    char *orioptineighstring;           // orientation opti / neighbour string
    char **orioptineigh;                //
    char *orioptiinistring;             // orientation opti initial orientations
    char **orioptiini;                  //
    char *orioptifixstring;             // orientation opti fixed orientations
    char **orioptifix;                  //
    char *orioptilogvarstring;           // variable log
    char **orioptilogvar;		//

    // post-tessellation options
    int reg;				// regularization
    double fmax;			// regularization fmax
    char *seltype;			// regularization sel type
    char *selstring;			// regularization sel
    int mloop;				// regularization loop
    int maxedgedelqty;			// regularization max del
    char *sortstring;			// cell sorting
    int checktess;			// checking tess?

    char *transform;			// transformation

    // internal variables
    int *periodic;			// periodicity
    int levelqty;			// number of levels
    char *mode;				// running mode: tess or tesr
    char *input;			// NULL (=none), n, tess, tesr

    char *loadpoint;			// loadpoint file
    char *load;				// loadtess or loadtesr file

    // output options
    char *format;			// output format
    char *tesrformat;			// output tesr format

    char *orides;			// orientation descriptor
    char *oriformat;			// orientation file format

    // output files
    char *body;				// file name without extension
    char *tess;				// tess file
    char *tesr;				// tesr file
    char *vtk;				// AMITEX-FFT file
    char *raw;				// raw data of tesr file
    char *geo;				// Gmsh geo file
    char *ply;				// Ply file
    char *stl;				// stl file
    char *dec;				// 3dec file
    char *fe;				// fe file
    char *stc;				// cell stat file
    char *stv;				// ver stat file
    char *ste;				// edge stat file
    char *stf;				// face stat file
    char *stp;				// poly stat file
    char *sts;				// seed stat file
    char *stpt;				// point stat file
    char *stvox;		        // vox stat file
    char *obj;				// Wavefront obj file
    char *orif;				// orientation file

    // output files for development
    char *debug;			// tess debug file
    char *mtess;			// multiscale tess file
    char *mgeo;				// multiscale Gmsh geo file
  };
  typedef struct IN_T IN_T;

#endif				/* STRUCTIN_T_H */

#ifdef __cplusplus
}
#endif
