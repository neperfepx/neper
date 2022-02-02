/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MTESS_OP_H
#define NEUT_MTESS_OP_H

/// \brief Set an MTESS structure to zero
///
///
///
  extern void neut_mtess_set_zero (struct MTESS *pMTess);

/// \brief Free an MTESS structure
///
///
///
  extern void neut_mtess_free (struct MTESS *pMTess, struct TESS **pTess);

/// \brief Set an MTESS structure to a domain (no tess)
///
///
///
  extern void neut_mtess_set_dom (struct MTESS *pMTess, struct TESS **pTess);

/// \brief Set an MTESS structure to a regular TESS
///
///
///
  extern void neut_mtess_set_tess (struct MTESS *pMTess, struct TESS **pTess);

#endif				/* NEUT_MTESS_OP_H */

#ifdef __cplusplus
}
#endif
