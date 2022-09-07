/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_print.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_PRINT_H
#define NEUT_PRINT_H

  extern void neut_print_set_default (struct PRINT *);
  extern void neut_print_free (struct PRINT *);
  extern void neut_print_outdir (struct PRINT Print, struct SIM Sim, char *format, char **pdir);
  extern void neut_print_imagesize (struct PRINT Print, int *pwidth, int *pheight);
  extern int neut_print_cell_show (struct PRINT Print, struct TESS Tess, int cell);

#endif				/* NEUT_PRINT_H */

#ifdef __cplusplus
}
#endif
