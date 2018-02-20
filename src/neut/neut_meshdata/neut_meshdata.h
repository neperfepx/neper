/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_meshdata.h
/// \brief Manipulate data associated to tessellations or meshes (TESSDATA, MESHDATA)
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_MESHDATA_H
#define NEUT_MESHDATA_H

  extern void neut_meshdata_set_default (struct MESHDATA *);
  extern void neut_meshdata_free (struct MESHDATA *);
  extern void neut_meshdata_coldatatype_size (struct MESHDATA MeshData,
					      int *psize);

  extern void neut_meshdata_mesh2slice (struct MESHDATA *MeshData,
					struct MESH SMesh,
					int *elt_newold,
					struct MESHDATA **pSMeshData);

#endif				/* NEUT_MESHDATA_H */

#ifdef __cplusplus
}
#endif
