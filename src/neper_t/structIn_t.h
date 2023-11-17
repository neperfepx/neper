/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTIN_T_H
#define STRUCTIN_T_H

  struct IN_T
  {
    // global options (level-independent) --------------------------------------

    // Dimension
    int dim;

    // Domain
    char *domain;

    // Periodicity
    char *periodicstring;

    // Tesr size
    char *tesrsizestring;

    // Options that are defined per-level --------------------------------------

    // number of seeds (-n)
    char *nstring;
    char **n;

    // identifier (-id)
    char *idstring;
    char **id;

    // morphology (-morpho)
    char *morphostring;
    char **morpho;

    int optiqty;        // 2
    char **optitype;    // "morpho", "ori"

    // optimization algorithms (-*opti)
    char **optialgostring;
    char ***optialgo;

    // optimization / degrees of freedom (-*optidof)
    char **optidofstring;
    char ***optidof;

    // optimization / initial state (-*optiini)
    char *optiinistring;
    char **optiini;

    // optimization / max number of iterations without decreasing the objective function (-*algomaxiter)
    char *optialgomaxiterstring;
    char **optialgomaxiter;

    // optimization / objective function (-*optiobj)
    char *optiobjectivestring;
    char **optiobjective;

    // optimization / grid (-*optigrid)
    char *optigridstring;
    char **optigrid;

    // optimization / smoothing parameter (-*optismooth)
    char *optismoothstring;
    char **optismooth;

    // optimization / termination criterion (-*optistop)
    char **optistopstring;
    char ***optistop;

    // optimization / limit distance (-*optideltamax)
    char *optideltamaxstring;
    char **optideltamax;

    // optimization / initial step (-*optiinistep)
    char *optiinistepstring;
    char **optiinistep;

    // optimization / time log (-*optilogtime)
    char *optilogtimestring;
    char **optilogtime;

    // optimization / variable log (-*optilogvar)
    char *optilogvarstring;
    char **optilogvar;

    // optimization / distribution log (-*optilogdis)
    char *optilogdisstring;
    char **optilogdis;

    // optimization / tesr control points log (-*optilogtesr)
    char *optilogtesrstring;
    char **optilogtesr;

    // optimization / value log (-*optilogval)
    char *optilogvalstring;
    char **optilogval;

    // optimization / multiseed (-*optimultiseed)
    char *optimultiseedstring;
    char **optimultiseed;

    // orientation (-ori)
    char *oristring;
    char **ori;

    char *orisamplingstring;            // orientation sampling
    char **orisampling;                 //

    char *orioptineighstring;           // orientation opti / neighbor string
    char **orioptineigh;                //

    char *orioptiinistring;             // orientation opti initial orientations
    char **orioptiini;                  //

    char *orioptifixstring;             // orientation opti fixed orientations
    char **orioptifix;                  //

    char *orioptilogvarstring;           // variable log
    char **orioptilogvar;		//

    char *orispreadstring;		// orientation spread string
    char **orispread;                   // orientation spreads

    char *crysymstring;		        // crystal symmetry
    char **crysym;			//

    // post-tessellation options -----------------------------------------------

    int reg;				// regularization
    double fmax;			// regularization fmax
    char *seltype;			// regularization sel type
    char *selstring;			// regularization sel
    int mloop;				// regularization loop
    int maxedgedelqty;			// regularization max del
    char *sortstring;			// cell sorting
    int checktess;			// checking tess?

    char *transform;			// transformation

    char *group;                      // cell sets

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
    char *sttess;			// tess stat file
    char *sttesr;			// tesr stat file
    char *stc;				// cell stat file
    char *stv;				// ver stat file
    char *ste;				// edge stat file
    char *stf;				// face stat file
    char *stp;				// poly stat file
    char *sts;				// seed stat file
    char *stpt;				// point stat file
    char *stvox;		        // vox stat file
    char *stgroup;		        // group stat file
    char *obj;				// Wavefront obj file
    char *orif;				// orientation file
    char *sim;				// simulation file

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
