/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_pointdata_.h"

void
neut_pointdata_set_default (struct POINTDATA *pPointData)
{
  (*pPointData).PointQty = 0;

  (*pPointData).ColData = NULL;
  (*pPointData).ColDataType = NULL;
  (*pPointData).ColScheme = NULL;
  (*pPointData).Col = NULL;

  (*pPointData).trsdata = NULL;
  (*pPointData).trsdatatype = NULL;
  (*pPointData).trscheme = NULL;
  (*pPointData).trs = NULL;

  (*pPointData).RadData = NULL;
  (*pPointData).RadDataType = NULL;
  (*pPointData).Rad = NULL;

  (*pPointData).Scale = NULL;
  (*pPointData).ScaleTitle = NULL;

  (*pPointData).CooData = NULL;
  (*pPointData).CooDataType = NULL;
  (*pPointData).Coo = NULL;
  (*pPointData).CooFact = 1;
  (*pPointData).Space = NULL;

  return;
}

void
neut_pointdata_free (struct POINTDATA *pPointData)
{
  if ((*pPointData).ColData)
    ut_free_2d ((*pPointData).ColData, (*pPointData).PointQty + 1);

  ut_free_1d_char ((*pPointData).ColDataType);
  ut_free_1d_char ((*pPointData).ColScheme);

  if ((*pPointData).Col)
    ut_free_2d_int ((*pPointData).Col, (*pPointData).PointQty + 1);

  if ((*pPointData).RadData)
    ut_free_2d ((*pPointData).RadData, (*pPointData).PointQty + 1);

  ut_free_1d_char ((*pPointData).RadDataType);
  ut_free_1d ((*pPointData).Rad);

  ut_free_1d_char ((*pPointData).Scale);
  ut_free_1d_char ((*pPointData).ScaleTitle);

  if ((*pPointData).CooData)
    ut_free_2d ((*pPointData).CooData, (*pPointData).PointQty + 1);

  ut_free_1d_char ((*pPointData).CooDataType);

  if ((*pPointData).Coo)
    ut_free_2d ((*pPointData).Coo, (*pPointData).PointQty + 1);

  if ((*pPointData).trsdata)
    ut_free_2d ((*pPointData).trsdata, (*pPointData).PointQty + 1);

  ut_free_1d_char ((*pPointData).trsdatatype);
  ut_free_1d_char ((*pPointData).trscheme);
  ut_free_1d ((*pPointData).trs);
  ut_free_1d_char ((*pPointData).Space);

  return;
}

void
neut_pointdata_coldatatype_size (struct POINTDATA PointData, int *psize)
{
  if (!strcmp (PointData.ColDataType, "col")
      || !strcmp (PointData.ColDataType, "ori"))
    (*psize) = 3;
  else if (!strcmp (PointData.ColDataType, "rad")
	   || !strcmp (PointData.ColDataType, "scal")
	   || !strcmp (PointData.ColDataType, "trs"))
    (*psize) = 1;
  else
    ut_error_reportbug ();

  return;
}
