/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
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

#include"../structIn_m.h"
#include"ut.h"
#include"neut_m.h"

#include "nem_partition.h"

#ifdef HAVE_LIBSCOTCH

extern void nem_partition_init (struct IN_M In, SCOTCH_Arch * pArch,
				struct PART *pPart);

extern void nem_partition_prep (struct NODES *pNodes, struct MESH *Mesh,
				struct PART *pPart, SCOTCH_Mesh * pSCMesh);

extern void nem_partition_writearch (FILE * file, int procqty, int coreqty,
				     int dcore, int dncor, int dproc);

extern void nem_partition_nodes (struct IN_M, SCOTCH_Mesh *, SCOTCH_Arch *,
				 struct NODES *, struct MESH *,
				 struct PART *);

extern void nem_partition_elts (struct IN_M, struct NODES *, struct MESH *, struct PART *);

extern void nem_partition_elts_direct (struct MESH *Mesh, struct PART *pPart);

extern void nem_partition_elts_bynumber (struct MESH *, struct PART *);
extern int nem_partition_elts_balancing_Q (struct MESH, struct PART *, double,
					   int, int **);
extern void nem_partition_elts_balancing_Q_elt (struct MESH, struct PART,
						int *, int *);

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
                                              int **nodeparteltqty, int eltnodeqty,
                                              int *ppart, int *pelt);

extern void nem_partition_elts_direct_recordelt (struct PART *pPart, int part, int elt,
                                                 int *eltnodepartqty, int **eltnodeparts,
                                                 int **eltnodepartnodeqty,
                                                 int ***nodepartelts, int **nodeparteltqty);

#endif
