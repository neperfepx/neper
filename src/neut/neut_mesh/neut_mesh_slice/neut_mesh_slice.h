/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_SLICE_H
#define NEUT_MESH_SLICE_H

extern void neut_mesh_slice (struct NODES Nodes, struct MESH Mesh, struct DATA
    NodeData, struct DATA MeshData, char *slice, int *pSQty, struct NODES
    **pN, struct MESH ***pM, struct DATA **pData, struct DATA ****pSMeshData, int
    ***pEltNewOld);

#endif /* NEUT_MESH_SLICE_H */

#ifdef __cplusplus
}
#endif
