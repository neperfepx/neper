/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tessdata_.h"

void
neut_tessdata_set_default (struct TESSDATA *pTessData)
{
  (*pTessData).SeedQty = 0;
  (*pTessData).VerQty = 0;
  (*pTessData).EdgeQty = 0;
  (*pTessData).FaceQty = 0;
  (*pTessData).PolyQty = 0;

  (*pTessData).ColData = ut_alloc_1d_ppdouble (5);
  (*pTessData).ColDataType = ut_alloc_1d_pchar (5);
  (*pTessData).ColScheme = ut_alloc_1d_pchar (5);
  (*pTessData).Col = ut_alloc_1d_ppint (5);

  (*pTessData).trsdata = ut_alloc_1d_ppdouble (5);
  (*pTessData).trsdatatype = ut_alloc_1d_pchar (5);
  (*pTessData).trscheme = ut_alloc_1d_pchar (5);
  (*pTessData).trs = ut_alloc_1d_pdouble (5);

  (*pTessData).RadData = ut_alloc_1d_ppdouble (5);
  (*pTessData).RadDataType = ut_alloc_1d_pchar (5);
  (*pTessData).Rad = ut_alloc_1d_pdouble (5);

  (*pTessData).Scale = ut_alloc_1d_pchar (5);
  (*pTessData).ScaleTitle = ut_alloc_1d_pchar (5);

  return;
}

void
neut_tessdata_free (struct TESSDATA *pTessData)
{
  int i, qty;

  for (i = 0; i < 5; i++)
  {
    neut_tessdata_dim_qty (*pTessData, i, &qty);
    ut_free_2d ((*pTessData).ColData[i], qty);
    ut_free_2d ((*pTessData).trsdata[i], qty);
    ut_free_2d ((*pTessData).RadData[i], qty);
    ut_free_2d_int ((*pTessData).Col[i], qty);
  }
  ut_free_1d_ppdouble ((*pTessData).ColData);
  ut_free_1d_ppdouble ((*pTessData).RadData);
  ut_free_1d_ppint ((*pTessData).Col);
  ut_free_2d_char ((*pTessData).ColDataType, 5);
  ut_free_2d_char ((*pTessData).ColScheme, 5);
  ut_free_2d_char ((*pTessData).trsdatatype, 5);
  ut_free_2d_char ((*pTessData).trscheme, 5);
  ut_free_2d ((*pTessData).trs, 5);

  ut_free_2d ((*pTessData).Rad, 5);
  ut_free_2d_char ((*pTessData).RadDataType, 5);

  ut_free_2d_char ((*pTessData).Scale, 5);
  ut_free_2d_char ((*pTessData).ScaleTitle, 5);

  return;
}

void
neut_tessdata_entity_dim (char *entity, int *pdim)
{
  if (!strcmp (entity, "ver"))
    (*pdim) = 0;
  else if (!strcmp (entity, "edge"))
    (*pdim) = 1;
  else if (!strcmp (entity, "face"))
    (*pdim) = 2;
  else if (!strcmp (entity, "poly"))
    (*pdim) = 3;
  if (!strcmp (entity, "seed"))
    (*pdim) = 4;

  return;
}

void
neut_tessdata_entity_qty (struct TESSDATA TessData, char *entity, int *pqty)
{
  if (!strcmp (entity, "ver"))
    (*pqty) = TessData.VerQty;
  else if (!strcmp (entity, "edge"))
    (*pqty) = TessData.EdgeQty;
  else if (!strcmp (entity, "face"))
    (*pqty) = TessData.FaceQty;
  else if (!strcmp (entity, "poly"))
    (*pqty) = TessData.PolyQty;
  else if (!strcmp (entity, "seed"))
    (*pqty) = TessData.SeedQty;
  else
    ut_error_reportbug ();

  return;
}

void
neut_tessdata_dim_qty (struct TESSDATA TessData, int dim, int *pqty)
{
  if (dim == 0)
    (*pqty) = TessData.VerQty;
  else if (dim == 1)
    (*pqty) = TessData.EdgeQty;
  else if (dim == 2)
    (*pqty) = TessData.FaceQty;
  else if (dim == 3)
    (*pqty) = TessData.PolyQty;
  else if (dim == 4)
    (*pqty) = TessData.SeedQty;

  return;
}
