/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_ORI_H
#define NEUT_MESH_ORI_H

extern void neut_mesh_elset_orianiso (struct NODES Nodes, struct MESH Mesh,
                                      double **EltOri, char *crysym, int elset,
                                      double **evect, double *eval);

#endif				/* NEUT_MESH_ORI_H */

#ifdef __cplusplus
}
#endif
