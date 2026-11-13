/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<limits.h>
#include<assert.h>
#include<float.h>
#include<unistd.h>
#define frand() ((double) rand() / (RAND_MAX+1.0))

#include"ut.h"
#include"neut_m.h"

#ifdef HAVE_OPENMP
#include<omp.h>
#endif

#include "nem_partition.h"

#ifdef HAVE_LIBSCOTCH
extern void nem_partition_init (struct IN_M In, struct PART *pPart);

extern void nem_partition_prep (struct NODES *pNodes, struct MESH *Mesh,
				struct PART *pPart, SCOTCH_Mesh * pSCMesh);

extern void nem_partition_nodes (SCOTCH_Mesh *, struct NODES *, struct MESH *,
				 struct PART *);

extern void nem_partition_elts (struct NODES *, struct MESH *, struct PART *);

extern void nem_partition_elts_direct (struct MESH *Mesh, struct PART *pPart);


extern void nem_partition_stats (int, struct MESH, struct PART);

extern void nem_partition_elts_direct_pre (struct MESH *Mesh, struct PART Part,
                                           int **peltnodepartqty,
                                           int ***peltnodeparts,
                                           int ***peltnodepartnodeqty,
                                           int ****pnodepartelts,
                                           int ***pnodeparteltqty);

extern void nem_partition_elts_direct_recordinteriorelts (struct PART *pPart, int eltnodeqty,
                                              int ***nodepartelts, int **nodepareltqty);

extern int nem_partition_elts_direct_findelt (struct MESH Mesh, struct PART Part, int **eltnodeparts,
                                              int *eltnodepartqty, int **eltnodepartnodeqty,
                                              int ***nodepartelts,
                                              int **nodeparteltqty, int *availqty,
                                              int eltnodeqty, int *ppart, int *pelt);

extern void nem_partition_elts_direct_recordelt (struct PART *pPart, int part, int elt,
                                                 int *eltnodepartqty, int **eltnodeparts,
                                                 int **eltnodepartnodeqty,
                                                 int ***nodepartelts, int **nodeparteltqty,
                                                 int *availqty);

#endif
