/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"

#include"neut_mesh_fprintf_msh.h"

extern void neut_mesh_fprintf_msh_pre (struct TESS Tess,
                                        char *fasetlist, char ***pfasets,
                                        int **pfasetids, int *pfasetqty);

extern void neut_mesh_fprintf_msh_header (FILE * file, char *mode,
                                           char *version);

extern void neut_mesh_fprintf_msh_domain (FILE * file, char *Domain);

extern void neut_mesh_fprintf_msh_elts (FILE * file, char *mode,
                                         struct TESS Tess, struct MESH Mesh0D,
                                         struct MESH Mesh1D,
                                         struct MESH Mesh2D,
                                         struct MESH Mesh3D,
                                         struct MESH CMesh, char **fasets,
                                         int *fasetids, int fasetqty,
                                         char *dim, char *numbering,
                                         int *shift);
extern void neut_mesh_fprintf_msh_elts_v4 (FILE * file, char *mode,
                                         struct TESS Tess, struct MESH Mesh0D,
                                         struct MESH Mesh1D,
                                         struct MESH Mesh2D,
                                         struct MESH Mesh3D,
                                         struct MESH CMesh, char **fasets,
                                         int *fasetids, int fasetqty,
                                         char *dim, char *numbering,
                                         int *shift);
extern void neut_mesh_fprintf_msh_nodes (FILE *file, char *mode, struct NODES Nodes);
extern void neut_mesh_fprintf_msh_nodes_print_mesh (struct MESH Mesh);

extern void neut_mesh_fprintf_msh_periodicity (FILE *file, struct NODES Nodes);

extern void neut_mesh_fprintf_msh_nsets (FILE * file, struct NSET NSet0D,
                                         struct NSET NSet1D, struct NSET NSet2D,
                                         char *nsetlist);
extern void neut_mesh_fprintf_msh_nset (FILE * file, char *name, int NodeQty, int *nset);

extern void neut_mesh_fprintf_msh_fasets (FILE * file, struct TESS Tess,
                                          struct MESH Mesh2D,
                                          struct MESH Mesh3D, int *shift,
                                          char *fasetlist);

extern void neut_mesh_fprintf_msh_nodeparts (FILE * file, struct NODES Nodes);

extern void neut_mesh_fprintf_msh_physical (FILE * file, struct MESH Mesh0D,
                                             struct MESH Mesh1D,
                                             struct MESH Mesh2D,
                                             struct MESH Mesh3D,
                                             struct MESH MeshCo,
                                             char **fasets, int *fasetids,
                                             int fasetqty, char *dim);

extern void neut_mesh_fprintf_msh_elts_dim (FILE * file, char *mode,
                                             int shift, struct MESH Mesh);
extern void neut_mesh_fprintf_msh_elts_dim_v4 (FILE * file, char *mode,
                                               struct MESH Mesh, int shift);

extern void neut_mesh_fprintf_msh_nodes_v4 (FILE * file, char *mode, struct TESS, struct
    NODES Nodes, struct MESH Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D,
    struct MESH Mesh3D);

extern void neut_mesh_fprintf_msh_entities (FILE * file, char *mode, struct
    TESS Tess, struct NODES Nodes, struct MESH Mesh0D, struct MESH Mesh1D,
    struct MESH Mesh2D, struct MESH Mesh3D);

extern void neut_mesh_fprintf_msh_entities_dim (FILE * file, char *mode,
                                                struct TESS Tess,
                                                struct NODES Nodes,
                                                struct MESH Mesh);

extern void neut_mesh_fprintf_msh_elts_pre (struct TESS Tess, struct MESH
    Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D, struct MESH Mesh3D, struct
    MESH MeshCo, char **fasets, int fasetqty, int *fasetids, char *dim, char
    *numbering, int **peltfaset, int *shift, int *peltqty);

extern void neut_mesh_fprintf_msh_nodes_dim_v4 (FILE * file, char *mode, struct TESS Tess,
                                     struct NODES Nodes,
                                     struct MESH MeshL, struct MESH MeshU);
