/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTSIM_H
#define STRUCTSIM_H

struct SIM
{
  char *simdir;         // simulation directory
  int StepQty;          // number of steps
  char *OriDes;         // orientation descriptor
  int *StepState;       // 1 if step written, 0 otherwise
  int RestartId;        // FEPX simulation directory - restart number
  int RestartFiles;     // 1 if restart files, 0 otherwise

  int EntityQty;        // number of entities
  char **Entities;      // entities [0...]

  char **EntityType;    // for an entity, type of its members, which can be:
                        // - node: each member is a node (case of "node")
                        // - elt: each member is one elt (case of "elt")
                        // - elset: each member is one set of elements (case of
                        // "elset", "mesh", and other elt-based entities)
                        // - unknown: if unknown

  int *EntityMemberQty; // for an entity, number of members [entity, 0...]
  char ***EntityMemberExpr; // for an entity, expression of the members (if any) [entity, 0...][1...]
  int ***EntityMembers; // for an entity, number of the members [entity, 0...][1...][0...]

  int *EntityResQty;    // for an entity, number of results [0...]
  char ***EntityRes;    // for an entity, results
  char ***EntityResExpr;// for an entity, result expressions (if any)

  char* OriSpace;       // orientation space

  // hardwired
  char *body;           // name of the simulation
  char *tess;           // tessellation
  char *tesr;           // raster tessellation
  char *msh;            // mesh
  char *bcs;            // bcs
  char *ori;            // ori
  char *phase;          // phase
  char *config;         // config

  // Only set when reading an FEPX raw result directory and used to transform
  // it into a simulation directory
  char *fepxdir;        // FEPX simulation directory
  int NodeQty;          // number of nodes
  int EltQty;           // number of elements
  int ElsetQty;         // number of elsets
  int PartQty;          // number of partitions
  int *PartNodeQty;     // number of nodes of the partitions
  int *PartEltQty;      // number of elements of the partitions

  int step;             // current step
};
typedef struct SIM SIM;
#endif /* STRUCTSIM_H */
