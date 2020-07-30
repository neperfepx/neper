/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTSIM_H
#define STRUCTSIM_H

struct SIM
{
  char *simdir;         // simulation directory
  int StepQty;          // number of steps
  int NodeResQty;       // number of result files
  int EltResQty;        // number of result files
  char **NodeRes;       // nodal result files (extensions)
  char **EltRes;        // elemental result files (extensions)
  int *NodeResWritten;  // nodal result files (extensions), written?
  int *EltResWritten;   // elemental result files (extensions), written?
  char *OriDes;         // orientation descriptor

  // hardwired
  char *body;           // name of the simulation
  char *tess;		// tessellation
  char *msh;		// mesh

  // Only set when reading an FEPX raw result directory and used to transform
  // it into a simulation directory
  char *fepxdir;        // FEPX simulation directory
  int EltQty;           // number of elements
  int NodeQty;          // number of nodes
  int PartQty;          // number of partitions
  int *PartNodeQty;     // number of nodes of the partitions
  int *PartEltQty;      // number of elements of the partitions
  int SlipSystemQty;    // number of slip systems

  // Only used by -V
  int step;             // current step
};
typedef struct SIM SIM;
#endif /* STRUCTSIM_H */
