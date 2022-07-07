/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_data_gen_.hpp"

void
neut_data_coldatatype_size (struct DATA Data, int *psize)
{
  if (!strcmp (Data.ColDataType, "int"))
    (*psize) = 1;
  else if (!strcmp (Data.ColDataType, "real"))
    (*psize) = 1;
  else if (!strcmp (Data.ColDataType, "col"))
    (*psize) = 3;
  // for ori, we are talking about coldata (not col), which contains quaternions
  else if (!strcmp (Data.ColDataType, "ori"))
    (*psize) = 4;
  else
    (*psize) = -1;

  return;
}

int
neut_data_value_type (struct SIM Sim, char *entity, char *attribute,
                      char *value, char **ptype)
{
  int status = 0;
  double **g = ol_g_alloc ();
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  if (ut_color_name_isvalid (value))
    ut_string_string ("col", ptype);

  else if (!ol_label_g (value, g))
    ut_string_string ("ori", ptype);

  else if (!strncmp (value, "file(", 5))
  {
    if (!strcmp (attribute, "coo"))
      ut_string_string ("coo", ptype);
    else
      status = -1;
  }

  else if (!neut_sim_isvoid (Sim))
  {
    neut_sim_simres (Sim, entity, value, &SimRes);
    if (SimRes.type && strlen (SimRes.type) > 0)
      ut_string_string (SimRes.type, ptype);
  }

  else
    ut_string_string ("expr", ptype);

  ol_g_free (g);

  neut_simres_free (&SimRes);

  return status;
}

int
neut_data_string_entity_attribute (char *string, char *entity, char *attribute)
{
  char *stringcpy = NULL;

  // skipping -data if present
  ut_string_string (!strncmp (string, "-data", 5) ? string + 5 : string, &stringcpy);

  // setting entity: must be one of:
  // for the mesh: node, elt, elset, elt1d, eltedge
  // for the tess: edge, poly
  if (!strncmp (stringcpy, "node", 4))
    strcpy (entity, "node");
  else if (!strncmp (stringcpy, "elt3dedge", 9))
    strcpy (entity, "elt3dedge");
  else if (!strncmp (stringcpy, "elt2dedge", 9))
    strcpy (entity, "elt2dedge");
  else if (!strncmp (stringcpy, "elt3d", 5))
    strcpy (entity, "elt3d");
  else if (!strncmp (stringcpy, "elt2d", 5))
    strcpy (entity, "elt2d");
  else if (!strncmp (stringcpy, "elt1d", 5))
    strcpy (entity, "elt1d");
  else if (!strncmp (stringcpy, "elt0d", 5))
    strcpy (entity, "elt0d");
  else if (!strncmp (stringcpy, "elt", 3))
    strcpy (entity, "elt");
  else if (!strncmp (stringcpy, "elset3dedge", 11))
    strcpy (entity, "elset3dedge");
  else if (!strncmp (stringcpy, "elset2dedge", 11))
    strcpy (entity, "elset2dedge");
  else if (!strncmp (stringcpy, "elset3d", 7))
    strcpy (entity, "elset3d");
  else if (!strncmp (stringcpy, "elset2d", 7))
    strcpy (entity, "elset2d");
  else if (!strncmp (stringcpy, "elset1d", 7))
    strcpy (entity, "elset1d");
  else if (!strncmp (stringcpy, "elset0d", 7))
    strcpy (entity, "elset0d");
  else if (!strncmp (stringcpy, "elset", 5))
    strcpy (entity, "elset");
  else if (!strncmp (stringcpy, "ver", 3))
    strcpy (entity, "ver");
  else if (!strncmp (stringcpy, "edge", 4))
    strcpy (entity, "edge");
  else if (!strncmp (stringcpy, "face", 4))
    strcpy (entity, "face");
  else if (!strncmp (stringcpy, "poly", 4))
    strcpy (entity, "poly");
  else if (!strncmp (stringcpy, "seed", 4))
    strcpy (entity, "seed");
  else if (!strncmp (stringcpy, "cell", 4))
    strcpy (entity, "cell");
  else if (!strncmp (stringcpy, "voxedge", 7))
    strcpy (entity, "voxedge");
  else if (!strncmp (stringcpy, "vox", 3))
    strcpy (entity, "vox");
  else if (!strncmp (stringcpy, "voidvox", 7))
    strcpy (entity, "voidvox");
  else if (!strncmp (stringcpy, "csys", 4))
    strcpy (entity, "csys");
  else if (!strncmp (stringcpy, "pointedge", 9))
    strcpy (entity, "pointedge");
  else if (!strncmp (stringcpy, "point", 5))
    strcpy (entity, "point");
  else if (!strncmp (stringcpy, "crystaledge", 9))
    strcpy (entity, "crystaledge");
  else if (!strncmp (stringcpy, "crystal", 5))
    strcpy (entity, "crystal");
  else if (!strncmp (stringcpy, "mesh", 4))
    strcpy (entity, "mesh");
  else
  {
    ut_print_message (2, 0, "option -data*: entity not supported");
    abort ();
  }

  // setting the attribute of data
  strcpy (attribute, stringcpy + strlen (entity));

  // fixing stuff
  if (!strncmp (attribute, "colorscheme", 12)
      || !strncmp (attribute, "colorscheme", 11))
    sprintf (attribute, "colscheme");

  ut_free_1d_char (&stringcpy);

  return 0;
}

int
neut_data_type_size (char *type, int *psize)
{
  int status = 0;
  if (!strcmp (type, "real") || !strcmp (type, "int")
      || !strcmp (type, "rad") || !strcmp (type, "edgerad")
      || !strcmp (type, "real"))
    (*psize) = 1;
  else if (!strcmp (type, "edgecol") || !strcmp (type, "col")
           || !strcmp (type, "coo"))
    (*psize) = 3;
  else if (!strcmp (type, "ori"))
    (*psize) = 4;
  else if (strcmp (type, "cube"))
    (*psize) = 10;
  else if (strcmp (type, "cyl"))
    (*psize) = 5;
  else if (strcmp (type, "ell"))
    (*psize) = 12;
  else if (!strncmp (type, "int", 3) || !strncmp (type, "real", 4))
    sscanf (type + 4, "%d", psize);
  else
    status = -1;

  return status;
}

void
neut_data_int_color (double **data, int size, int *UndefCol, int **Col)
{
  int i, palettesize, **palette = NULL;
  ut_color_palette_0208 (&palette, &palettesize);

#pragma omp parallel for private(i)
  for (i = 1; i <= size; i++)
  {
    int j, pos;
    pos = ut_num_d2ri (data[i][0] - 1) % palettesize;
    if (pos >= 0)
      for (j = 0; j < 3; j++)
        Col[i][j] = palette[pos][j];
    else if (UndefCol)
      ut_array_1d_int_memcpy (UndefCol, 3, Col[i]);
    else
      ut_array_1d_int_set (Col[i], 3, 128);
  }

  ut_free_2d_int (&palette, palettesize);

  return;
}

void
neut_data_col_color (double **data, int size, int **Col)
{
  int i, j;

  for (i = 1; i <= size; i++)
    for (j = 0; j < 3; j++)
      Col[i][j] = ut_num_d2ri (data[i][j]);

  return;
}

void
neut_data_tr_tr (double **data, int size, double *trs)
{
  int i;

  for (i = 1; i <= size; i++)
    trs[i] = data[i][0];

  return;
}

void
neut_data_rad_radius (double **data, int size, double *Rad)
{
  int i;

  for (i = 1; i <= size; i++)
    Rad[i] = data[i][0];

  return;
}

void
neut_data_ori_color (double **data, int size, char *scheme, int **Col)
{
  int varqty;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (scheme ? scheme : (char *) "rodrigues", &fct, &vars, &vals, &varqty);

  if (!strcmp (fct, "rodrigues"))
    neut_data_ori_color_rodrigues (data, size, scheme, Col);

  else if (!strcmp (fct, "axis"))
    neut_data_ori_color_axis (data, size, scheme, Col);

  else if (!strcmp (fct, "angle"))
    neut_data_ori_color_angle (data, size, scheme, Col);

  else if (!strncmp (fct, "axis-angle", 10))
    neut_data_ori_color_axisangle (data, size, scheme, Col);

  else if (!strcmp (fct, "ipf"))
    neut_data_ori_color_ipf (data, size, scheme, Col);

  else
    ut_print_exprbug (fct);

  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);

  return;
}

void
neut_data_real_color (double **data, int *datadef, int size, char *scale,
                      char *scheme_in, int **Col, char **pscale)
{
  int i;
  double ScaleBeg, ScaleEnd;
  double datamin, datamax;
  char **parts = NULL;
  int qty;
  char *min = NULL, *max = NULL;
  char *tmp = ut_alloc_1d_char (1000);
  char *scheme = NULL;

  ut_string_string (scheme_in? scheme_in : NEUT_DEFAULT_COLSCHEME_REAL, &scheme);

  qty = 0;
  if (scale)
    ut_list_break (scale, NEUT_SEP_DEP, &parts, &qty);

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

  if (neut_data_colscheme_istinycolormap (scheme))
  {
    tinycolormap::ColormapType type =
      neut_data_colscheme_tinycolormaptype (scheme);

    for (i = 1; i <= size; i++)
    {
      double val = (data[i][0] - ScaleBeg) / (ScaleEnd - ScaleBeg);
      tinycolormap::Color col = tinycolormap::GetColor(val, type);
      Col[i][0] = ut_num_d2ri (255 * col.r());
      Col[i][1] = ut_num_d2ri (255 * col.g());
      Col[i][2] = ut_num_d2ri (255 * col.b());
    }
  }

  else
    for (i = 1; i <= size; i++)
      ut_color_bar_val_color (scheme, ScaleBeg, ScaleEnd, data[i][0], Col[i]);

  if (pscale)
  {
    (*pscale) = ut_alloc_1d_char (1000);
    if (qty >= 2)
    {
      strcpy (*pscale, parts[0]);
      for (i = 1; i < qty - 1; i++)
      {
        (*pscale) = strcat (*pscale, NEUT_SEP_DEP);
        (*pscale) = strcat (*pscale, parts[i]);
      }
      (*pscale) = strcat (*pscale, NEUT_SEP_DEP);
      (*pscale) = strcat (*pscale, parts[qty - 1]);
    }
    else
      sprintf (*pscale, "%f%s%f", ScaleBeg, NEUT_SEP_DEP, ScaleEnd);

    (*pscale) = ut_realloc_1d_char (*pscale, strlen (*pscale) + 1);
  }

  ut_free_2d_char (&parts, qty);
  ut_free_1d_char (&min);
  ut_free_1d_char (&max);
  ut_free_1d_char (&tmp);
  ut_free_1d_char (&scheme);

  return;
}

void
neut_data_coo_coo (double **ref, double **data, double fact, int size,
                  double **coo)
{
  int i, j;

  for (i = 1; i <= size; i++)
    for (j = 0; j < 3; j++)
      coo[i][j] = ref[i][j] + fact * (data[i][j] - ref[i][j]);

  return;
}

void
neut_data_disp_coo (double **ref, double **data, double fact, int size,
                   double **coo)
{
  int i, j;

  for (i = 1; i <= size; i++)
    for (j = 0; j < 3; j++)
      coo[i][j] = ref[i][j] + fact * data[i][j];

  return;
}

void
neut_data_datastring_type_value (char *entity, char *attribute,
                                 char *datastring, char **pdatatype,
                                 char **pdatavalue)
{
  if (!strcmp (attribute, "scale")
   || !strcmp (attribute, "scaletitle")
   || !strcmp (attribute, "colscheme")
   || !strcmp (attribute, "label"))
  {
    ut_string_string ("none", pdatatype);
    ut_string_string (datastring, pdatavalue);
  }

  else if (!strcmp (entity, "csys")
        && !strcmp (attribute, "coo"))
  {
    ut_string_string ("none", pdatatype);
    ut_string_string (datastring, pdatavalue);
  }

  else
  {
    int argqty;
    char **args = NULL;

    ut_list_break (datastring, NEUT_SEP_DEP, &args, &argqty);
    // patch to let colors provided as <R_value>:<G_value>:<B_value> slide
    if (isdigit (args[0][0]))
      ut_list_break (datastring, "fdsk,fdslkf,dsl", &args, &argqty);

    if (argqty == 1)
    {
      if (!strcmp (datastring, "ori"))
      {
        ut_string_string ("ori", pdatatype);
        ut_string_string ("internal", pdatavalue);
      }
      else if (!strcmp (datastring, "disori"))
      {
        ut_string_string ("disori", pdatatype);
        ut_string_string ("internal", pdatavalue);
      }
      else if (!strcmp (datastring, "coo") && !strncmp (datastring, "file(", 5))
      {
        ut_string_string ("coo", pdatatype);
        ut_string_string (datastring, pdatavalue);
      }
      else
      {
        ut_string_string ("none", pdatatype);
        ut_string_string (args[0], pdatavalue);
      }
    }

    else if (argqty == 2)
    {
      ut_string_string (args[0], pdatatype);
      ut_string_string (args[1], pdatavalue);
    }

    ut_free_2d_char (&args, argqty);
  }

  return;
}

int
neut_data_colscheme_istinycolormap (char *colscheme)
{
  if (!colscheme)
    return 0;

  if (!strcmp (colscheme, "Parula")
   || !strcmp (colscheme, "Heat")
   || !strcmp (colscheme, "Hot")
   || !strcmp (colscheme, "Jet")
   || !strcmp (colscheme, "Gray")
   || !strcmp (colscheme, "Magma")
   || !strcmp (colscheme, "Inferno")
   || !strcmp (colscheme, "Plasma")
   || !strcmp (colscheme, "Viridis")
   || !strcmp (colscheme, "Cividis")
   || !strcmp (colscheme, "Github")
   || !strcmp (colscheme, "parula")
   || !strcmp (colscheme, "heat")
   || !strcmp (colscheme, "hot")
   || !strcmp (colscheme, "jet")
   || !strcmp (colscheme, "gray")
   || !strcmp (colscheme, "grey")
   || !strcmp (colscheme, "magma")
   || !strcmp (colscheme, "inferno")
   || !strcmp (colscheme, "plasma")
   || !strcmp (colscheme, "viridis")
   || !strcmp (colscheme, "cividis")
   || !strcmp (colscheme, "github"))
   return 1;

  return 0;
}

tinycolormap::ColormapType
neut_data_colscheme_tinycolormaptype (char *colscheme)
{
  tinycolormap::ColormapType type;

  if (!strcmp (colscheme, "Parula") || !strcmp (colscheme, "parula"))
    type = tinycolormap::ColormapType::Parula;
  else if (!strcmp (colscheme, "Heat") || !strcmp (colscheme, "heat"))
    type = tinycolormap::ColormapType::Heat;
  else if (!strcmp (colscheme, "Hot") || !strcmp (colscheme, "hot"))
    type = tinycolormap::ColormapType::Hot;
  else if (!strcmp (colscheme, "Jet") || !strcmp (colscheme, "jet"))
    type = tinycolormap::ColormapType::Jet;
  else if (!strcmp (colscheme, "Gray") || !strcmp (colscheme, "gray")
        || !strcmp (colscheme, "Grey") || !strcmp (colscheme, "grey"))
    type = tinycolormap::ColormapType::Gray;
  else if (!strcmp (colscheme, "Magma") || !strcmp (colscheme, "magma"))
    type = tinycolormap::ColormapType::Magma;
  else if (!strcmp (colscheme, "Inferno") || !strcmp (colscheme, "inferno"))
    type = tinycolormap::ColormapType::Inferno;
  else if (!strcmp (colscheme, "Plasma") || !strcmp (colscheme, "plasma"))
    type = tinycolormap::ColormapType::Plasma;
  else if (!strcmp (colscheme, "Viridis") || !strcmp (colscheme, "viridis"))
    type = tinycolormap::ColormapType::Viridis;
  else if (!strcmp (colscheme, "Cividis") || !strcmp (colscheme, "cividis"))
    type = tinycolormap::ColormapType::Cividis;
  else if (!strcmp (colscheme, "Github") || !strcmp (colscheme, "github"))
    type = tinycolormap::ColormapType::Github;
  else
    abort ();

  return type;
}
