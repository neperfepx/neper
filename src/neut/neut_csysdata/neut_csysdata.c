/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_csysdata_.h"

void
neut_csysdata_set_default (struct CSYSDATA *pCsysData)
{
  (*pCsysData).ColData = NULL;
  (*pCsysData).ColDataType = NULL;
  (*pCsysData).Col = NULL;

  (*pCsysData).RadData = 0;
  (*pCsysData).RadDataType = NULL;
  (*pCsysData).Rad = 0;

  (*pCsysData).LengthData = 0;
  (*pCsysData).LengthDataType = NULL;
  (*pCsysData).Length = 0;

  (*pCsysData).CooData = NULL;
  (*pCsysData).CooDataType = NULL;
  (*pCsysData).Coo = NULL;

  (*pCsysData).Label = NULL;
  (*pCsysData).FontSize = -1;

  return;
}

void
neut_csysdata_free (struct CSYSDATA *pCsysData)
{
  ut_free_1d ((*pCsysData).ColData);
  ut_free_1d_char ((*pCsysData).ColDataType);
  ut_free_1d_int ((*pCsysData).Col);
  (*pCsysData).RadData = 0;
  ut_free_1d_char ((*pCsysData).RadDataType);
  (*pCsysData).Rad = 0;
  (*pCsysData).LengthData = 0;
  ut_free_1d_char ((*pCsysData).LengthDataType);
  (*pCsysData).Length = 0;

  ut_free_1d ((*pCsysData).CooData);
  ut_free_1d_char ((*pCsysData).CooDataType);
  ut_free_1d ((*pCsysData).Coo);

  ut_free_2d_char ((*pCsysData).Label, 3);
  (*pCsysData).FontSize = -1;

  return;
}

void
neut_csysdata_coldatatype_size (struct CSYSDATA CsysData, int *psize)
{
  if (!strcmp (CsysData.ColDataType, "col")
      || !strcmp (CsysData.ColDataType, "ori"))
    (*psize) = 3;
  else if (!strcmp (CsysData.ColDataType, "rad")
	   || !strcmp (CsysData.ColDataType, "scal"))
    (*psize) = 1;
  else
    ut_error_reportbug ();

  return;
}
