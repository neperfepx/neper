/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
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
    {
      (*pTD).ColData = ut_alloc_2d ((*pTD).Qty + 1, 4);

      if (!value)
      {
	if (!Tesr.CellOri)
	  ut_print_message (2, 3, "No orientation data available.\n");
	else
	{
	  ut_string_string ("oriq", &((*pTD).ColDataType));
	  ut_array_2d_memcpy ((*pTD).ColData + 1, (*pTD).Qty, 4,
			      Tesr.CellOri + 1);
	}
      }
      else
      {
	double *tmpd = ut_alloc_1d (4);
	int *tmpi = ut_alloc_1d_int (6);
	double **tmpdd = ut_alloc_2d (3, 3);
	FILE *file = ut_file_open (value, "r");

	int qty0;
	if (!strcmp ((*pTD).ColDataType + 3, "e")
	    || !strcmp ((*pTD).ColDataType + 3, "ek")
	    || !strcmp ((*pTD).ColDataType + 3, "er")
	    || !strcmp ((*pTD).ColDataType + 3, "R"))
	  qty0 = 3;
	else if (!strcmp ((*pTD).ColDataType + 3, "q"))
	  qty0 = 4;
	else if (!strcmp ((*pTD).ColDataType + 3, "m"))
	  qty0 = 6;
	else if (!strcmp ((*pTD).ColDataType + 3, "g"))
	  qty0 = 9;
	else
	{
	  printf ("(*pTD).ColDataType = %s\n", (*pTD).ColDataType);
	  ut_error_reportbug ();
	  abort ();
	}

	ut_file_nbwords_testwmessage (value, (*pTD).Qty * qty0);

	for (i = 1; i <= (*pTD).Qty; i++)
	  if (!strcmp ((*pTD).ColDataType + 3, "e"))
	  {
	    ol_e_fscanf (file, tmpd);
	    ol_e_q (tmpd, (*pTD).ColData[i]);
	  }
	  else if (!strcmp ((*pTD).ColDataType + 3, "ek"))
	  {
	    ol_e_fscanf (file, tmpd);
	    ol_ek_e (tmpd, tmpd);
	    ol_e_q (tmpd, (*pTD).ColData[i]);
	  }
	  else if (!strcmp ((*pTD).ColDataType + 3, "er"))
	  {
	    ol_e_fscanf (file, tmpd);
	    ol_er_e (tmpd, tmpd);
	    ol_e_q (tmpd, (*pTD).ColData[i]);
	  }
	  else if (!strcmp ((*pTD).ColDataType + 3, "q"))
	    ol_q_fscanf (file, (*pTD).ColData[i]);
	  else if (!strcmp ((*pTD).ColDataType + 3, "R"))
	  {
	    ol_R_fscanf (file, tmpd);
	    ol_R_q (tmpd, (*pTD).ColData[i]);
	  }
	  else if (!strcmp ((*pTD).ColDataType + 3, "m"))
	  {
	    ol_m_fscanf (file, tmpi);
	    ol_m_q (tmpi, (*pTD).ColData[i]);
	  }
	  else if (!strcmp ((*pTD).ColDataType + 3, "g"))
	  {
	    ol_g_fscanf (file, tmpdd);
	    ol_g_q (tmpdd, (*pTD).ColData[i]);
	  }
	  else
	    ut_error_reportbug ();
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
nev_tesrdata_fscanf_rptb (struct TESRDATA *pTD, char *prop, char *argument)
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
nev_tesrdata_cell2rpt (struct TESR Tesr, char *prop,
		       struct TESRDATA TesrDataCell, struct TESRDATA *pTD)
{
  int i, j, k, id, size, rpt;

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
    rpt = 0;
    for (k = 1; k <= Tesr.size[2]; k++)
      for (j = 1; j <= Tesr.size[1]; j++)
	for (i = 1; i <= Tesr.size[0]; i++)
	{
	  rpt++;
	  (*pTD).ColData[rpt] = TesrDataCell.ColData[Tesr.VoxCell[i][j][k]];
	}
  }
  else if (!strcmp (prop, "colscheme"))
  {
    (*pTD).ColScheme = ut_alloc_1d_char (strlen (TesrDataCell.ColScheme) + 1);
    strcpy ((*pTD).ColScheme, TesrDataCell.ColScheme);
  }
  else if (!strcmp (prop, "scale"))
  {
    (*pTD).Scale = ut_alloc_1d_char (strlen (TesrDataCell.Scale) + 1);
    strcpy ((*pTD).Scale, TesrDataCell.Scale);
  }
  else if (!strcmp (prop, "scaletitle"))
  {
    (*pTD).ScaleTitle
      = ut_alloc_1d_char (strlen (TesrDataCell.ScaleTitle) + 1);
    strcpy ((*pTD).ScaleTitle, TesrDataCell.ScaleTitle);
  }
  else
    ut_error_reportbug ();

  return;
}
