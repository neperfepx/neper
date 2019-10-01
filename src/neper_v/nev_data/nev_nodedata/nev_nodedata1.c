/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_nodedata_.h"

void
nev_nodedata_init (struct NODES Nodes, int Qty, struct NODEDATA *pNodeData)
{
  double noderad;

  (*pNodeData).Col = ut_alloc_2d_int ((*pNodeData).NodeQty + 1, 3);
  (*pNodeData).Rad = ut_alloc_1d ((*pNodeData).NodeQty + 1);
  (*pNodeData).Coo = ut_alloc_2d ((*pNodeData).NodeQty + 1, 3);

  noderad = 0.0168 / pow (Qty, 0.25);
  ut_array_1d_set ((*pNodeData).Rad + 1, (*pNodeData).NodeQty, noderad);

  ut_array_2d_int_set ((*pNodeData).Col + 1, (*pNodeData).NodeQty, 3, 0.);
  ut_array_1d_set ((*pNodeData).Rad + 1, (*pNodeData).NodeQty, noderad);

  ut_array_2d_memcpy ((*pNodeData).Coo + 1, (*pNodeData).NodeQty, 3,
		      Nodes.NodeCoo + 1);

  if ((*pNodeData).ColData)
  {
    if ((*pNodeData).ColDataType && !strcmp ((*pNodeData).ColDataType, "id"))
      nev_data_id_colour ((*pNodeData).ColData, (*pNodeData).NodeQty,
			  (*pNodeData).Col);
    else if ((*pNodeData).ColDataType
	     && !strcmp ((*pNodeData).ColDataType, "col"))
      nev_data_col_colour ((*pNodeData).ColData, (*pNodeData).NodeQty,
			   (*pNodeData).Col);
    else if ((*pNodeData).ColDataType
	     && !strncmp ((*pNodeData).ColDataType, "ori", 3))
      nev_data_ori_colour ((*pNodeData).ColData, (*pNodeData).NodeQty,
			   (*pNodeData).ColScheme, (*pNodeData).Col);
    else if ((*pNodeData).ColDataType
	     && !strcmp ((*pNodeData).ColDataType, "scal"))
      nev_data_scal_colour ((*pNodeData).ColData, NULL,
			    (*pNodeData).NodeQty,
			    (*pNodeData).Scale,
			    (*pNodeData).ColScheme,
			    (*pNodeData).Col, &((*pNodeData).Scale));

    else
    {
      printf ("(*pNodeData).ColDataType = %s\n", (*pNodeData).ColDataType);
      ut_error_reportbug ();
    }
  }

  if ((*pNodeData).RadData)
    nev_data_rad_radius ((*pNodeData).RadData, (*pNodeData).NodeQty,
			 (*pNodeData).Rad);

  if ((*pNodeData).CooDataType)
  {
    if (!strcmp ((*pNodeData).CooDataType, "coo"))
      nev_data_coo_coo (Nodes.NodeCoo, (*pNodeData).CooData,
			(*pNodeData).CooFact, (*pNodeData).NodeQty,
			(*pNodeData).Coo);
    else if (!strcmp ((*pNodeData).CooDataType, "disp"))
      nev_data_disp_coo (Nodes.NodeCoo, (*pNodeData).CooData,
			 (*pNodeData).CooFact, (*pNodeData).NodeQty,
			 (*pNodeData).Coo);
    else
      abort ();
  }

  return;
}

void
nev_nodedata_fscanf (char *type, char *argument, struct NODEDATA *pNodeData)
{
  char **args = NULL;
  int i, argqty;
  char *value = NULL;

  ut_string_separate (argument, NEUT_SEP_DEP, &args, &argqty);

  if (!strcmp (type, "col"))
  {
    nev_data_typearg_args ("col", argument, &(*pNodeData).ColDataType,
			   &value, NULL);

    if (!strcmp ((*pNodeData).ColDataType, "id"))
    {
      (*pNodeData).ColData = ut_alloc_2d ((*pNodeData).NodeQty + 1, 3);

      for (i = 1; i <= (*pNodeData).NodeQty; i++)
	(*pNodeData).ColData[i][0] = i;
    }
    else if (!strcmp ((*pNodeData).ColDataType, "col"))
    {
      (*pNodeData).ColData = ut_alloc_2d ((*pNodeData).NodeQty + 1, 3);
      ut_array_2d_fscanfn_wcard (value, (*pNodeData).ColData + 1,
				 (*pNodeData).NodeQty, 3, "colour,size");
    }
    else if (!strncmp ((*pNodeData).ColDataType, "ori", 3))
      nev_data_fscanf_ori (value, (*pNodeData).NodeQty, NULL,
                           &(*pNodeData).ColData, &(*pNodeData).ColDataType);
    else if (!strcmp ((*pNodeData).ColDataType, "scal"))
    {
      (*pNodeData).ColData = ut_alloc_2d ((*pNodeData).NodeQty + 1, 1);
      ut_array_2d_fscanfn_wcard (value, (*pNodeData).ColData + 1,
				 (*pNodeData).NodeQty, 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (type, "rad"))
  {
    (*pNodeData).RadDataType = ut_alloc_1d_char (100);

    if (argqty == 1)
      strcpy ((*pNodeData).RadDataType, type);
    else
      strcpy ((*pNodeData).RadDataType, args[0]);

    if (!strcmp ((*pNodeData).RadDataType, "rad"))
    {
      (*pNodeData).RadData = ut_alloc_2d ((*pNodeData).NodeQty + 1, 3);
      ut_array_2d_fscanfn_wcard (args[0], (*pNodeData).RadData + 1,
				 (*pNodeData).NodeQty, 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (type, "colscheme"))
  {
    (*pNodeData).ColScheme = ut_alloc_1d_char (strlen (argument) + 1);
    strcpy ((*pNodeData).ColScheme, argument);
  }
  else if (!strcmp (type, "scale"))
  {
    (*pNodeData).Scale = ut_alloc_1d_char (strlen (argument) + 1);
    strcpy ((*pNodeData).Scale, argument);
  }
  else if (!strcmp (type, "scaletitle"))
  {
    (*pNodeData).ScaleTitle = ut_alloc_1d_char (strlen (argument) + 1);
    strcpy ((*pNodeData).ScaleTitle, argument);
  }
  else if (!strcmp (type, "coo"))
  {
    if (argqty == 1)
    {
      (*pNodeData).CooDataType = ut_alloc_1d_char (strlen (type) + 1);
      strcpy ((*pNodeData).CooDataType, type);
    }
    else
    {
      (*pNodeData).CooDataType = ut_alloc_1d_char (strlen (args[0]) + 1);
      strcpy ((*pNodeData).CooDataType, args[0]);
    }

    if ((*pNodeData).CooData == NULL)
      (*pNodeData).CooData = ut_alloc_2d ((*pNodeData).NodeQty + 1, 3);

    if (!strcmp ((*pNodeData).CooDataType, "coo"))
      ut_array_2d_fscanfn_wcard (args[0], (*pNodeData).CooData + 1,
				 (*pNodeData).NodeQty, 3, "size");
    else if (!strcmp ((*pNodeData).CooDataType, "disp"))
      ut_array_2d_fscanfn_wcard (args[1], (*pNodeData).CooData + 1,
				 (*pNodeData).NodeQty, 3, "size");
    else
      abort ();
  }
  else if (!strcmp (type, "coofact"))
    sscanf (args[0], "%lf", &((*pNodeData).CooFact));
  else
    ut_error_reportbug ();

  ut_free_1d_char (value);

  return;
}
