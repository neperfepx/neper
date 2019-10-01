/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_utils_.h"

int
nev_data_string_entity_type (char *string, char *entity, char *type)
{
  int skip;
  char *stringcpy = ut_alloc_1d_char (strlen (string) + 1);

  // skipping -data if present
  if (strncmp (string, "-data", 5) == 0)
    strcpy (stringcpy, string + 5);
  else
    strcpy (stringcpy, string);

  // setting entity: must be one of:
  // for the mesh: node, elt, elset, elt1d, eltedge
  // for the tess: edge, poly
  skip = -1;
  if (strncmp (stringcpy, "node", 4) == 0)
    strcpy (entity, "node");
  else if (strncmp (stringcpy, "elt3dedge", 9) == 0)
  {
    strcpy (entity, "elt3db");
    skip = 9;
  }
  else if (strncmp (stringcpy, "elt2dedge", 9) == 0)
  {
    strcpy (entity, "elt2db");
    skip = 9;
  }
  else if (strncmp (stringcpy, "elt3d", 5) == 0)
    strcpy (entity, "elt3d");
  else if (strncmp (stringcpy, "elt2d", 5) == 0)
    strcpy (entity, "elt2d");
  else if (strncmp (stringcpy, "elt1d", 5) == 0)
    strcpy (entity, "elt1d");
  else if (strncmp (stringcpy, "elt0d", 5) == 0)
    strcpy (entity, "elt0d");
  else if (strncmp (stringcpy, "elt", 3) == 0)
    strcpy (entity, "elt");
  else if (strncmp (stringcpy, "elset3dedge", 11) == 0)
  {
    strcpy (entity, "elset3db");
    skip = 11;
  }
  else if (strncmp (stringcpy, "elset2dedge", 11) == 0)
  {
    strcpy (entity, "elset2db");
    skip = 11;
  }
  else if (strncmp (stringcpy, "elset3d", 7) == 0)
    strcpy (entity, "elset3d");
  else if (strncmp (stringcpy, "elset2d", 7) == 0)
    strcpy (entity, "elset2d");
  else if (strncmp (stringcpy, "elset1d", 7) == 0)
    strcpy (entity, "elset1d");
  else if (strncmp (stringcpy, "elset0d", 7) == 0)
    strcpy (entity, "elset0d");
  else if (strncmp (stringcpy, "elset", 5) == 0)
    strcpy (entity, "elset");
  else if (strncmp (stringcpy, "ver", 3) == 0)
    strcpy (entity, "ver");
  else if (strncmp (stringcpy, "edge", 4) == 0)
    strcpy (entity, "edge");
  else if (strncmp (stringcpy, "face", 4) == 0)
    strcpy (entity, "face");
  else if (strncmp (stringcpy, "poly", 4) == 0)
    strcpy (entity, "poly");
  else if (strncmp (stringcpy, "seed", 4) == 0)
    strcpy (entity, "seed");
  else if (strncmp (stringcpy, "cell", 4) == 0)
    strcpy (entity, "cell");
  else if (strncmp (stringcpy, "rptedge", 7) == 0)
  {
    ut_print_message (1, 3, "'rpt' is deprecated and will be removed in future versions.  Use 'vox' instead.\n");
    strcpy (entity, "voxedge");
  }
  else if (strncmp (stringcpy, "voxedge", 7) == 0)
    strcpy (entity, "voxedge");
  else if (strncmp (stringcpy, "vox", 3) == 0)
    strcpy (entity, "vox");
  else if (strncmp (stringcpy, "csys", 4) == 0)
    strcpy (entity, "csys");
  else if (strncmp (stringcpy, "point", 5) == 0)
    strcpy (entity, "point");
  else
  {
    ut_print_message (2, 0, "option -data*: entity not supported");
    abort ();
  }

  // setting the type of data
  if (skip == -1)
    skip = strlen (entity);
  strcpy (type, stringcpy + skip);

  // fixing stuff
  if (strncmp (type, "colourscheme", 12) == 0
      || strncmp (type, "colorscheme", 11) == 0)
    sprintf (type, "colscheme");

  ut_free_1d_char (stringcpy);

  return 0;
}

int
nev_data_type_size (char *type, int *psize)
{
  int status = 0;
  if (strcmp (type, "scal") == 0
      || strcmp (type, "int") == 0
      || strcmp (type, "rad") == 0
      || strcmp (type, "edgerad") == 0 || strcmp (type, "real") == 0)
    (*psize) = 1;
  else if (strncmp (type, "ori", 3) == 0
	   || strcmp (type, "edgecol") == 0
	   || strcmp (type, "col") == 0 || strcmp (type, "coo") == 0)
    (*psize) = 3;
  else if (strcmp (type, "cube"))
    (*psize) = 10;
  else if (strcmp (type, "cyl"))
    (*psize) = 5;
  else if (strcmp (type, "ell"))
    (*psize) = 12;
  else if (strncmp (type, "int", 3) == 0 || strncmp (type, "real", 4) == 0)
    sscanf (type + 4, "%d", psize);
  else
    status = -1;

  return status;
}

void
nev_data_id_colour (double **data, int size, int **Col)
{
  int i, j;
  int **palette = NULL;
  int palettesize, pos;
  ut_color_palette_0208 (&palette, &palettesize);

#pragma omp parallel for private(pos,j)
  for (i = 1; i <= size; i++)
  {
    pos = ut_num_d2ri (data[i][0] - 1) % palettesize;
    if (pos >= 0)
      for (j = 0; j < 3; j++)
	Col[i][j] = palette[pos][j];
    else
      ut_array_1d_int_zero (Col[i], 3);
  }

  ut_free_2d_int (palette, palettesize);

  return;
}

void
nev_data_col_colour (double **data, int size, int **Col)
{
  int i, j;

  for (i = 1; i <= size; i++)
    for (j = 0; j < 3; j++)
      Col[i][j] = ut_num_d2ri (data[i][j]);

  return;
}

void
nev_data_tr_tr (double **data, int size, double *trs)
{
  int i;

  for (i = 1; i <= size; i++)
    trs[i] = data[i][0];

  return;
}

void
nev_data_rad_radius (double **data, int size, double *Rad)
{
  int i;

  for (i = 1; i <= size; i++)
    Rad[i] = data[i][0];

  return;
}

void
nev_data_ori_colour (double **data, int size, char *scheme, int **Col)
{
  int i, j;
  double length;

  if (!scheme || !strncmp (scheme, "R", 1))
  {
    if (!scheme || !strcmp (scheme, "R"))
      length = OL_S2 - 1;
    else if (!strncmp (scheme, "R(", 2))
      sscanf (scheme, "R(%lf)", &length);
    else
      ut_error_reportbug ();

#pragma omp parallel for private(j)
    for (i = 1; i <= size; i++)
    {
      double *R = ol_R_alloc ();
      ol_q_R (data[i], R);
      ol_R_Rcrysym (R, "cubic", R);
      for (j = 0; j < 3; j++)
        Col[i][j] = ut_num_bound (ut_num_d2ri (127.5 * (R[j] + length) / length), 0, 255);
      ol_R_free (R);
    }
  }

  else if (!strcmp (scheme, "r"))
  {
#pragma omp parallel for private(j)
    for (i = 1; i <= size; i++)
    {
      double *q = ol_q_alloc ();
      double *r = ol_r_alloc ();
      ol_q_qcrysym (data[i], "cubic", q);
      ol_q_r (q, r);
      for (j = 0; j < 3; j++)
        Col[i][j] = ut_num_bound (ut_num_d2ri (127.5 * (r[j] + 1)), 0, 255);
      ol_q_free (q);
      ol_r_free (r);
    }
  }

  else if (!strncmp (scheme, "theta", 5))
  {
    if (!strcmp (scheme, "theta"))
      length = 1.09606677025243897430; // 62.8 * pi / 180
    else if (!strncmp (scheme, "theta(", 6))
      sscanf (scheme, "theta(%lf)", &length);
    else
      ut_error_reportbug ();

#pragma omp parallel for private(j)
    for (i = 1; i <= size; i++)
    {
      double theta, *q = ol_q_alloc ();
      ol_q_qcrysym (data[i], "cubic", q);
      ol_q_theta (q, &theta);
      for (j = 0; j < 3; j++)
        Col[i][j] = ut_num_bound (ut_num_d2ri (225 * theta / length), 0, 255);
      ol_q_free (q);
    }
  }

  else if (!strncmp (scheme, "rtheta", 5))
  {
    if (!strcmp (scheme, "rtheta"))
      length = 1.09606677025243897430; // 62.8 * pi / 180
    else if (!strncmp (scheme, "rtheta(", 7))
      sscanf (scheme, "rtheta(%lf)", &length);
    else
      ut_error_reportbug ();

#pragma omp parallel for private(i,j)
    for (i = 1; i <= size; i++)
    {
      double theta, *q = ol_q_alloc (), *r = ol_r_alloc ();
      ol_q_qcrysym (data[i], "cubic", q);
      ol_q_rtheta_rad (q, r, &theta);
      for (j = 0; j < 3; j++)
        Col[i][j] = ut_num_bound (ut_num_d2ri (127.5 * (r[j] * theta + length) / length), 0, 255);
      ol_q_free (q);
      ol_r_free (r);
    }
  }

  return;
}

void
nev_data_scal_colour (double **data, int *datadef, int size, char *scale,
		      char *scheme, int **Col, char **pscale)
{
  int i;
  double ScaleBeg, ScaleEnd;
  double datamin, datamax;
  char **parts = NULL;
  int qty;
  char *min = NULL, *max = NULL;
  char *tmp = ut_alloc_1d_char (1000);
  char* format = ut_alloc_1d_char (100);

  qty = 0;
  if (scale)
  {
    ut_string_separate (scale, NEUT_SEP_DEP, &parts, &qty);
    ut_string_format (parts[0], format);
  }
  else
    ut_string_string ("%.3g", &format);

  if (qty >= 2)
  {
    ut_string_string (parts[0], &min);
    ut_string_string (parts[qty - 1], &max);
  }

  if (!datadef)
  {
    datamin = ut_array_2d_col_min (data + 1, 0, size);
    datamax = ut_array_2d_col_max (data + 1, 0, size);
  }
  else
  {
    datamin = DBL_MAX;
    datamax = -DBL_MAX;

    for (i = 1; i <= size; i++)
      if (datadef[i])
      {
	datamin = ut_num_min (datamin, data[i][0]);
	datamax = ut_num_max (datamax, data[i][0]);
      }
  }

  if (min == NULL || strlen (min) == 0 || !strcmp (min, "min"))
    ScaleBeg = datamin;
  else
    ut_string_real (min, &ScaleBeg);

  if (max == NULL || strlen (max) == 0 || !strcmp (max, "max"))
    ScaleEnd = datamax;
  else
    ut_string_real (max, &ScaleEnd);

  if (ut_num_min (ScaleBeg, ScaleEnd) > datamin)
    ut_print_message (1, 2,
		      "Scale minimum (%f) larger than data minimum (%f)\n",
		      ut_num_min (ScaleBeg, ScaleEnd), datamin);

  if (ut_num_max (ScaleBeg, ScaleEnd) < datamax)
    ut_print_message (1, 2,
		      "Scale maximum (%f) smaller than data maximum (%f)\n",
		      ut_num_max (ScaleBeg, ScaleEnd), datamax);

  if (scheme == NULL)
    for (i = 1; i <= size; i++)
      ut_color_scheme_val_color ("blue,cyan,yellow,red",
				 ScaleBeg, ScaleEnd, data[i][0], Col[i]);
  else
    for (i = 1; i <= size; i++)
      ut_color_scheme_val_color (scheme, ScaleBeg, ScaleEnd, data[i][0],
				 Col[i]);

  if (pscale)
  {
    (*pscale) = ut_alloc_1d_char (1000);
    {
      sprintf (*pscale, format, ScaleBeg);
      for (i = 1; i < qty - 1; i++)
      {
	(*pscale) = strcat (*pscale, NEUT_SEP_DEP);
	(*pscale) = strcat (*pscale, parts[i]);
      }

      sprintf (tmp, format, ScaleEnd);
      (*pscale) = strcat (*pscale, NEUT_SEP_DEP);
      (*pscale) = strcat (*pscale, tmp);
    }

    (*pscale) = ut_realloc_1d_char (*pscale, strlen (*pscale) + 1);
  }

  ut_free_2d_char (parts, qty);
  ut_free_1d_char (min);
  ut_free_1d_char (max);
  ut_free_1d_char (tmp);
  ut_free_1d_char (format);

  return;
}

void
nev_data_coo_coo (double **ref, double **data, double fact, int size,
		  double **coo)
{
  int i, j;

  for (i = 1; i <= size; i++)
    for (j = 0; j < 3; j++)
      coo[i][j] = ref[i][j] + fact * (data[i][j] - ref[i][j]);

  return;
}

void
nev_data_disp_coo (double **ref, double **data, double fact, int size,
		   double **coo)
{
  int i, j;

  for (i = 1; i <= size; i++)
    for (j = 0; j < 3; j++)
      coo[i][j] = ref[i][j] + fact * data[i][j];

  return;
}

void
nev_data_typearg_args (char *input, char *argument, char **ptype,
		       char **pvalue, char **pmod)
{
  char **args = NULL;
  int argqty;

  ut_string_separate (argument, NEUT_SEP_DEP, &args, &argqty);

  if (argqty == 1 && !strcmp (argument, "ori"))
  {
    ut_string_string ("ori", ptype);
    ut_free_1d_char_ (pvalue);
  }
  else if (argqty == 1 && !strcmp (argument, "disori"))
  {
    ut_string_string ("disori", ptype);
    ut_free_1d_char_ (pvalue);
  }
  else if (argqty == 1 && !strcmp (argument, "id"))
  {
    ut_string_string ("id", ptype);
    ut_free_1d_char_ (pvalue);
  }
  else if (argqty == 1 && !strncmp (argument, "scaleid", 7))
  {
    ut_string_string (argument, ptype);
    ut_free_1d_char_ (pvalue);
  }
  else if (argqty == 1 && !strcmp (argument, "modeid"))
  {
    ut_string_string (argument, ptype);
    ut_free_1d_char_ (pvalue);
  }
  else if (argqty == 1)
  {
    ut_string_string (input, ptype);
    ut_string_string (args[0], pvalue);
  }
  else if (!isalpha (argument[0]))
    ut_string_string (argument, ptype);
  else if (argqty == 2)
  {
    ut_string_string (args[0], ptype);
    ut_string_string (args[1], pvalue);
  }
  else if (argqty == 3)
  {
    ut_string_string (args[0], ptype);
    ut_string_string (args[1], pvalue);
    ut_string_string (args[2], pmod);
  }

  ut_free_2d_char (args, argqty);

  if (!strcmp (*ptype, "col"))
    return;
  else if (!strcmp (*ptype, "e")
	   || !strcmp (*ptype, "ek")
	   || !strcmp (*ptype, "er")
	   || !strcmp (*ptype, "R")
	   || !strcmp (*ptype, "q")
	   || !strcmp (*ptype, "m")
	   || !strcmp (*ptype, "rtheta")
	   || !strcmp (*ptype, "g"))
  {
    char *tmpc = ut_alloc_1d_char (100);
    tmpc = ut_string_paste ("ori", *ptype);
    ut_string_string (tmpc, ptype);
    ut_free_1d_char (tmpc);
  }
  else if (!strncmp (*ptype, "ori", 3))
    ut_string_string ("ori", ptype);
  // else
  // ut_error_reportbug ();

  if (!strcmp (*ptype, "ori") && *pvalue)
    ut_string_string ("orie", ptype);

  return;
}

void
nev_data_fscanf_ori (char *value, int qty, double **dataembed,
                     double ***pColData, char **pColDataType)
{
  (*pColData) = ut_alloc_2d (qty + 1, 4);

  // ut_string_string ("oriq", pColDataType);

  if (!value)
  {
    if (dataembed)
      ut_array_2d_memcpy (*pColData + 1, qty, 4, dataembed + 1);
    else
      ut_print_message (2, 3, "No orientation data available.\n");
  }

  else
    nev_data_fscanf_ori_file (value, qty, pColData, pColDataType);

  return;
}

void
nev_data_fscanf_ori_tesr (struct TESR Tesr, char *value, int qty, double ****dataembedvox,
                          double **dataembedcell, double ***pColData,
                          char **pColDataType)
{
  int i, j, k, id;

  (*pColData) = ut_alloc_2d (qty + 1, 4);

  // ut_string_string ("oriq", pColDataType);

  if (!value)
  {
    if (dataembedvox)
    {
      id = 0;
      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
            ut_array_1d_memcpy ((*pColData)[++id], 4, dataembedvox[i][j][k]);
    }
    else if (dataembedcell)
    {
      id = 0;
      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
            ut_array_1d_memcpy ((*pColData)[++id], 4, dataembedcell[Tesr.VoxCell[i][j][k]]);
    }
    else
      ut_print_message (2, 3, "No orientation data available.\n");
  }

  else
    nev_data_fscanf_ori_file (value, qty, pColData, pColDataType);

  return;
}
