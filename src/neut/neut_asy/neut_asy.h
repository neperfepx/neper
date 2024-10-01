/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_asy.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_ASYMPTOTE_H
#define NEUT_ASYMPTOTE_H

  extern int neut_asy_check (char *asy);
  extern int neut_asy_check_error (char *asy);
  extern void neut_asy_convert (char *asymptote, char *filename, int imagewidth, int imageheight,
                  char *formatstring, int messagetag);

#endif				/* NEUT_ASYMPTOTE_H */

#ifdef __cplusplus
}
#endif
