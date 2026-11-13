/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#include<float.h>
#include<limits.h>

#include"neut_m.h"
#include"ut.h"

#include"../nem_interface.h"
#include"nem_interface_dup.h"

extern void nem_interface_dup_pre (struct TESS Tess, int *intertypes, int **pvers,
                                   int *pverqty, int **pedges, int *pedgeqty,
                                   int **pfaces, int *pfaceqty);
extern void nem_interface_dup_ver (struct TESS Tess, int *vers, int verqty,
                                   int *inters, int interqty,
                                   struct NODES *pNodes, struct MESH *Mesh, int ***pnodeseeds);
extern void nem_interface_dup_edge (struct TESS Tess, int *edges, int edgeqty,
                                    int *inters, int interqty,
                                    struct NODES *pNodes, struct MESH *Mesh, int ***pnodeseeds);
extern void nem_interface_dup_face (struct TESS Tess, int *faces, int faceqty,
                                    int *inters, int interqty,
                                    struct NODES *pNodes, struct MESH *Mesh, int ***pnodeseeds);

extern void nem_interface_duplicate (int **groups, int groupqty, int *nodes, int nodeqty,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     int ***pnodeseeds);

extern void nem_interface_dup_boundelts_3d (struct TESS Tess, int *faces, int faceqty,
                                            struct NODES Nodes, int **nodeseeds,
                                            struct MESH *Mesh, struct BOUNDARY *pBound);
extern void nem_interface_dup_boundelts_2d (struct TESS Tess, int *edges, int edgeqty,
                                            struct NODES Nodes, int **nodeseeds,
                                            struct MESH *Mesh, struct BOUNDARY *pBound);

extern void nem_interface_dup_renumber_1d (struct TESS Tess, struct NODES Nodes,
                                           int **nodeseeds, struct MESH *Mesh);
extern void nem_interface_dup_renumber_2d (struct TESS Tess, struct NODES Nodes,
                                           int **nodeseeds, struct MESH *Mesh);

extern void nem_interface_dup_nodeseeds_complete (struct TESS Tess, struct MESH *Mesh,
                                                  int **nodeseeds);
extern void nem_interface_dup_per (struct TESS Tess, struct NODES *pNodes, int **nodeseeds);

extern void nem_interface_dup_seeds (struct TESS Tess, int *inters,
                                     int interqty, int dim, int id,
                                     int **pseeds, int *pseedqty);
extern void nem_interface_dup_seedgroups (struct TESS Tess, int *inters, int interqty,
                                          int dim, int id, int *seeds, int seedqty,
                                          int ***pgroups, int *pgroupqty);

extern void nem_interface_dup_nodes_seed_elt (struct MESH Mesh, int *nodes, int nodeqty,
                                              int seed, int *pelt);

extern int nem_interface_dup_seedpair_continuous (struct TESS Tess, int *inters,
                                                  int interqty, int dim, int id,
                                                  int seed1, int seed2);
extern int nem_interface_dup_entity_shift (struct TESS Tess, int dim, int id, int *shift,
                                           int *pid);
