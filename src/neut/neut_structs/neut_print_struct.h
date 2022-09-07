/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTPRINT_H
#define STRUCTPRINT_H

  struct PRINT
  {
    // prerequisites -----------------------------------------------------------
    char *povray;
    int povrayantialiasing;

    char *asymptote;

    // space -------------------------------------------------------------------
    char *space;

    // space -------------------------------------------------------------------
    char *pfpolestring;
    int** pfpoles;
    int pfpoleqty;

    char *pfprojection;         // stereographic or equal-area
    char *pfmode;               // representation mode: point, contour, density
    char *pffont;               // TimesRoman or ComputerModern

    int pfgridsize;
    char *pfkernel;
    double pfkernelsig;
    char *pfdirstring;
    double **pfdir;             // [0,1]
    char *pfsym;
    int pfclustering;

    char *pfshape;

    // slicing -----------------------------------------------------------------
    char *slice;

    // show --------------------------------------------------------------------
    int showtess;
    int showtesr;
    int showmesh;
    int showslice;
    int showcsys;
    int showscale;

    int inputqty;
    char **inputs;

    int *shownode;
    int *showelt3d;
    int *showelt2d;
    int *showelt1d;
    int *showelt0d;

    int *showseed;
    int *showver;
    int *showedge;
    int *showface;
    int showfaceinter;
    int *showpoly;
    int ***showvox;
    char *showvoxstring;
    char *showvoidvoxstring;
    char *showedgestring;

    int **showpoint;
    int *showcrystal;

    // camera ------------------------------------------------------------------
    char *cameracoostring;
    double *cameracoo;
    char *cameralookatstring;
    double *cameralookat;
    char *cameraskystring;
    double *camerasky;
    double cameraangle;
    char *cameraprojection;

    // light -------------------------------------------------------------------
    char *lightsourcestring;
    char **lightsources;
    int lightsourceqty;
    char *lightambientstring;
    double lightambient;
    char *lightdiffusestring;
    double lightdiffuse;
    char *lightreflectionstring;
    double lightreflection;

    // scene -------------------------------------------------------------------
    char *scenebackground;

    // image -------------------------------------------------------------------
    char *imagesize;
    char *imageformat;

    // output ------------------------------------------------------------------
    char *outdir;

    // advanced ----------------------------------------------------------------
    char *includepov;

    // internal for now --------------------------------------------------------
    int datareduction; // set to 1
  };
  typedef struct PRINT PRINT;

#endif				/* STRUCTPRINT_H */

#ifdef __cplusplus
}
#endif
