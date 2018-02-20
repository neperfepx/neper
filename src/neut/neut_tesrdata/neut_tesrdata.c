/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tesrdata_.h"

void
neut_tesrdata_set_default (struct TESRDATA *pTesrData)
{
  (*pTesrData).type = NULL;

  (*pTesrData).Qty = 0;

  (*pTesrData).ColData = NULL;
  (*pTesrData).ColDataDef = NULL;
  (*pTesrData).ColDataType = NULL;
  (*pTesrData).ColScheme = NULL;
  (*pTesrData).Col = NULL;

  (*pTesrData).trsdata = NULL;
  (*pTesrData).trsdatatype = NULL;
  (*pTesrData).trscheme = NULL;
  (*pTesrData).trs = NULL;

  (*pTesrData).Scale = NULL;
  (*pTesrData).ScaleTitle = NULL;

  (*pTesrData).BCol = NULL;
  (*pTesrData).BRad = -1;

  return;
}
