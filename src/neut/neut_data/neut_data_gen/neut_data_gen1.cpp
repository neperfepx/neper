/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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

// needs improvements
int
neut_data_value_type (struct SIM Sim, char *entity, char *attribute,
                      char *value, char **ptype)
{
  int status = 0;
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  // type "scal" can be an integer, real, of file().

  // depending on the attribute, we should know what to expect
  // should be consistent with the doc
  if ( !strcmp (attribute, "rad"))
    ut_string_string ("scal", ptype);

  else if (!strcmp (attribute, "trs"))
    ut_string_string ("scal", ptype);

  else if (!strcmp (attribute, "weight"))
    ut_string_string ("scal", ptype);

  else if (!strcmp (attribute, "coo"))
    ut_string_string ("coo", ptype);

  else if (!strcmp (attribute, "col"))
  {
    if (ut_color_name_isvalid (value))
      ut_string_string ("col", ptype);
    else if (!strncmp (value, "file(", 5))
      ut_string_string ("col", ptype);
    else
      ut_string_string ("scal", ptype);
  }

  (void) entity;
  (void) Sim;
  /*
  else if (!neut_sim_isvoid (Sim))
  {
    neut_sim_simres (Sim, entity, value, &SimRes);
    if (SimRes.type && strlen (SimRes.type) > 0)
      ut_string_string (SimRes.type, ptype);
  }

  else
    ut_string_string ("scal", ptype);
  */

  neut_simres_free (&SimRes);

  return status;
}

int
neut_data_string_entity_attribute (char *string, char *entity, char **inputs, int inputqty,
                                   char *attribute)
{
  int i;
  char *stringcpy = NULL;

  // skipping -data if present
  ut_string_string (!strncmp (string, "-data", 5) ? string + 5 : string, &stringcpy);

  entity[0] = '\0';

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
  else if (!strncmp (stringcpy, "celledge", 8))
    strcpy (entity, "celledge");
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
    for (i = 0; i < inputqty; i++)
      if (!strncmp (stringcpy, inputs[i], strlen (inputs[i])))
      {
        strcpy (entity, inputs[i]);
        break;
      }

    for (i = 0; i < inputqty; i++)
    {
      char *tmp = ut_string_paste (inputs[i], (char *) "edge");
      if (!strncmp (stringcpy, tmp, strlen (tmp)))
      {
        strcpy (entity, tmp);
        break;
      }
      ut_free_1d_char (&tmp);
    }
  }

  if (strlen (entity) == 0)
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
      || !strcmp (type, "rad") || !strcmp (type, "edgerad"))
    (*psize) = 1;
  else if (!strcmp (type, "edgecol") || !strcmp (type, "col")
           || !strcmp (type, "coo"))
    (*psize) = 3;
  else if (!strcmp (type, "ori"))
    (*psize) = 4;
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
neut_data_real_real (double **data, int size, double *res)
{
  int i;

  for (i = 1; i <= size; i++)
    res[i] = data[i][0];

  return;
}

void
neut_data_ori_color (double **data, int size, char *crysym, char *scheme, int **Col)
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
    neut_data_ori_color_ipf (data, size, crysym, scheme, Col);

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
  int i, qty, valqty;
  double ScaleBeg, ScaleEnd, alpha;
  double datamin, datamax;
  char **parts = NULL;
  char *min = NULL, *max = NULL;
  char *scheme = NULL, *transform = NULL, *fct = NULL, **vals = NULL;

  ut_string_string (scheme_in? scheme_in : NEUT_DEFAULT_COLSCHEME_REAL, &scheme);

  ut_list_break (scheme, NEUT_SEP_DEP, &parts, &qty);

  if (qty == 2)
  {
    ut_string_string (parts[0], &scheme);
    ut_string_string (parts[1], &transform);
  }

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

    double th = 0;
    if (transform)
    {
      ut_string_function (transform, &fct, NULL, &vals, &valqty);

      if (!strcmp (fct, "fade"))
      {
        th = 0.1;
        if (valqty == 1)
          sscanf (vals[0], "%lf", &th);
      }
      else
        ut_print_exprbug (scheme_in);
    }

    for (i = 1; i <= size; i++)
    {
      double val = (data[i][0] - ScaleBeg) / (ScaleEnd - ScaleBeg);
      tinycolormap::Color col = tinycolormap::GetColor(val, type);

      Col[i][0] = ut_num_d2ri (255 * col.r());
      Col[i][1] = ut_num_d2ri (255 * col.g());
      Col[i][2] = ut_num_d2ri (255 * col.b());

      if (th)
      {
        alpha = val / th;

        if (alpha < 1)
        {
          Col[i][0] = ut_num_d2ri (255 * (1 + alpha * (col.r() - 1)));
          Col[i][1] = ut_num_d2ri (255 * (1 + alpha * (col.g() - 1)));
          Col[i][2] = ut_num_d2ri (255 * (1 + alpha * (col.b() - 1)));
        }
      }
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
  ut_free_1d_char (&scheme);
  ut_free_1d_char (&transform);
  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, valqty);

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
  if (!strcmp (attribute, "symbol"))
  {
    ut_string_string ("string", pdatatype);
    ut_string_string (datastring, pdatavalue);
  }

  else if (!strcmp (attribute, "scale")
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
  char *tmp = ut_alloc_1d_char (strlen (colscheme) + 1);

  if (!colscheme)
    return 0;

  ut_string_untilstring (colscheme, NEUT_SEP_DEP, NULL, tmp);

  if (!strcmp (tmp, "Parula")
   || !strcmp (tmp, "Heat")
   || !strcmp (tmp, "Hot")
   || !strcmp (tmp, "Jet")
   || !strcmp (tmp, "Gray")
   || !strcmp (tmp, "Magma")
   || !strcmp (tmp, "Inferno")
   || !strcmp (tmp, "Plasma")
   || !strcmp (tmp, "Viridis")
   || !strcmp (tmp, "Cividis")
   || !strcmp (tmp, "Github")
   || !strcmp (tmp, "parula")
   || !strcmp (tmp, "heat")
   || !strcmp (tmp, "hot")
   || !strcmp (tmp, "jet")
   || !strcmp (tmp, "gray")
   || !strcmp (tmp, "grey")
   || !strcmp (tmp, "magma")
   || !strcmp (tmp, "inferno")
   || !strcmp (tmp, "plasma")
   || !strcmp (tmp, "viridis")
   || !strcmp (tmp, "cividis")
   || !strcmp (tmp, "github"))
   return 1;

  ut_free_1d_char (&tmp);

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
  else if (!strcmp (colscheme, "Viridis") || !strcmp (colscheme, "viridis")
        || !strncmp (colscheme, "wviridis", 8))
    type = tinycolormap::ColormapType::Viridis;
  else if (!strcmp (colscheme, "Cividis") || !strcmp (colscheme, "cividis"))
    type = tinycolormap::ColormapType::Cividis;
  else if (!strcmp (colscheme, "Github") || !strcmp (colscheme, "github"))
    type = tinycolormap::ColormapType::Github;
  else
    abort ();

  return type;
}

void
neut_data_colscheme_asygradient (char *colscheme, int stepqty, char **pasygradient)
{
  int i;
  double **vals = ut_alloc_2d (stepqty + 1, 1);
  int **rgb = ut_alloc_2d_int (stepqty + 1, 3);

  for (i = 1; i <= stepqty; i++)
    vals[i][0] = (double) (i - 1) / (stepqty - 1);

  neut_data_real_color (vals, NULL, stepqty, NULL, colscheme, rgb, NULL);

  (*pasygradient) = ut_alloc_1d_char (stepqty * 100);

  sprintf (*pasygradient, "Gradient(");
  for (i = 1; i <= stepqty; i++)
    sprintf (*pasygradient + strlen (*pasygradient), "rgb(%f,%f,%f)%s",
             rgb[i][0] / 255., rgb[i][1] / 255., rgb[i][2] / 255.,
             i < stepqty ? "," : "");

  sprintf (*pasygradient + strlen (*pasygradient), ")");

  ut_free_2d (&vals, stepqty + 1);
  ut_free_2d_int (&rgb, stepqty + 1);

  return;
}
