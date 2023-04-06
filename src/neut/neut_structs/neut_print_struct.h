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
    int *showelset3d; // used by pf
    int *showelset2d; // used by pf
    int *showelset1d; // used by pf
    int *showelset0d; // used by pf
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
    double *cameracoo;
    double *cameralookat;
    double *camerasky;
    double cameraangle;
    char *cameraprojection;

    // light -------------------------------------------------------------------
    char **lightsources;
    int lightsourceqty;
    double lightambient;
    double lightdiffuse;
    double lightreflection;

    // internal for now --------------------------------------------------------
    int datareduction; // set to 1
  };
  typedef struct PRINT PRINT;

#endif				/* STRUCTPRINT_H */

#ifdef __cplusplus
}
#endif
