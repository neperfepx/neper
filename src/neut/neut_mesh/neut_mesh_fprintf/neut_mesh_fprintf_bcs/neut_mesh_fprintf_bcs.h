/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FPRINTF_BCS

extern void neut_mesh_fprintf_bcs (FILE * file, struct NSET NSet0D,
                                   struct NSET NSet1D, struct NSET NSet2D,
                                   char *nset);

#endif				// NEUT_MESH_FPRINTF_BCS

#ifdef __cplusplus
}
#endif
