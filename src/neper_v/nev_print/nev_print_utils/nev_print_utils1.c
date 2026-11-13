/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_utils_.h"

void
nev_print_symbol_expr_params (double rad, char *expr, char *symbol, double *params)
{
  char *fct = NULL;

  ut_string_function (expr, &fct, NULL, NULL, NULL);
  strcpy (symbol, fct);

  if (!strcmp (fct, "cube"))
    nev_print_cube_expr_params (rad, expr, params);
  else if (!strcmp (fct, "cylinder"))
    nev_print_cyl_expr_params (rad, expr, params);
  else if (!strcmp (fct, "arrow"))
    nev_print_arr_expr_params (rad, expr, params);
  else if (!strcmp (fct, "torus"))
    nev_print_tor_expr_params (rad, expr, params);
  else if (!strcmp (fct, "circle") || !strcmp (fct, "disc"))
    nev_print_disc_expr_params (rad, expr, params);
  else if (!strcmp (fct, "ellipsoid"))
    nev_print_ellipsoid_expr_params (rad, expr, params);
  else // sphere and others
    nev_print_generic_expr_params (rad, expr, params);

  ut_free_1d_char (&fct);

  return;
}

void
nev_print_cube_expr_params (double rad, char *expr, double *params)
{
  int i, varqty = 0;
  double **g = ol_g_alloc ();
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ol_g_set_id (g);

  ut_string_function (expr, &fct, &vars, &vals, &varqty);

  params[0] = rad;

  for (i = 0; i < varqty; i++)
  {
    if (!vars[i])
      ut_print_exprbug (expr);
    else if (!strcmp (vars[i], "rad"))
      params[0] = atof (vals[i]);
    else if (!strcmp (vars[i], "ori") || !strcmp (vars[i], "theta"))
      ol_label_g (vals[i], g);
    else
      abort ();
  }

  for (i = 0; i < 3; i++)
    ut_array_1d_memcpy (g[i], 3, params + 1 + 3 * i);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ol_g_free (&g);

  return;
}

void
nev_print_generic_expr_params (double rad, char *expr, double *params)
{
  int i;
  int varqty = 0;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  params[0] = rad;

  ut_string_function (expr, &fct, &vars, &vals, &varqty);

  for (i = 0; i < varqty; i++)
  {
    if (!vars[i])
      ut_print_exprbug (expr);
    else if (!strcmp (vars[i], "rad"))
      params[0] = atof (vals[i]);
    else if (!strcmp (vars[i], "theta"))
      params[1] = atof (vals[i]);
    else
      abort ();
  }

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);

  return;
}

void
nev_print_cyl_expr_params (double rad, char *expr, double *params)
{
  int i, init_rad, init_length, varqty = 0;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  double *v = ol_vect_alloc ();

  v[2] = 1.;

  ut_string_function (expr, &fct, &vars, &vals, &varqty);

  init_rad = 0;
  init_length = 0;

  for (i = 0; i < varqty; i++)
  {
    if (!vars[i])
      ut_print_exprbug (expr);
    else if (!strcmp (vars[i], "rad"))
    {
      params[0] = atof (vals[i]);
      init_rad = 1;
    }
    else if (!strcmp (vars[i], "length"))
    {
      params[1] = atof (vals[i]);
      init_length = 1;
    }
    else if (!strcmp (vars[i], "axis"))
      sscanf (vals[i], "(%lf,%lf,%lf)", v, v + 1, v + 2);
    else
      abort ();
  }

  if (!init_rad)
    params[0] = pow (2. / 9, 1. / 3) * rad;

  if (!init_length)
    params[1] = 6 * params[0];

  ut_array_1d_memcpy (v, 3, params + 2);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ol_vect_free (&v);

  return;
}

void
nev_print_arr_expr_params (double rad, char *expr, double *params)
{
  int i, init_rad, init_length;
  int varqty = 0;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  double *v = ol_vect_alloc ();

  v[2] = 1.;

  ut_string_function (expr, &fct, &vars, &vals, &varqty);

  init_rad = 0;
  init_length = 0;

  for (i = 0; i < varqty; i++)
  {
    if (!vars[i])
      ut_print_exprbug (expr);
    else if (!strcmp (vars[i], "rad"))
    {
      params[0] = atof (vals[i]);
      init_rad = 1;
    }
    else if (!strcmp (vars[i], "length"))
    {
      params[1] = atof (vals[i]);
      init_length = 1;
    }
    else if (!strcmp (vars[i], "axis"))
      sscanf (vals[i], "(%lf,%lf,%lf)", v, v + 1, v + 2);
    else
      abort ();
  }

  double alpha = 10;

  if (!init_rad)
    params[0] = pow (4. / (3 * (alpha + 12)), 1. / 3) * rad;

  if (!init_length)
    params[1] = alpha * params[0];

  ut_array_1d_memcpy (v, 3, params + 2);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ol_vect_free (&v);

  return;
}

void
nev_print_tor_expr_params (double rad, char *expr, double *params)
{
  int i, init_rad1, init_rad2;
  int varqty = 0;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  double *v = ol_vect_alloc ();

  v[2] = 1.;

  ut_string_function (expr, &fct, &vars, &vals, &varqty);

  init_rad1 = 0;
  init_rad2 = 0;

  for (i = 0; i < varqty; i++)
  {
    if (!vars[i])
      ut_print_exprbug (expr);
    else if (!strcmp (vars[i], "rad1"))
    {
      init_rad1 = 1;
      params[0] = atof (vals[i]);
    }
    else if (!strcmp (vars[i], "rad2"))
    {
      init_rad2 = 1;
      params[1] = atof (vals[i]);
    }
    else if (!strcmp (vars[i], "axis"))
      sscanf (vals[i], "(%lf,%lf,%lf)", v, v + 1, v + 2);
    else
      abort ();
  }

  if (!init_rad1)
    params[0] = pow (3. / (2 * M_PI), 1. / 3) * rad;

  if (!init_rad2)
    params[1] = 2. / 3 * params[0];

  ut_array_1d_memcpy (v, 3, params + 2);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ol_vect_free (&v);

  return;
}

void
nev_print_ellipsoid_expr_params (double rad, char *expr, double *params)
{
  int i, varqty = 0;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  double *v1 = ol_vect_alloc ();
  double *v2 = ol_vect_alloc ();
  double *v3 = ol_vect_alloc ();

  v1[0] = 1.;
  v2[1] = 1.;
  v3[2] = 1.;

  ut_array_1d_set (params, 3, rad);

  ut_string_function (expr, &fct, &vars, &vals, &varqty);

  for (i = 0; i < varqty; i++)
  {
    if (!vars[i])
      ut_print_exprbug (expr);
    else if (!strcmp (vars[i], "rad1"))
      params[0] = atof (vals[i]);
    else if (!strcmp (vars[i], "rad2"))
      params[1] = atof (vals[i]);
    else if (!strcmp (vars[i], "rad3"))
      params[2] = atof (vals[i]);
    else if (!strcmp (vars[i], "axis1"))
      sscanf (vals[i], "(%lf,%lf,%lf)", v1, v1 + 1, v1 + 2);
    else if (!strcmp (vars[i], "axis2"))
      sscanf (vals[i], "(%lf,%lf,%lf)", v2, v2 + 1, v2 + 2);
    else if (!strcmp (vars[i], "axis3"))
      sscanf (vals[i], "(%lf,%lf,%lf)", v3, v3 + 1, v3 + 2);
    else
      abort ();
  }

  ut_array_1d_memcpy (v1, 3, params + 3);
  ut_array_1d_memcpy (v2, 3, params + 6);
  ut_array_1d_memcpy (v3, 3, params + 9);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ol_vect_free (&v1);
  ol_vect_free (&v2);
  ol_vect_free (&v3);

  return;
}
void
nev_print_disc_expr_params (double rad, char *expr, double *params)
{
  int i;
  int varqty = 0;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  double *v = ol_vect_alloc ();

  v[2] = 1.;

  ut_string_function (expr, &fct, &vars, &vals, &varqty);

  params[0] = rad;

  for (i = 0; i < varqty; i++)
  {
    if (!vars[i])
      ut_print_exprbug (expr);
    else if (!strcmp (vars[i], "rad"))
      params[0] = atof (vals[i]);
    else if (!strcmp (vars[i], "axis"))
      sscanf (vals[i], "(%lf,%lf,%lf)", v, v + 1, v + 2);
    else
      abort ();
  }

  ut_array_1d_memcpy (v, 3, params + 1);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ol_vect_free (&v);

  return;
}

void
nev_print_scale_ticks (char *scale, double *pmin, double *pmax,
                       char ***pticks, double **ptickvals, int *ptickqty,
                       char **ptickformat)
{
  int i, qty;
  char *minexpr = NULL, *maxexpr = NULL, *ticks = NULL;
  char **parts = NULL;
  char *tmp = NULL;
  *ptickformat = ut_alloc_1d_char (100);

  ut_list_break (scale, NEUT_SEP_DEP, &parts, &qty);

  if (qty >= 2)
  {
    ut_string_string (parts[0], &tmp);
    ut_string_fnrs (tmp, "-", "", 1);
    ut_string_format (tmp, *ptickformat);
    if (strstr (*ptickformat, "d"))
      ut_string_string ("%.0f", ptickformat);
    ut_string_string (parts[0], &minexpr);
    ut_string_string (parts[qty - 1], &maxexpr);
    ut_free_2d_char (&parts, qty);
  }
  else
    ut_string_string ("%.1f", ptickformat);

  if (qty > 2)
    ut_string_string (scale, &ticks);

  if (minexpr)
    ut_string_real (minexpr, pmin);
  if (maxexpr)
    ut_string_real (maxexpr, pmax);

  // In case *pmin and *pmax are equal, putting them apart a little
  if (*pmin == *pmax)
  {
    if (*pmin == 0)
    {
      *pmin = -1e-12;
      *pmax = 1e-12;
    }
    else
    {
      *pmin *= (1 - 1e-12);
      *pmax *= (1 + 1e-12);
    }
  }

  if (!ticks)
  {
    (*ptickqty) = 11;
    (*ptickvals) = ut_alloc_1d (*ptickqty);
    (*pticks) = ut_alloc_2d_char (*ptickqty, 11);
    for (i = 0; i < *ptickqty; i++)
    {
      (*ptickvals)[i] = *pmin + ((double) i / ((*ptickqty) - 1)) * (*pmax - *pmin);
      sprintf ((*pticks)[i], *ptickformat, (*ptickvals)[i]);
    }
  }
  else
  {
    ut_list_break_double (ticks, NEUT_SEP_DEP, ptickvals, ptickqty);
    (*pticks) = ut_alloc_2d_char ((*ptickqty), 100);

    for (i = 0; i < *ptickqty; i++)
      sprintf ((*pticks)[i], *ptickformat, (*ptickvals)[i]);
  }

  ut_free_1d_char (&minexpr);
  ut_free_1d_char (&maxexpr);
  ut_free_1d_char (&ticks);
  ut_free_1d_char (&tmp);

  return;
}

// Default radius of vertex- and edge-like entities (vertices, edges, seeds, 0D
// and 1D elements), scaled by the domain size and the number of cells, so
// that tessellations and meshes of the same domain get the same values.
double
nev_print_defrad (double size, int cellqty, double fact)
{
  return pow (size, 1. / 3) * fact / pow (ut_num_max_int (cellqty, 1), 0.25);
}

int
nev_print_tess_defrad (struct TESS Tess, int dim, double *prad)
{
  int status = 0;
  double size;

  neut_tess_size (Tess, &size);

  if (dim == Tess.Dim)
    (*prad) = 0.02;
  else if (dim == 0)
    (*prad) = nev_print_defrad (size, Tess.CellQty, NEV_DEFRAD_VER);
  else if (dim == 1)
    (*prad) = nev_print_defrad (size, Tess.CellQty, NEV_DEFRAD_EDGE);
  else if (dim == 4)
    (*prad) = nev_print_defrad (size, Tess.CellQty, NEV_DEFRAD_SEED);
  else
  {
    (*prad) = -1;
    status = -1;
  }

  return status;
}

void
nev_print_mesh_size_cellqty (struct NODES Nodes, struct MESH *Mesh, double *psize,
                             int *pcellqty)
{
  int meshdim = neut_mesh_array_dim (Mesh);

  (*psize) = 1;
  (*pcellqty) = 1;

  if (meshdim > 0)
  {
    neut_mesh_size (Nodes, Mesh[meshdim], psize);
    (*pcellqty) = Mesh[meshdim].ElsetQty;
  }

  return;
}

int
nev_print_mesh_defrad (double size, int cellqty, int dim, double *prad)
{
  int status = 0;

  if (dim == 0)
    (*prad) = nev_print_defrad (size, cellqty, NEV_DEFRAD_VER);
  else if (dim == 1)
    (*prad) = nev_print_defrad (size, cellqty, NEV_DEFRAD_EDGE);
  else if (dim == 2 || dim == 3) // used by pf
    (*prad) = 0.02;
  else
  {
    (*prad) = -1;
    status = -1;
  }

  return status;
}

int
nev_print_mesh_defedgerad (double size, int cellqty, int dim, char *space, double *prad)
{
  int status = 0, real = !space || !strcmp (space, "real");

  if (dim == 0)
    (*prad) = nev_print_defrad (size, cellqty, NEV_DEFRAD_VER);
  else if (dim == 1)
    (*prad) = nev_print_defrad (size, cellqty, NEV_DEFRAD_EDGE);
  else if (dim == 2)
    (*prad) = real ? nev_print_defrad (size, cellqty, NEV_DEFRAD_ELT2DEDGE) : 0.01;
  else if (dim == 3)
    (*prad) = real ? nev_print_defrad (size, cellqty, NEV_DEFRAD_ELT3DEDGE) : 0.01;
  else
  {
    (*prad) = -1;
    status = -1;
  }

  return status;
}

double
nev_print_nodes_defrad (int nodeqty)
{
  return NEV_DEFRAD_NODE / pow (ut_num_max_int (nodeqty, 1), 0.25);
}

// Replaces the variable `default' of a radius expression by the default
// radius (defrad < 0 if not available).
void
nev_print_rad_default (char *expr, double defrad, char **pexpr)
{
  char *var = (char *) "default";

  nev_print_rad_vars (expr, 1, &var, &defrad, pexpr);

  return;
}

// Replaces the variables of a radius expression (whole words) by their values
// (val < 0 if not available).
void
nev_print_rad_vars (char *expr, int varqty, char **vars, double *vals, char **pexpr)
{
  int i, j, len;
  char *val = ut_alloc_1d_char (100);

  (*pexpr) = ut_alloc_1d_char (strlen (expr) * 100 + 1);

  for (i = 0; expr[i] != '\0';)
  {
    for (j = 0; j < varqty; j++)
    {
      len = strlen (vars[j]);
      if (!strncmp (expr + i, vars[j], len)
          && (i == 0 || !(isalnum (expr[i - 1]) || expr[i - 1] == '_'))
          && !(isalnum (expr[i + len]) || expr[i + len] == '_')
          && strncmp (expr, "file(", 5))
        break;
    }

    if (j < varqty)
    {
      if (vals[j] < 0)
        ut_print_message (2, 3, "`%s' is not available for this radius.\n", vars[j]);

      sprintf (val, "%.12g", vals[j]);
      strcat (*pexpr, val);
      i += strlen (vars[j]);
    }
    else
    {
      strncat (*pexpr, expr + i, 1);
      i++;
    }
  }

  (*pexpr) = ut_realloc_1d_char (*pexpr, strlen (*pexpr) + 1);

  ut_free_1d_char (&val);

  return;
}

// Default radius of the domain edges (-showdomain): twice the default radius
// of the tessellation edges, so that the domain stands out from them
double
nev_print_domain_defrad (struct TESS Tess)
{
  int cellqty = 1;
  double size = 1;

  if (!neut_tess_isvoid (Tess))
  {
    neut_tess_size (Tess, &size);
    cellqty = Tess.CellQty;
  }

  return NEV_DEFRAD_DOMAIN * nev_print_defrad (size, cellqty, NEV_DEFRAD_EDGE);
}
