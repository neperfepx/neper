/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neut_m.h"
#include "nem_readmesh.h"

extern void nem_readmesh_nsets (struct NSET NSetIn, int dim, struct NSET *NSet);
extern int nem_readmesh_nsets_split (char *name, char **labels, int labelqty, int exclude);
extern void nem_readmesh_nsets_inter (struct NSET *NSet, int dim, int pair, int triple);
extern void nem_readmesh_nsets_inter_name (struct NSET NSet, int *ids, int idqty, char **pname);
extern void nem_readmesh_nsets_inter_mask (struct NSET NSet, int id, int *mask, int val);
extern void nem_readmesh_nsets_inter_nodes (struct NSET NSet, int id, int *mask, int val,
                                            int **pnodes, int *pqty);
extern void nem_readmesh_nsets_add (struct NSET *pNSet, char *name, int *nodes, int qty);
