/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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
  ol_g_free (g);

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
  ol_vect_free (v);

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
  ol_vect_free (v);

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
  ol_vect_free (v);

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
  ol_vect_free (v1);
  ol_vect_free (v2);
  ol_vect_free (v3);

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
  ol_vect_free (v);

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
