/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_tessdata_.h"
  
void
nev_tessdata_fscanf_init (struct TESS Tess,
			  char *entity_in, char *prop, char *argument,
			  int *pid, int *pentityqty,
			  char **ptype, char **pvalue)
{
  ut_print_message (0, 1, "Reading data (%s, %s)...\n", entity_in, prop);
  char *tmp = NULL;

  if (!strcmp (entity_in, "cell"))
    neut_tess_cell (Tess, &tmp);
  else
    ut_string_string (entity_in, &tmp);

  neut_tess_entity_dim (tmp, pid);
  neut_tess_entity_qty (Tess, tmp, pentityqty);

  if (*pid < 0 || *pentityqty < 0)
    abort ();

  nev_data_typearg_args (prop, argument, ptype, pvalue, &tmp);

  ut_free_1d_char (tmp);

  return;
}

void
nev_tessdata_fscanf_col (struct TESS Tess,
                         struct TESSDATA *pTessData, char *entity,
                         int id, int entityqty,
                         char *type, char *value)
{
  int i, scale, qty;

  ut_string_string (type, &(*pTessData).ColDataType[id]);

  if (!strcmp (type, "col"))
  {
    (*pTessData).ColData[id] = ut_alloc_2d (entityqty + 1, 3);
    ut_array_2d_fscanfn_wcard (value, (*pTessData).ColData[id] + 1,
			       entityqty, 3, "colour,size");
  }
  else if (!strcmp (type, "id"))
  {
    (*pTessData).ColData[id] = ut_alloc_2d (entityqty + 1, 1);

    if (!value)
      for (i = 1; i <= entityqty; i++)
	(*pTessData).ColData[id][i][0]
	  = (!strcmp (entity, "cell") && Tess.CellId) ? Tess.CellId[i] : i;
    else
      ut_array_2d_fscanfn_wcard (value, (*pTessData).ColData[id] + 1,
				 entityqty, 1, "numeral");
  }
  else if (!strncmp (type, "scaleid", 7))
  {
    if (sscanf (type, "scaleid(%d)", &scale) != 1)
      scale = 1;
    (*pTessData).ColData[id] = ut_alloc_2d (entityqty + 1, 1);
    for (i = 1; i <= entityqty; i++)
    {
      (*pTessData).ColData[id][i][0]
	= (!strcmp (entity, "cell") && Tess.ScaleCellId
	   && scale >= 0 && scale <= Tess.ScaleQty) ? Tess.ScaleCellId[i][scale] : i;
    }
  }
  else if (!strcmp (type, "modeid"))
  {
    (*pTessData).ColData[id] = ut_alloc_2d (entityqty + 1, 1);

    if (!value)
      for (i = 1; i <= entityqty; i++)
	(*pTessData).ColData[id][i][0]
	  = (!strcmp (entity, "cell") && Tess.CellModeId) ?
	  Tess.CellModeId[i] : 1;
    else
      ut_array_2d_fscanfn_wcard (value, (*pTessData).ColData[id] + 1,
				 entityqty, 1, "numeral");
  }
  else if (!strncmp (type, "ori", 3))
  {
    (*pTessData).ColData[id] = ut_alloc_2d (entityqty + 1, 4);

    if (!value)
    {
      if (!Tess.CellOri)
	ut_print_message (2, 3, "No orientation data available.\n");
      else
      {
	ut_string_string ("oriq", &((*pTessData).ColDataType[id]));
	ut_array_2d_memcpy ((*pTessData).ColData[id] + 1,
			    Tess.CellQty, 4, Tess.CellOri + 1);
      }
    }
    else
    {
      double *tmpd = ut_alloc_1d (4);
      int *tmpi = ut_alloc_1d_int (6);
      double **tmpdd = ut_alloc_2d (3, 3);
      FILE *file = ut_file_open (value, "r");

      if (strlen (type) == 3
	  || !strcmp (type + 3, "e") || !strcmp (type + 3, "ek")
	  || !strcmp (type + 3, "er") || !strcmp (type + 3, "R"))
	qty = 3;
      else if (!strcmp (type + 3, "q"))
	qty = 4;
      else if (!strcmp (type + 3, "m"))
	qty = 6;
      else if (!strcmp (type + 3, "g"))
	qty = 9;
      else
      {
	printf ("type = %s\n", type);
	ut_error_reportbug ();
	abort ();
      }

      ut_file_nbwords_testwmessage (value, entityqty * qty);

      for (i = 1; i <= Tess.CellQty; i++)
	if (!strcmp (type, "orie"))
	{
	  ol_e_fscanf (file, tmpd);
	  ol_e_q (tmpd, (*pTessData).ColData[id][i]);
	}
	else if (!strcmp (type, "oriek"))
	{
	  ol_e_fscanf (file, tmpd);
	  ol_ek_e (tmpd, tmpd);
	  ol_e_q (tmpd, (*pTessData).ColData[id][i]);
	}
	else if (!strcmp (type, "orier"))
	{
	  ol_e_fscanf (file, tmpd);
	  ol_er_e (tmpd, tmpd);
	  ol_e_q (tmpd, (*pTessData).ColData[id][i]);
	}
	else if (!strcmp (type, "oriq"))
	  ol_q_fscanf (file, (*pTessData).ColData[id][i]);
	else if (!strcmp (type, "oriR"))
	{
	  ol_R_fscanf (file, tmpd);
	  ol_R_q (tmpd, (*pTessData).ColData[id][i]);
	}
	else if (!strcmp (type, "orim"))
	{
	  ol_m_fscanf (file, tmpi);
	  ol_m_q (tmpi, (*pTessData).ColData[id][i]);
	}
	else if (!strcmp (type, "orig"))
	{
	  ol_g_fscanf (file, tmpdd);
	  ol_g_q (tmpdd, (*pTessData).ColData[id][i]);
	}
	else
	  ut_error_reportbug ();
    }
  }
  else if (!strcmp (type, "scal"))
  {
    (*pTessData).ColData[id] = ut_alloc_2d (entityqty + 1, 1);
    ut_array_2d_fscanfn_wcard (value, (*pTessData).ColData[id] + 1,
			       entityqty, 1, "numeral,size");
  }
  else
    abort ();

  return;
}

void
nev_tessdata_fscanf_trs (struct TESSDATA *pTessData,
                         int id, int entityqty,
                         char *type, char *value)
{
  ut_string_string (type, &(*pTessData).trsdatatype[id]);

  if (!strcmp (type, "trs") || !strcmp (type, "scal"))
  {
    (*pTessData).trsdata[id] = ut_alloc_2d (entityqty + 1, 3);
    ut_array_2d_fscanfn_wcard (value, (*pTessData).trsdata[id] + 1,
			       entityqty, 1, "numeral,size");
  }
  else
    abort ();

  return;
}

void
nev_tessdata_fscanf_rad (struct TESSDATA *pTessData,
                         int id, int entityqty,
                         char *type, char *value)
{
  ut_string_string (type, &(*pTessData).RadDataType[id]);

  if (!strcmp (type, "rad") || !strcmp (type, "scal"))
  {
    (*pTessData).RadData[id] = ut_alloc_2d (entityqty + 1, 3);
    ut_array_2d_fscanfn_wcard (value, (*pTessData).RadData[id] + 1,
			       entityqty, 1, "numeral,size");
  }
  else
    abort ();

  return;
}

void
nev_tessdata_fscanf_colscheme (struct TESSDATA *pTessData,
                               int id, char *type)
{
  ut_string_string (type, &(*pTessData).ColScheme[id]);

  return;
}

void
nev_tessdata_fscanf_scale (struct TESSDATA *pTessData,
			   int id, char *type)
{
  ut_string_string (type, &(*pTessData).Scale[id]);

  return;
}

void
nev_tessdata_fscanf_scaletitle (struct TESSDATA *pTessData,
			        int id, char *type)
{
  ut_string_string (type, &(*pTessData).ScaleTitle[id]);

  return;
}
