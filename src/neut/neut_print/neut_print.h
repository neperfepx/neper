/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
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
  extern void neut_print_outdir (char *outdir, struct SIM Sim, char *format, char **pdir);
  extern void neut_print_imagesize (char *imagesize, int *pwidth, int *pheight);
  extern int neut_print_cell_show (struct PRINT Print, struct TESS Tess, int cell);
  extern void neut_print_init_ipfborder (char *, struct PRINT *pPrint);

  extern void neut_print_povheader (char *format, struct PRINT Print, FILE * file);
  extern void neut_print_povfoot (char *includepov, char *imageformat, FILE * file);

  extern void neut_print_pov2png (char *povray, char *filename, int imagewidth,
                                  int imageheight, int antialiasing, int messagetag);

#endif				/* NEUT_PRINT_H */

#ifdef __cplusplus
}
#endif
