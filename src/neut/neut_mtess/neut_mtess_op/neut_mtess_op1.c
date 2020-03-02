/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_op_.h"

void
neut_mtess_set_zero (struct MTESS *pMTess)
{
  (*pMTess).LevelQty = 0;
  (*pMTess).LevelTessQty = NULL;
  (*pMTess).LevelTess = NULL;

  (*pMTess).TessQty = 0;
  (*pMTess).TessDom = NULL;
  (*pMTess).DomTess = NULL;

  (*pMTess).TessDQty = 0;

  (*pMTess).TessDomVerNb = NULL;
  (*pMTess).TessDomEdgeNb = NULL;
  (*pMTess).TessDomFaceNb = NULL;
  (*pMTess).DomTessFaceNb = NULL;

  return;
}

void
neut_mtess_free (struct MTESS *pMTess, struct TESS **pTess)
{
  int i;

  ut_free_1d_int (&(*pMTess).LevelTessQty);
  ut_free_2d_int (&(*pMTess).LevelTess, (*pMTess).LevelQty + 1);
  ut_free_2d_int (&(*pMTess).TessDom, (*pMTess).TessQty + 1);
  ut_free_2d_int (&(*pMTess).DomTess, (*pMTess).TessQty + 1);

  /* UNCOMMENT and FIXME with neper -T -n from_morpho::2 -morpho "cube(2)::voronoi"
     ut_free_2d_int (&(*pMTess).TessDomVerNb, (*pMTess).TessQty + 1);
     ut_free_2d_int (&(*pMTess).TessDomEdgeNb, (*pMTess).TessQty + 1);
     ut_free_2d_int (&(*pMTess).TessDomFaceNb, (*pMTess).TessQty + 1);
   */

  if ((*pMTess).DomTessFaceNb)
  {
    for (i = 0; i <= (*pMTess).TessQty; i++)
      ut_free_2d_int (&(*pMTess).DomTessFaceNb[i], (*pTess)[i].FaceQty + 1);
    ut_free_1d_ppint (&(*pMTess).DomTessFaceNb);
  }

  if ((*pTess))
  {
    for (i = 0; i <= (*pMTess).TessQty; i++)
      neut_tess_free ((*pTess) + i);
    free ((*pTess));
  }
  (*pTess) = NULL;

  neut_mtess_set_zero (pMTess);

  return;
}

void
neut_mtess_set_dom (struct MTESS *pMTess, struct TESS **pTess)
{
  (*pMTess).LevelQty = 0;

  (*pMTess).LevelTessQty = ut_alloc_1d_int (1);
  (*pMTess).LevelTessQty[0] = 1;

  (*pMTess).LevelTess = ut_alloc_2d_int (1, 2);
  ut_array_1d_int_set_2 ((*pMTess).LevelTess[0], 0, 0);

  (*pMTess).TessQty = 0;

  (*pMTess).TessDom = ut_alloc_2d_int (1, 2);
  ut_array_1d_int_set_2 ((*pMTess).TessDom[0], 0, 1);

  (*pMTess).DomTess = ut_alloc_2d_int (1, 2);
  (*pMTess).DomTess[0][1] = 0;

  (*pMTess).DomTessFaceNb = ut_alloc_1d_ppint (1);
  (*pMTess).DomTessFaceNb[0] = NULL;

  (*pMTess).TessDQty = 0;

  if (pTess)
  {
    (*pTess) = calloc (sizeof (struct TESS), 1);
    neut_tess_set_zero (*pTess);
  }

  return;
}

void
neut_mtess_set_tess (struct MTESS *pMTess, struct TESS **pTess)
{
  (*pMTess).LevelQty = 1;

  (*pMTess).LevelTessQty = ut_alloc_1d_int (2);
  ut_array_1d_int_set ((*pMTess).LevelTessQty, 2, 1);

  (*pMTess).LevelTess = ut_alloc_2d_int (2, 2);
  (*pMTess).LevelTess[1][1] = 1;

  (*pMTess).TessQty = 1;

  (*pMTess).TessDom = ut_alloc_2d_int (2, 2);
  ut_array_1d_int_set ((*pMTess).TessDom[1], 2, 1);

  (*pMTess).DomTess = ut_alloc_2d_int (1, 2);
  (*pMTess).DomTess[0][1] = 1;

  (*pMTess).TessDQty = 0;

  if (pTess != NULL)
  {
    (*pTess) = malloc (2 * sizeof (TESS));
    neut_tess_set_zero (*pTess);
    neut_tess_set_zero (*pTess + 1);
  }

  return;
}
