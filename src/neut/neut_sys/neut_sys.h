/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_sys.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_SYS_H
#define NEUT_SYS_H

  extern int neut_sys_data (char **pdata, int verbosity);
  extern int neut_sys_exedir (char **pdir);
  extern int neut_sys_cache (char *subdir, char **pcache);

#endif				/* NEUT_SYS_H */

#ifdef __cplusplus
}
#endif
