/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTFEPXSIM_H
#define STRUCTFEPXSIM_H

struct FEPXSIM
{
  char *fepxdir;        // FEPX simulation directory

  int NodeQty;          // number of nodes
  int EltQty;           // number of elements
  int PartQty;          // number of partitions
  int *PartNodeQty;     // number of nodes of the partitions
  int *PartEltQty;      // number of elements of the partitions

  char *OriDes;         // orientation descriptor

  int NodeResQty;       // node result qty
  char **NodeRes;       // node results
  int EltResQty;        // elt result qty
  char **EltRes;        // elt results

  int StepQty;          // number of steps
  int *StepState;       // 1 if step written, 0 otherwise
  int RestartId;        // FEPX simulation directory - restart number
  int RestartFiles;     // 1 if restart files, 0 otherwise

  // hardwired
  char *tess;           // tessellation
  char *tesr;           // raster tessellation
  char *msh;            // mesh
  char *bcs;            // bcs
  char *ori;            // ori
  char *phase;          // phase
  char *cfg;            // config
};
typedef struct FEPXSIM FEPXSIM;
#endif /* STRUCTFEPXSIM_H */
