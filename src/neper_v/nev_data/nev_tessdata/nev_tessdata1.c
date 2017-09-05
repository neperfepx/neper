/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_tessdata_.h"

void
nev_tessdata_init (struct TESS Tess, struct TESSDATA *pTessData)
{
  int dim, qty;
  double size, Rad1, Rad0, Rad4;

  neut_tess_size (Tess, &size);
  Rad1 = pow (size, 1. / 3) * 0.01414 / pow (Tess.CellQty, 0.25);
  Rad0 = pow (size, 1. / 3) * 0.02000 / pow (Tess.CellQty, 0.25);
  Rad4 = pow (size, 1. / 3) * 0.04000 / pow (Tess.CellQty, 0.25);

  for (dim = 0; dim <= 4; dim++)
  {
    neut_tessdata_dim_qty (*pTessData, dim, &qty);
    (*pTessData).Col[dim] = ut_alloc_2d_int (qty + 1, 3);
    (*pTessData).trs[dim] = ut_alloc_1d (qty + 1);
    (*pTessData).Rad[dim] = ut_alloc_1d (qty + 1);

    if (dim == 0)
    {
      ut_array_2d_int_set ((*pTessData).Col[dim] + 1, qty, 3, 0);
      ut_array_1d_set ((*pTessData).trs[dim] + 1, qty, 0);
      ut_array_1d_set ((*pTessData).Rad[dim] + 1, qty, Rad0);
    }
    else if (dim == 1)
    {
      ut_array_2d_int_set ((*pTessData).Col[dim] + 1, qty, 3, 0);
      ut_array_1d_set ((*pTessData).trs[dim] + 1, qty, 0);
      ut_array_1d_set ((*pTessData).Rad[dim] + 1, qty, Rad1);
    }
    else if (dim == 2 || dim == 3)
    {
      ut_array_2d_int_set ((*pTessData).Col[dim] + 1, qty, 3, 255);
      ut_array_1d_set ((*pTessData).trs[dim] + 1, qty, 0);
    }
    else if (dim == 4)
    {
      ut_array_2d_int_set ((*pTessData).Col[dim] + 1, qty, 3, 128);
      ut_array_1d_set ((*pTessData).trs[dim] + 1, qty, 0);
      ut_array_1d_set ((*pTessData).Rad[dim] + 1, qty, Rad4);
    }

    if ((*pTessData).ColData[dim])
    {
      if (!strcmp ((*pTessData).ColDataType[dim], "id")
       || !strncmp ((*pTessData).ColDataType[dim], "scaleid", 7)
       || !strcmp ((*pTessData).ColDataType[dim], "modeid"))
	nev_data_id_colour ((*pTessData).ColData[dim], qty,
			    (*pTessData).Col[dim]);
      else if (!strcmp ((*pTessData).ColDataType[dim], "col"))
	nev_data_col_colour ((*pTessData).ColData[dim], qty,
			     (*pTessData).Col[dim]);
      else if (!strncmp ((*pTessData).ColDataType[dim], "ori", 3))
	nev_data_ori_colour ((*pTessData).ColData[dim], qty,
			     "R", (*pTessData).Col[dim]);
      else if (!strcmp ((*pTessData).ColDataType[dim], "scal"))
	nev_data_scal_colour ((*pTessData).ColData[dim], NULL, qty,
			      (*pTessData).Scale[dim],
			      "blue,cyan,yellow,red",
			      (*pTessData).Col[dim],
			      &((*pTessData).Scale[dim]));
      else
	ut_error_reportbug ();
    }

    if ((*pTessData).trsdata[dim])
      nev_data_tr_tr ((*pTessData).trsdata[dim], qty, (*pTessData).trs[dim]);

    if ((*pTessData).RadData[dim])
      nev_data_rad_radius ((*pTessData).RadData[dim], qty,
			   (*pTessData).Rad[dim]);
  }

  return;
}

void
nev_tessdata_fscanf (struct TESS Tess, char *entity, char *prop,
		     char *argument, struct TESSDATA *pTessData)
{
  int id, entityqty;
  char *type = NULL, *value = NULL;

  nev_tessdata_fscanf_init (Tess, entity, prop, argument,
			    &id, &entityqty, &type, &value);

  if (!strcmp (prop, "col"))
    nev_tessdata_fscanf_col (Tess, pTessData, entity, id, entityqty, type, value);
  else if (!strcmp (prop, "trs"))
    nev_tessdata_fscanf_trs (pTessData, id, entityqty, type, value);
  else if (!strcmp (prop, "rad"))
    nev_tessdata_fscanf_rad (pTessData, id, entityqty, type, value);
  else if (!strcmp (prop, "colscheme"))
    nev_tessdata_fscanf_colscheme (pTessData, id, type);
  else if (!strcmp (prop, "scale"))
    nev_tessdata_fscanf_scale (pTessData, id, type);
  else if (!strcmp (prop, "scaletitle"))
    nev_tessdata_fscanf_scaletitle (pTessData, id, value);
  else
    ut_error_reportbug ();

  ut_free_1d_char (type);
  ut_free_1d_char (value);

  return;
}
