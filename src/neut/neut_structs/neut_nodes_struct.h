/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTNODES_H
#define STRUCTNODES_H

struct NODES
{
  int NodeQty;		// total nb of nodes
  // arrays are indexed [1...NodeQty]

  double **NodeCoo;		// for each node: coordinates, indexed [O...2]

  double *NodeCl;		// for each node: characteristic length

  int *Periodic;		// [0...2] for x, y and z periodicity
  // (0: no, 1: yes)
  double *PeriodicDist;	// [0...2] periodicity size along x, y and z

  int PerNodeQty;		// number of periodic (slave) nodes
  int *PerNodeNb;		// ids of periodic (slave) nodes [1...]
  int *PerNodeMaster;		// master of nodes
  int **PerNodeShift;		// shift of a node
  int *PerNodeSlaveQty;		// number of slaves of a master
  int **PerNodeSlaveNb;		// slaves of a master [1...]

  int DupNodeQty;		// number of duplicate (slave) nodes
  int* DupNodeNb;		// ids of the duplicate nodes [1...]
  int* DupNodeMaster;		// master of nodes [NodeQty + 1...]
  int* DupNodeCell;		// cell of nodes [NodeQty + 1...]
  int* DupNodeSlaveQty;		// number of duplicates of a master [NodeQty + 1...]
  int** DupNodeSlaveNb;		// duplicates of a master [NodeQty + 1...][1...]
};
typedef struct NODES NODES;

#endif

#ifdef __cplusplus
}
#endif
