/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_pointdata_.h"

void
nev_pointdata_init (struct POINT Point, struct POINTDATA *pPointData)
{
  double pointrad;

  (*pPointData).Col = ut_alloc_2d_int ((*pPointData).PointQty + 1, 3);
  (*pPointData).Rad = ut_alloc_1d ((*pPointData).PointQty + 1);
  (*pPointData).trs = ut_alloc_1d ((*pPointData).PointQty + 1);
  (*pPointData).Coo = ut_alloc_2d ((*pPointData).PointQty + 1, 3);

  pointrad = 0.0168 / pow ((*pPointData).PointQty, 0.25);
  ut_array_1d_set ((*pPointData).Rad + 1, (*pPointData).PointQty, pointrad);
  ut_array_2d_int_set ((*pPointData).Col + 1, (*pPointData).PointQty, 3, 128);
  ut_array_2d_memcpy ((*pPointData).Coo + 1, (*pPointData).PointQty, 3,
		      Point.PointCoo + 1);

  if ((*pPointData).ColData)
  {
    if ((*pPointData).ColDataType
	&& !strcmp ((*pPointData).ColDataType, "id"))
      nev_data_id_colour ((*pPointData).ColData, (*pPointData).PointQty,
			  (*pPointData).Col);
    else if ((*pPointData).ColDataType
	     && !strcmp ((*pPointData).ColDataType, "col"))
      nev_data_col_colour ((*pPointData).ColData, (*pPointData).PointQty,
			   (*pPointData).Col);
    else if ((*pPointData).ColDataType
	     && !strncmp ((*pPointData).ColDataType, "ori", 3))
      nev_data_ori_colour ((*pPointData).ColData, (*pPointData).PointQty,
			   (*pPointData).ColScheme, (*pPointData).Col);
    else if ((*pPointData).ColDataType
	     && !strcmp ((*pPointData).ColDataType, "scal"))
      nev_data_scal_colour ((*pPointData).ColData, NULL,
			    (*pPointData).PointQty,
			    (*pPointData).Scale,
			    (*pPointData).ColScheme,
			    (*pPointData).Col, &((*pPointData).Scale));

    else
    {
      printf ("(*pPointData).ColDataType = %s\n", (*pPointData).ColDataType);
      ut_error_reportbug ();
    }
  }

  if (!(*pPointData).RadDataType)
    ut_string_string ("rad", &((*pPointData).RadDataType));
  if ((*pPointData).RadData)
    nev_data_rad_radius ((*pPointData).RadData, (*pPointData).PointQty,
			 (*pPointData).Rad);

  if ((*pPointData).trsdata)
    nev_data_tr_tr ((*pPointData).trsdata, (*pPointData).PointQty,
		    (*pPointData).trs);

  if ((*pPointData).CooDataType)
  {
    if (!strcmp ((*pPointData).CooDataType, "coo"))
      nev_data_coo_coo (Point.PointCoo, (*pPointData).CooData,
			(*pPointData).CooFact, (*pPointData).PointQty,
			(*pPointData).Coo);
    else if (!strcmp ((*pPointData).CooDataType, "disp"))
      nev_data_disp_coo (Point.PointCoo, (*pPointData).CooData,
			 (*pPointData).CooFact, (*pPointData).PointQty,
			 (*pPointData).Coo);
    else
      abort ();
  }

  return;
}

void
nev_pointdata_fscanf (char *type, char *argument,
		      struct POINTDATA *pPointData)
{
  char **args = NULL;
  int i, argqty;
  char *value = NULL, *mod = NULL;

  ut_string_separate (argument, NEUT_SEP_DEP, &args, &argqty);

  if (!strcmp (type, "col"))
  {
    nev_data_typearg_args ("col", argument, &(*pPointData).ColDataType,
			   &value, &mod);

    if (!strcmp ((*pPointData).ColDataType, "id"))
    {
      (*pPointData).ColData = ut_alloc_2d ((*pPointData).PointQty + 1, 3);

      if (!value)
	for (i = 1; i <= (*pPointData).PointQty; i++)
	  (*pPointData).ColData[i][0] = i;
      else
	ut_array_2d_fscanfn_wcard (value, (*pPointData).ColData + 1,
				   (*pPointData).PointQty, 1, "numeral");
    }
    else if (!strcmp ((*pPointData).ColDataType, "col"))
    {
      (*pPointData).ColData = ut_alloc_2d ((*pPointData).PointQty + 1, 3);
      ut_array_2d_fscanfn_wcard (value, (*pPointData).ColData + 1,
				 (*pPointData).PointQty, 3, "colour,size");
    }
    else if (!strncmp ((*pPointData).ColDataType, "ori", 3))
      nev_data_fscanf_ori (value, (*pPointData).PointQty, NULL,
                           &(*pPointData).ColData, &(*pPointData).ColDataType);
    else if (!strcmp ((*pPointData).ColDataType, "scal"))
    {
      (*pPointData).ColData = ut_alloc_2d ((*pPointData).PointQty + 1, 1);
      ut_array_2d_fscanfn_wcard (value, (*pPointData).ColData + 1,
				 (*pPointData).PointQty, 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (type, "trs"))
  {
    (*pPointData).trsdatatype = ut_alloc_1d_char (100);

    if (argqty == 1)
      strcpy ((*pPointData).trsdatatype, type);
    else
      strcpy ((*pPointData).trsdatatype, args[0]);

    if (!strcmp ((*pPointData).trsdatatype, "trs"))
    {
      (*pPointData).trsdata = ut_alloc_2d ((*pPointData).PointQty + 1, 3);
      ut_array_2d_fscanfn_wcard (args[0], (*pPointData).trsdata + 1,
				 (*pPointData).PointQty, 1, "numeral,size");
    }
    else if (!strcmp ((*pPointData).trsdatatype, "scal"))
    {
      (*pPointData).trsdata = ut_alloc_2d ((*pPointData).PointQty + 1, 3);
      ut_array_2d_fscanfn_wcard (args[0] + 5, (*pPointData).trsdata + 1,
				 (*pPointData).PointQty, 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (type, "rad"))
  {
    nev_data_typearg_args ("rad", argument, &(*pPointData).RadDataType,
			   &value, &mod);
    if (mod)
      ut_string_string (mod, &((*pPointData).Space));

    if (!strcmp ((*pPointData).RadDataType, "rad"))
    {
      (*pPointData).RadData = ut_alloc_2d ((*pPointData).PointQty + 1, 3);
      ut_array_2d_fscanfn_wcard (value, (*pPointData).RadData + 1,
				 (*pPointData).PointQty, 1, "numeral,size");
    }
    else if (!strcmp ((*pPointData).RadDataType, "cube"))
    {
      (*pPointData).RadData = ut_alloc_2d ((*pPointData).PointQty + 1, 10);
      ut_array_2d_fscanfn_wcard (value, (*pPointData).RadData + 1,
				 (*pPointData).PointQty, 10, "numeral,size");
    }
    else if (!strcmp ((*pPointData).RadDataType, "cyl"))
    {
      (*pPointData).RadData = ut_alloc_2d ((*pPointData).PointQty + 1, 5);
      ut_array_2d_fscanfn_wcard (value, (*pPointData).RadData + 1,
				 (*pPointData).PointQty, 5, "numeral,size");
    }
    else if (!strcmp ((*pPointData).RadDataType, "arr"))
    {
      (*pPointData).RadData = ut_alloc_2d ((*pPointData).PointQty + 1, 5);
      ut_array_2d_fscanfn_wcard (value, (*pPointData).RadData + 1,
				 (*pPointData).PointQty, 5, "numeral,size");
    }
    else if (!strcmp ((*pPointData).RadDataType, "tor"))
    {
      (*pPointData).RadData = ut_alloc_2d ((*pPointData).PointQty + 1, 5);
      ut_array_2d_fscanfn_wcard (value, (*pPointData).RadData + 1,
				 (*pPointData).PointQty, 5, "numeral,size");
    }
    else if (!strcmp ((*pPointData).RadDataType, "disc"))
    {
      (*pPointData).RadData = ut_alloc_2d ((*pPointData).PointQty + 1, 4);
      ut_array_2d_fscanfn_wcard (value, (*pPointData).RadData + 1,
				 (*pPointData).PointQty, 4, "numeral,size");
    }
    else if (!strcmp ((*pPointData).RadDataType, "ell"))
    {
      (*pPointData).RadData = ut_alloc_2d ((*pPointData).PointQty + 1, 12);
      ut_array_2d_fscanfn_wcard (value, (*pPointData).RadData + 1,
				 (*pPointData).PointQty, 12, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (type, "colscheme"))
  {
    (*pPointData).ColScheme = ut_alloc_1d_char (strlen (argument) + 1);
    strcpy ((*pPointData).ColScheme, argument);
  }
  else if (!strcmp (type, "scale"))
  {
    (*pPointData).Scale = ut_alloc_1d_char (strlen (argument) + 1);
    strcpy ((*pPointData).Scale, argument);
  }
  else if (!strcmp (type, "scaletitle"))
  {
    (*pPointData).ScaleTitle = ut_alloc_1d_char (strlen (argument) + 1);
    strcpy ((*pPointData).ScaleTitle, argument);
  }
  else if (!strcmp (type, "coo"))
  {
    if (argqty == 1)
    {
      (*pPointData).CooDataType = ut_alloc_1d_char (strlen (type) + 1);
      strcpy ((*pPointData).CooDataType, type);
    }
    else
    {
      (*pPointData).CooDataType = ut_alloc_1d_char (strlen (args[0]) + 1);
      strcpy ((*pPointData).CooDataType, args[0]);
    }

    if ((*pPointData).CooData == NULL)
      (*pPointData).CooData = ut_alloc_2d ((*pPointData).PointQty + 1, 3);

    if (!strcmp ((*pPointData).CooDataType, "coo"))
      ut_array_2d_fscanfn_wcard (args[0], (*pPointData).CooData + 1,
				 (*pPointData).PointQty, 3, "size");
    else if (!strcmp ((*pPointData).CooDataType, "disp"))
      ut_array_2d_fscanfn_wcard (args[1], (*pPointData).CooData + 1,
				 (*pPointData).PointQty, 3, "size");
    else
      abort ();
  }
  else if (!strcmp (type, "coofact"))
    sscanf (args[0], "%lf", &((*pPointData).CooFact));
  else
    ut_error_reportbug ();

  ut_free_1d_char (value);
  ut_free_1d_char (mod);

  return;
}
