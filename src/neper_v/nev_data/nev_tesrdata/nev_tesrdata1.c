/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_tesrdata_.h"

void
nev_tesrdata_init (struct TESR Tesr, struct TESRDATA *pTesrData)
{
  int Qty = ut_array_1d_int_prod (Tesr.size, 3);

  (*pTesrData).Col = ut_alloc_2d_int (Qty + 1, 3);
  (*pTesrData).trs = ut_alloc_1d (Qty + 1);

  ut_array_2d_int_set ((*pTesrData).Col + 1, Qty, 3, 255);
  ut_array_1d_set ((*pTesrData).trs + 1, Qty, 0);

  if ((*pTesrData).ColData)
  {
    ut_print_message (0, 1, "Computing colours...\n");
    if (!strcmp ((*pTesrData).ColDataType, "id"))
      nev_data_id_colour ((*pTesrData).ColData, Qty, (*pTesrData).Col);
    else if (!strcmp ((*pTesrData).ColDataType, "col"))
      nev_data_col_colour ((*pTesrData).ColData, Qty, (*pTesrData).Col);
    else if (!strncmp ((*pTesrData).ColDataType, "ori", 3))
      nev_data_ori_colour ((*pTesrData).ColData, Qty, (*pTesrData).ColScheme, (*pTesrData).Col);
    else if (!strncmp ((*pTesrData).ColDataType, "disori", 6))
      nev_data_ori_colour ((*pTesrData).ColData, Qty, (*pTesrData).ColScheme, (*pTesrData).Col);
    else if (!strcmp ((*pTesrData).ColDataType, "scal"))
      nev_data_scal_colour ((*pTesrData).ColData,
	                    (*pTesrData).ColDataDef, Qty,
			    (*pTesrData).Scale,
			    "blue,cyan,yellow,red",
			    (*pTesrData).Col, &((*pTesrData).Scale));
    else
      ut_error_reportbug ();
  }

  if ((*pTesrData).trsdata)
  {
    ut_print_message (0, 1, "Computing transparency...\n");
    nev_data_tr_tr ((*pTesrData).trsdata, Qty, (*pTesrData).trs);
  }

  if ((*pTesrData).BRad < 0)
  {
    if (Tesr.Dim >= 2)
      (*pTesrData).BRad = 0.075 * ut_array_1d_gmean (Tesr.vsize, Tesr.Dim);
    else
      (*pTesrData).BRad = 1.00070287798127172169 * Tesr.vsize[0];
  }
  else if (Tesr.Dim == 1)
    (*pTesrData).BRad = sqrt (pow (Tesr.vsize[0], 2)
			      + pow (.5 * (*pTesrData).BRad, 2));

  if (!(*pTesrData).BCol)
    (*pTesrData).BCol = ut_alloc_1d_int (3);

  return;
}

void
nev_tesrdata_fscanf (struct TESR Tesr, char *entity, char *type,
		     char *argument, struct TESRDATA *pTesrData)
{
  int cell;
  struct TESRDATA TesrDataCell;
  struct TESRDATA *pTD = NULL;

  ut_print_message (0, 1, "Reading data (%s, %s)...\n", entity, type);

  neut_tesrdata_set_default (&TesrDataCell);

  cell = !strcmp (entity, "cell");

  if (!cell)
    pTD = pTesrData;
  else
  {
    TesrDataCell.Qty = Tesr.CellQty;
    pTD = &TesrDataCell;
  }

  if (!strcmp (entity, "cell") != 0)
  {
    nev_tesrdata_fscanf_cell (Tesr, pTD, type, argument);
    nev_tesrdata_cell2vox (Tesr, type, TesrDataCell, pTesrData);
  }
  else if (!strcmp (entity, "vox") != 0)
    nev_tesrdata_fscanf_vox (Tesr, pTD, type, argument);
  else if (!strcmp (entity, "voxedge") != 0)
    nev_tesrdata_fscanf_voxedge (pTD, type, argument);
  else
    abort ();

  return;
}
