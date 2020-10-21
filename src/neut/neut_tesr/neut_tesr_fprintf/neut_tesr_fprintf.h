/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_VOX_FPRINTF_H
#define NEUT_VOX_FPRINTF_H

  extern void neut_tesr_fprintf (FILE *, char *, struct TESR);

  extern void neut_tesr_name_fprintf_raw (char *tesr, char *raw, char *format,
					  struct TESR Tesr);

  extern void neut_tesr_fprintf_raw (FILE *, char *, struct TESR);

  extern void neut_tesr_fprintf_ami (FILE *, char *, struct TESR);

#endif				/* NEUT_VOX_FPRINTF_H */

#ifdef __cplusplus
}
#endif
