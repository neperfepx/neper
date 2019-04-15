/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESSE_OP_H
#define NEUT_TESSE_OP_H

  extern void neut_tesse_set_zero (struct TESSE *pTessE);
  extern void neut_tesse_face_addface (struct TESSE *pTessE, int face,
				       int fface, int fori);
  extern void neut_tesse_poly_addpoly (struct TESSE *pTessE, int poly,
				       int fpoly);
  extern void neut_tesse_free (struct TESSE *pTessE);

#endif				/* NEUT_TESSE_OP_H */

#ifdef __cplusplus
}
#endif
