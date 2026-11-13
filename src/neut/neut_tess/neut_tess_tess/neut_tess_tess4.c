/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_tess_.h"

void
neut_tess_tess_domfaces_minimal (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  (*pTessB).DomFaceQty = TessA.DomFaceQty;

  (*pTessB).DomFaceEq = ut_alloc_2d ((*pTessB).DomFaceQty + 1, 4);
  ut_array_2d_memcpy (TessA.DomFaceEq + 1, (*pTessB).DomFaceQty, 4,
                      (*pTessB).DomFaceEq + 1);

  (*pTessB).DomFaceType = ut_alloc_1d_pchar ((*pTessB).DomFaceQty + 1);
  for (i = 1; i <= (*pTessB).DomFaceQty; i++)
    ut_string_string (TessA.DomFaceType[i], (*pTessB).DomFaceType + i);

  (*pTessB).DomFaceParmQty = ut_alloc_1d_int ((*pTessB).DomFaceQty + 1);
  (*pTessB).DomFaceParms = ut_alloc_1d_pdouble ((*pTessB).DomFaceQty + 1);
  for (i = 1; i <= (*pTessB).DomFaceQty; i++)
  {
    (*pTessB).DomFaceParmQty[i] = TessA.DomFaceParmQty[i];
    if ((*pTessB).DomFaceParmQty[i] > 0)
    {
      (*pTessB).DomFaceParms[i] = ut_alloc_1d ((*pTessB).DomFaceParmQty[i]);
      ut_array_1d_memcpy (TessA.DomFaceParms[i], (*pTessB).DomFaceParmQty[i],
                          (*pTessB).DomFaceParms[i]);
    }
  }

  (*pTessB).DomFaceLabel = ut_alloc_1d_pchar ((*pTessB).DomFaceQty + 1);
  ut_array_1d_pchar_memcpy (TessA.DomFaceLabel + 1, (*pTessB).DomFaceQty,
                            (*pTessB).DomFaceLabel + 1);

  return;
}

/* neut_tess_tess_domprim copies the domain primitives */
void
neut_tess_tess_domprim (struct TESS TessA, struct TESS *pTessB)
{
  int i;

  for (i = 0; i < (*pTessB).DomPrimQty; i++)
    neut_prim_free ((*pTessB).DomPrim + i);
  free ((*pTessB).DomPrim);

  (*pTessB).DomPrimQty = TessA.DomPrimQty;
  (*pTessB).DomPrim =
    TessA.DomPrimQty ? (struct PRIM *) calloc (TessA.DomPrimQty, sizeof (struct PRIM)) : NULL;

  for (i = 0; i < TessA.DomPrimQty; i++)
  {
    neut_prim_set_zero ((*pTessB).DomPrim + i);
    neut_prim_memcpy (TessA.DomPrim[i], (*pTessB).DomPrim + i);
  }

  return;
}
