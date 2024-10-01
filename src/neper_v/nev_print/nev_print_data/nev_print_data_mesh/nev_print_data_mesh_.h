/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"ut.h"
#include"neut_v.h"

#include "nev_print_data_mesh.h"

extern void nev_print_data_mesh_init (struct MESH *Mesh,
                                struct DATA **MeshData, char **pentity,
                                char *attribute, char *datastring,
                                int *pdim, int *pentityqty,
                                char **ptype, char **pvalue, struct DATA **ppData);

extern void nev_print_data_mesh_elt (struct SIM Sim, struct TESS *pTess,
                                      struct NODES *pNodes,
                                      struct MESH **pMesh, char
                                      *entity, int dim, int entityqty,
                                      char *attribute, char *type, char *datastring,
                                      char *value, struct DATA *pData);

extern void nev_print_data_mesh_eltedge (char *attribute, char *datastring,
                                       struct DATA *pMD);

extern void nev_print_data_mesh_elset2elt (struct MESH Mesh, char *entity,
                                     char *attribute, struct DATA MeshDataSet,
                                     struct DATA *pData);

extern void nev_print_data_mesh_elt_rad (char *datastring, struct DATA *pMD);
