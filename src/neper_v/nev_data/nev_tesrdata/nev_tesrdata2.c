/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_tesrdata_.h"

void
nev_tesrdata_fscanf_cell (struct TESR Tesr, struct TESRDATA *pTD,
			  char *prop, char *argument)
{
  int i;
  char **args = NULL;
  int argqty;
  char *type = NULL, *value = NULL;

  nev_data_typearg_args (prop, argument, &type, &value, NULL);

  ut_string_separate (argument, NEUT_SEP_DEP, &args, &argqty);

  if (!strcmp (prop, "col"))
  {
    ut_string_string (type, &(*pTD).ColDataType);

    if (!strcmp ((*pTD).ColDataType, "col"))
    {
      (*pTD).ColData = ut_alloc_2d ((*pTD).Qty + 1, 3);
      ut_array_2d_fscanfn_wcard (value, (*pTD).ColData + 1,
				 (*pTD).Qty, 3, "colour,size");
    }
    else if (!strcmp ((*pTD).ColDataType, "id"))
    {
      (*pTD).ColData = ut_alloc_2d ((*pTD).Qty + 1, 1);
      for (i = 1; i <= (*pTD).Qty; i++)
	(*pTD).ColData[i][0] = Tesr.CellId ? Tesr.CellId[i] : i;
    }
    else if (!strncmp ((*pTD).ColDataType, "ori", 3))
      nev_data_fscanf_ori (value, (*pTD).Qty, Tesr.CellOri, &(*pTD).ColData,
                           &(*pTD).ColDataType);
    else if (!strcmp ((*pTD).ColDataType, "scal"))
    {
      (*pTD).ColData = ut_alloc_2d ((*pTD).Qty + 1, 1);
      ut_array_2d_fscanfn_wcard (value, (*pTD).ColData + 1,
				 (*pTD).Qty, 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (prop, "trs"))
  {
    ut_string_string (type, &(*pTD).trsdatatype);

    if (!strcmp ((*pTD).trsdatatype, "trs"))
    {
      (*pTD).trsdata = ut_alloc_2d ((*pTD).Qty + 1, 3);
      ut_array_2d_fscanfn_wcard (args[0], (*pTD).trsdata + 1,
				 (*pTD).Qty, 1, "numeral,size");
    }
    else if (!strcmp ((*pTD).trsdatatype, "scal"))
    {
      (*pTD).trsdata = ut_alloc_2d ((*pTD).Qty + 1, 3);
      ut_array_2d_fscanfn_wcard (args[0] + 5, (*pTD).trsdata + 1,
				 (*pTD).Qty, 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (prop, "colscheme"))
    ut_string_string (argument, &(*pTD).ColScheme);
  else if (!strcmp (prop, "scale"))
    ut_string_string (argument, &(*pTD).Scale);
  else if (!strcmp (prop, "scaletitle"))
    ut_string_string (argument, &(*pTD).ScaleTitle);
  else
    ut_error_reportbug ();

  ut_free_2d_char (args, argqty);
  ut_free_1d_char (value);

  return;
}

void
nev_tesrdata_fscanf_vox (struct TESR Tesr, struct TESRDATA *pTD,
			 char *prop, char *argument)
{
  int i;
  char *type = NULL, *value = NULL;

  nev_data_typearg_args (prop, argument, &type, &value, NULL);

  if (!strcmp (prop, "col"))
  {
    ut_string_string (type, &(*pTD).ColDataType);

    if (!strcmp ((*pTD).ColDataType, "col"))
    {
      (*pTD).ColData = ut_alloc_2d ((*pTD).Qty + 1, 3);
      ut_array_2d_fscanfn_wcard (value, (*pTD).ColData + 1,
				 (*pTD).Qty, 3, "colour,size");
    }
    else if (!strcmp ((*pTD).ColDataType, "id"))
    {
      (*pTD).ColData = ut_alloc_2d ((*pTD).Qty + 1, 1);
      for (i = 1; i <= (*pTD).Qty; i++)
	(*pTD).ColData[i][0] = i;
    }
    else if (!strncmp ((*pTD).ColDataType, "ori", 3))
      nev_data_fscanf_ori_tesr (Tesr, value, (*pTD).Qty, Tesr.VoxOri, Tesr.CellOri,
                                &(*pTD).ColData, &(*pTD).ColDataType);
    else if (!strncmp ((*pTD).ColDataType, "disori", 6))
    {
      nev_data_fscanf_ori_tesr (Tesr, value, (*pTD).Qty, Tesr.VoxOri, Tesr.CellOri,
                                &(*pTD).ColData, &(*pTD).ColDataType);
      int j, k, id = 0, cell;
      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
          {
            id++;
            cell = Tesr.VoxCell[i][j][k];
            ol_q_q_qdisori ((*pTD).ColData[id], Tesr.CellOri[cell],
                            Tesr.CellCrySym, (*pTD).ColData[id]);
          }
    }
    else if (!strcmp ((*pTD).ColDataType, "scal"))
    {
      (*pTD).ColData = ut_alloc_2d ((*pTD).Qty + 1, 1);
      ut_array_2d_fscanfn_wcard (value, (*pTD).ColData + 1,
				 (*pTD).Qty, 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (prop, "colscheme"))
    ut_string_string (argument, &(*pTD).ColScheme);
  else if (!strcmp (prop, "scale"))
    ut_string_string (argument, &(*pTD).Scale);
  else if (!strcmp (prop, "scaletitle"))
    ut_string_string (argument, &(*pTD).ScaleTitle);
  else
    ut_error_reportbug ();

  ut_free_1d_char (value);

  return;
}

void
nev_tesrdata_fscanf_voxedge (struct TESRDATA *pTD, char *prop, char *argument)
{
  if (!strcmp (prop, "col"))
  {
    (*pTD).BCol = ut_alloc_1d_int (3);
    ut_array_1d_int_fscanfn_wcard (argument, (*pTD).BCol, 3, "colour");
  }
  else if (!strcmp (prop, "rad"))
    sscanf (argument, "%lf", &((*pTD).BRad));
  else
    ut_error_reportbug ();

  return;
}

void
nev_tesrdata_cell2vox (struct TESR Tesr, char *prop,
		       struct TESRDATA TesrDataCell, struct TESRDATA *pTD)
{
  int i, j, k, id, size, vox;

  (*pTD).ColDataDef = ut_alloc_1d_int ((*pTD).Qty + 1);
  id = 0;
  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
	(*pTD).ColDataDef[++id] = (Tesr.VoxCell[i][j][k] > 0);

  ut_string_string ("cell", &(*pTD).type);

  if (!strcmp (prop, "col"))
  {
    (*pTD).ColDataType
      = ut_alloc_1d_char (strlen (TesrDataCell.ColDataType) + 1);
    strcpy ((*pTD).ColDataType, TesrDataCell.ColDataType);

    size = -1;
    if (!strcmp ((*pTD).ColDataType, "id"))
      size = 1;
    else if (!strcmp ((*pTD).ColDataType, "col")
	     || !strncmp ((*pTD).ColDataType, "ori", 3))
      size = 3;
    else if (!strcmp ((*pTD).ColDataType, "rad"))
      size = 1;
    else if (!strcmp ((*pTD).ColDataType, "scal"))
      size = 1;
    else
      abort ();

    (*pTD).ColData = ut_alloc_2d ((*pTD).Qty + 1, size);
    vox = 0;
    for (k = 1; k <= Tesr.size[2]; k++)
      for (j = 1; j <= Tesr.size[1]; j++)
	for (i = 1; i <= Tesr.size[0]; i++)
	{
	  vox++;
	  (*pTD).ColData[vox] = TesrDataCell.ColData[Tesr.VoxCell[i][j][k]];
	}
  }
  else if (!strcmp (prop, "colscheme"))
    ut_string_string (TesrDataCell.ColScheme, &(*pTD).ColScheme);
  else if (!strcmp (prop, "scale"))
    ut_string_string (TesrDataCell.Scale, &(*pTD).Scale);
  else if (!strcmp (prop, "scaletitle"))
    ut_string_string (TesrDataCell.ScaleTitle, &(*pTD).ScaleTitle);
  else
    ut_error_reportbug ();

  return;
}
