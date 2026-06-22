/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_DATA_OP_H
#define NEUT_DATA_OP_H

extern void neut_data_set_default (struct DATA *pData);
extern void neut_data_free (struct DATA *pData);
extern void neut_data_mesh2slice_elts (struct DATA Data, struct MESH SMesh,
                                       int *elt_newold, struct DATA ***pSData);
extern void neut_data_mesh2slice_nodes (struct DATA NodeData, struct NODES SNodes,
                                        int **node_newold, double *node_fact,
                                        struct DATA *pSNodeData);

extern void neut_datainput_set_default (struct DATAINPUT *pDataInput);

extern void neut_data_symboldata_symbol (struct DATA *pData);

extern void neut_data_init_tess (struct TESS Tess, int dim, struct DATA *pTessData);
extern void neut_data_init_tesr (struct TESR Tesr, int id, struct DATA *pTesrData);
extern void neut_data_init_node (struct NODES Nodes, struct DATA *pData);
extern void neut_data_init_point (struct POINT Point, struct DATA *pPointData);
extern void neut_data_init_mesh (struct MESH Mesh, char *entity, struct DATA *pMeshData);

#endif				/* NEUT_DATA_OP_H */

#ifdef __cplusplus
}
#endif
