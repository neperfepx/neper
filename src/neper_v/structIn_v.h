/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTIN_H
#define STRUCTIN_H

struct IN_V
{
  char *input;

  // prerequisites
  char *povray;			// path to povray
  char *asymptote;		// path to asymptote

  // input
  char *step;
  char *crysym;

  // space
  char *space;

  // data
  int dataqty;
  char ***data;

  // show
  int showqty;
  char ***show;

  // slicing
  char *slice;

  // camera
  char *cameracoo;
  char *cameralookat;
  char *camerasky;
  char *cameraangle;
  char *cameraprojection;

  // light
  char *lightsource;
  char *lightambient;
  char *lightdiffuse;
  char *lightreflection;

  // scene
  char *scenebackground;

  // pf and ipf
  char *pfpolestring;
  char *pfpolelabel;
  char *pfprojection;
  char *pfprojlabel;
  char *pfmode;
  char *pffont;
  char *pfgridsize;
  char *pfkernel;
  char *pfdirstring;
  char *pfsym;
  char *pfshape;
  int pfclustering;

  // image
  char *imagesize;
  char *imageformat;

  // povray
  int povrayantialiasing;

  // output
  char *outdir;

  // print

  // advanced
  char *includepov;

  // output file name
  char *print;
};
typedef struct IN_V IN_V;

#endif /* STRUCTIN_H */
