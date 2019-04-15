/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_nodedata.h
/// \brief Manipulate data associated to tessellations or meshes (TESSDATA, NODEDATA)
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_NODEDATA_H
#define NEUT_NODEDATA_H

  extern void neut_nodedata_set_default (struct NODEDATA *);
  extern void neut_nodedata_free (struct NODEDATA *);
  extern void neut_nodedata_coldatatype_size (struct NODEDATA NodeData,
					      int *psize);
  extern void neut_nodedata_mesh2slice (struct NODEDATA NodeData, struct
					NODES SNodes, int **node_newold,
					double *node_fact,
					struct NODEDATA *pSNodeData);

#endif				/* NEUT_NODEDATA_H */

#ifdef __cplusplus
}
#endif
