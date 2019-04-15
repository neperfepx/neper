/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_tessdata.h
/// \brief Manipulate data associated to tessellations (TESSDATA)
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_TESSDATA_H
#define NEUT_TESSDATA_H

  extern void neut_tessdata_set_default (struct TESSDATA *);
  extern void neut_tessdata_free (struct TESSDATA *);
  extern void neut_tessdata_entity_dim (char *, int *);
  extern void neut_tessdata_entity_qty (struct TESSDATA, char *, int *);
  extern void neut_tessdata_dim_qty (struct TESSDATA TessData, int dim,
				     int *pqty);

#endif				/* NEUT_TESSDATA_H */

#ifdef __cplusplus
}
#endif
