/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TDYN_FPRINTF_H
#define NEUT_TDYN_FPRINTF_H

/// \brief Set a TDYN structure to zero
///
///
///
  extern void neut_tdyn_fprintf_pre (struct TDYN *pTDyn);
  extern void neut_tdyn_fprintf_post (struct TDYN *pTDyn);
  extern void neut_tdyn_fprintf_time (struct TDYN TDyn);

#endif				/* NEUT_TDYN_FPRINTF_H */

#ifdef __cplusplus
}
#endif
