/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTSIM_H
#define STRUCTSIM_H

struct SIM
{
  char *simdir;         // simulation directory
  int StepQty;          // number of steps
  int NodeResQty;       // number of nodal results
  int EltResQty;        // number of elemental results
  int ElsetResQty;      // number of elset results
  char **NodeRes;       // nodal results
  char **EltRes;        // elemental results
  char **ElsetRes;      // element set results
  char **NodeResExpr;   // nodal result expressions (optional)
  char **EltResExpr;    // elemental result expressions (optional)
  char **ElsetResExpr;  // elset result expressions (optional)
  int *NodeResWritten;  // nodal results, written?
  int *EltResWritten;   // elemental results, written?
  int *ElsetResWritten; // elset results, written?
  char *OriDes;         // orientation descriptor
  int *StepState;       // 1 if step written, 0 otherwise
  int RestartId;   // FEPX simulation directory - restart number
  int RestartFiles;     // 1 if restart files, 0 otherwise

  // hardwired
  char *body;           // name of the simulation
  char *tess;		// tessellation
  char *tesr;		// raster tessellation
  char *msh;		// mesh
  char *bcs;		// bcs
  char *ori;		// ori
  char *phase;		// phase

  // Only set when reading an FEPX raw result directory and used to transform
  // it into a simulation directory
  char *fepxdir;        // FEPX simulation directory
  int NodeQty;          // number of nodes
  int EltQty;           // number of elements
  int ElsetQty;         // number of elsets
  int GroupQty;         // number of groups (phases)
  int PartQty;          // number of partitions
  int *PartNodeQty;     // number of nodes of the partitions
  int *PartEltQty;      // number of elements of the partitions
  int SlipSystemQty;    // number of slip systems

  int step;             // current step
};
typedef struct SIM SIM;
#endif /* STRUCTSIM_H */
