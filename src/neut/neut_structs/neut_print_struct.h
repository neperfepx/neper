/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTPRINT_H
#define STRUCTPRINT_H

  struct PRINT
  {
    // povray
    char *povray;

    // camera settings
    char *cameracoostring;
    double *cameracoo;

    char *cameralookatstring;
    double *cameralookat;

    char *cameraskystring;
    double *camerasky;

    double cameraangle;

    char *cameraprojection;

    // image settings
    char *imagebackground;
    char *imagesize;
    int imageantialias;

    int showtess;
    int showtesr;
    int showmesh;
    int showslice;

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

    int *showpoint;

    int showshadow;
    int showcsys;

    // miscellaneous
    int datareduction;

    char *format;

    char *includepov;

    char *slice;
  };
  typedef struct PRINT PRINT;

#endif				/* STRUCTPRINT_H */

#ifdef __cplusplus
}
#endif
