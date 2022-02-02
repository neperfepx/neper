/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESR_FPRINTF_H
#define NEUT_TESR_FPRINTF_H

  extern void neut_tesr_fprintf (FILE * file, char *format, struct TESR Tesr);

  extern void neut_tesr_name_fprintf_raw (char *tesr, char *raw, char *format,
					  struct TESR Tesr);

  extern void neut_tesr_fprintf_ami (FILE * file, char *format, struct TESR Tesr);

#endif				/* NEUT_TESR_FPRINTF_H */

#ifdef __cplusplus
}
#endif
