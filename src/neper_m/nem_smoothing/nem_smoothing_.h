/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

#include"ut.h"
#include"neut_m.h"
#include"../structIn_m.h"
#include "../nem/nem.h"

#include"nem_smoothing.h"

extern void nem_smoothing_barypos (struct NODES Nodes,
				   int *neighnodes, double *neighnodeweights,
				   int neighnodeqty, double *NeighCoo);
extern void nem_smoothing_newcoo (double *coo, double *neighcoo, double
				  A, double *newcoo);
extern void nem_smoothing_nodes_updatecoo (struct NODES *pNodes, int *nodes,
					   int nodeqty, double **NodeCoo);
extern void nem_smoothing_neighnodeweights (struct NODES Nodes,
					    struct MESH *Mesh, int node,
					    int *neighnodes, int neighnodeqty,
					    double **pneighnodeweights);
