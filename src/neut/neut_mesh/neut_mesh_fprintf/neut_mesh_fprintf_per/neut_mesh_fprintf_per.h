/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FPRINTF_PER

  extern void neut_mesh_fprintf_per_plain (FILE * file, struct NODES Nodes);
  extern void neut_mesh_fprintf_per_geof (FILE * file, struct NODES Nodes);

#endif				// NEUT_MESH_FPRINTF_PER

#ifdef __cplusplus
}
#endif
