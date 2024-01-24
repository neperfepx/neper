/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"

extern void neut_mesh_fscanf_msh_head (FILE *, char **, char **, int *);
extern void neut_mesh_fscanf_msh_nodes (FILE *, char *, struct NODES *, int **);
extern void neut_mesh_fscanf_msh_nodes_head (FILE *, int *);
extern void neut_mesh_fscanf_msh_nodes_prop (FILE *, char *, struct NODES *, int *);
extern void neut_mesh_fscanf_msh_nodes_foot (FILE  *);


extern void neut_mesh_fscanf_msh_elts (FILE * file, char *mode, char *domain, int *node_nbs,
    struct MESH *pMesh0D, struct MESH *pMesh1D, struct
    MESH *pMesh2D, struct MESH *pMesh3D, struct MESH *pMeshCo, struct MESH **ppMesh);
extern int neut_mesh_fscanf_msh_elts_dim (FILE *, char *, struct MESH *, int *, int, int);
extern void neut_mesh_fscanf_msh_elts_foot (FILE *);
extern void neut_mesh_fscanf_msh_elts_dim_prop (FILE *, char *, struct MESH *, int **, int);

extern void neut_mesh_fscanf_msh_periodicity (FILE *file, struct NODES *pNodes);

extern void neut_mesh_fscanf_msh_physical (FILE *file, struct MESH *pMesh0D,
                               struct MESH *pMesh1D, struct MESH *pMesh2D,
                               struct MESH *pMesh3D);

extern void neut_mesh_fscanf_msh_orientations (FILE *file, char *entity, struct MESH *pMesh);
extern void neut_mesh_fscanf_msh_groups (FILE *file, struct MESH *pMesh);
extern void neut_mesh_fscanf_msh_nsets (FILE *file);
extern void neut_mesh_fscanf_msh_fasets (FILE *file, struct MESH *pMesh);
extern void neut_mesh_fscanf_msh_nodeparts (FILE *file, struct NODES *pNodes);

extern void neut_mesh_fscanf_msh_crysym (FILE *file, struct NODES *pNodes, struct MESH *pMesh0D,
                                         struct MESH *pMesh1D, struct MESH *pMesh2D,
                                         struct MESH *pMesh3D, struct MESH *pMeshCo);

extern void neut_mesh_fscanf_msh_version (FILE *file);
