/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_FPRINTF_H
#define NEUT_TESS_FPRINTF_H

  extern void neut_tess_fprintf (FILE *, struct TESS);
  extern void neut_tess_name_fprintf_gmsh (char *, struct TESS);
  extern void neut_tess_fprintf_gmsh (FILE *, struct TESS);
  extern void neut_tess_name_fprintf (char *, struct TESS);
  extern void neut_tess_fprintf_ply (FILE *, struct TESS);
  extern void neut_tess_fprintf_stl (FILE *, struct TESS);
  extern void neut_tess_cell_fprintf_stl (FILE *, struct TESS, int cell);
  extern void neut_tess_name_fprintf_stl (char *, struct TESS);
  extern void neut_tess_name_fprintf_stl_bycell (char *, struct TESS);
  extern void neut_tess_fprintf_obj (FILE *, struct TESS);
  extern void neut_tess_fprintf_svg (FILE *, char *, struct TESS);
  extern void neut_tess_fprintf_dec (FILE *, struct TESS);
  extern void neut_tess_fprintf_fe (FILE *, struct TESS);
  extern void neut_tess_fprintf_ovm (FILE *, struct TESS);

#endif				/* NEUT_TESS_FPRINTF_H */

#ifdef __cplusplus
}
#endif
