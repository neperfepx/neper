/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_ORI_H
#define NEUT_MESH_ORI_H

extern void neut_mesh_elts_olset (struct NODES Nodes, struct MESH Mesh,
                      int *elts, int eltqty, struct OL_SET *pOSet);

extern void neut_mesh_elsets_olset (struct NODES Nodes, struct MESH Mesh,
                      double **ElsetOri, int *elsets, int elsetqty, struct OL_SET *pOSet);

extern void neut_mesh_elts_orimean (struct NODES Nodes, struct MESH Mesh,
                                    int *elts, int eltqty, double *mean);

extern void neut_mesh_elts_orianiso (struct NODES Nodes, struct MESH Mesh,
                                     int *elts, int eltqty, double **evect,
                                     double *eval);

extern void neut_mesh_elset_orianiso (struct NODES Nodes, struct MESH Mesh,
                                      int elset,
                                      double **evect,
                                      double *eval);

extern void neut_mesh_elt_ori (struct MESH Mesh, int id, double *ori);

extern void neut_mesh_elset_gos (struct NODES Nodes, struct MESH Mesh,
                                int elset, double *pgos);
extern void neut_mesh_elts_gos (struct NODES Nodes, struct MESH Mesh,
                                int *elts, int eltqty, double *pgos);

extern void neut_mesh_elset_orianiso_delta (struct NODES Nodes, struct MESH Mesh, int elset,
                               double **evect_in, double *delta_in);
extern void neut_mesh_elts_orianiso_delta (struct NODES Nodes, struct MESH Mesh, int *elts, int eltqty,
                               double **evect_in, double *delta_in);

extern void neut_mesh_elset_orianisogos (struct NODES Nodes, struct MESH Mesh,
                                         int elset, double *pgos);

#endif				/* NEUT_MESH_ORI_H */

#ifdef __cplusplus
}
#endif
