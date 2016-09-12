/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cpluplus
extern "C"
{
#endif

  extern int PolyhedronModification (int Nei, double *plane, struct
				     POLYMOD *pPolymod, int *BadVer);

  extern void NewPolyhedron_plane (int NeiId, int CenterSide, double *plane,
				   struct POLYMOD *pPolymod);

#ifdef __cpluplus
}
#endif
