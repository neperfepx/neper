/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_.h"

void
net_domain_cylinder_string (char *domain, char *nstring, struct POLY *pDomain)
{
  int pseudodim;
  double pseudosize;
  double *parms = ut_alloc_1d (10);

  net_domain_cylinderparms (domain, nstring, parms, &pseudodim, &pseudosize);
  net_domain_cylinder (parms, pDomain);

  if (pseudodim != -1)
  {
    (*pDomain).PseudoDim = pseudodim;
    (*pDomain).PseudoSize = pseudosize;
  }

  ut_free_1d (parms);

  return;
}

void
net_domain_cylinderparms (char *domain, char *nstring, double *parms,
                          int *ppseudodim, double *ppseudosize)
{
  int i, varqty;
  char **vars = NULL;
  char *fct = NULL;

  parms[2] = -1;
  *ppseudodim = -1;
  *ppseudosize = -DBL_MAX;

  ut_string_function_separate (domain, &fct, NULL, &vars, &varqty);

  if (!strncmp (fct, "cylinder", 8))
  {
    if (varqty != 2 && varqty != 3)
      ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

    for (i = 0; i < varqty; i++)
      ut_string_real (vars[i], parms + i);
  }

  else if (!strcmp (fct, "circle"))
  {
    if (varqty != 1 && varqty != 2)
      ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

    for (i = 0; i < varqty; i++)
      ut_string_real (vars[i], parms + i + 1);
    parms[0] = 1e-6 * parms[1];

    *ppseudodim = 2;
    *ppseudosize = parms[0];
  }

  else
    ut_error_reportbug ();

  if (ut_num_d2ri (parms[2]) != -1 && ut_num_d2ri (parms[2]) < 3)
  {
    ut_print_message (1, 2, "Increasing the number of facets to 3.\n");
    parms[2] = 3;
  }

  if (ut_num_d2ri (parms[2]) == -1)
  {
    double cl;
    int varqty = 1;
    char **vars = ut_alloc_2d_char (1, 10);
    double *vals = ut_alloc_1d (1);
    double res;

    strcpy (vars[0], "vol");
    if (!strcmp (fct, "cylinder"))
      vals[0] = M_PI * parms[0] * parms[0] * parms[1] / 4;
    else if (!strcmp (fct, "circle"))
      vals[0] = M_PI * parms[1] / 4;
    else
      abort ();

    if (!strcmp (nstring, "from_morpho"))
      ut_print_messagewnc (2, 72, "The number of facets must be specified in `-morpho cylinder' when using `-n from_morpho'.");

    ut_math_eval (nstring, 1, vars, vals, &res);
    res = ut_num_max (res, 1);

    ut_free_2d_char (vars, varqty);
    ut_free_1d (vals);

    if (!strcmp (fct, "cylinder"))
      rcl2cl (1, 3, M_PI * parms[0] * parms[0] * parms[1] / 4, ut_num_d2ri (res), NULL, &cl);
    else if (!strcmp (fct, "circle"))
      rcl2cl (1, 3, M_PI * parms[1] / 4, ut_num_d2ri (res), NULL, &cl);
    else
      abort ();

    parms[2] = (int) floor (M_PI * parms[1] / cl);
    parms[2] = ut_num_max_int (parms[2], 12);
  }
  parms[2] += 2;

  ut_free_1d_char (fct);
  ut_free_2d_char (vars, varqty);

  return;
}

void
net_domain_cylinder (double *parms, struct POLY *pDomain)
{
  double **eq = ut_alloc_2d (ut_num_d2ri (parms[2]), 4);

  net_domain_cylinder_planes (parms[0], parms[1] / 2, parms[2] - 2, eq);
  net_domain_clip (pDomain, eq, parms[2]);
  ut_free_2d (eq, parms[2]);

  return;
}

void
net_domain_stdtriangle_string (char *domain, struct POLY *pDomain)
{
  double *parms = ut_alloc_1d (2);

  net_domain_stdtriangleparms (domain, parms);
  net_domain_stdtriangle (parms, pDomain);

  ut_free_1d (parms);

  return;
}

void
net_domain_stdtriangleparms (char *domain, double *parms)
{
  int varqty;
  char **vars = NULL;

  ut_string_function_separate (domain, NULL, NULL, &vars, &varqty);

  if (varqty != 1)
    ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

  ut_string_real (vars[0], parms);

  parms[0] += 4;

  ut_free_2d_char (vars, varqty);

  return;
}

void
net_domain_stdtriangle (double *parms, struct POLY *pDomain)
{
  int qty = ut_num_d2ri (parms[0]);
  double **eq = ut_alloc_2d (qty, 4);

  net_domain_stdtriangle_planes (qty - 4, eq);
  net_domain_clip (pDomain, eq, qty);

  (*pDomain).PseudoDim = 2;
  (*pDomain).PseudoSize = 1e-6;

  ut_free_2d (eq, qty);

  return;
}

void
net_domain_sphere_string (char *domain, struct POLY *pDomain)
{
  double *parms = ut_alloc_1d (2);

  net_domain_sphereparms (domain, parms);
  net_domain_sphere (parms, pDomain);

  ut_free_1d (parms);

  return;
}

void
net_domain_sphereparms (char *domain, double *parms)
{
  int i, varqty;
  char **vars = NULL;

  parms[1] = 100;

  ut_string_function_separate (domain, NULL, NULL, &vars, &varqty);

  if (varqty != 1 && varqty != 2)
    ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

  for (i = 0; i < varqty; i++)
    ut_string_real (vars[i], parms + i);

  parms[0] += 4;

  if (parms[1] != -1 && parms[1] < 4)
  {
    ut_print_message (1, 2, "Increasing the number of facets to 4.\n");
    parms[1] = 4;
  }

  ut_free_2d_char (vars, varqty);

  return;
}

void
net_domain_sphere (double *parms, struct POLY *pDomain)
{
  double **eq = ut_alloc_2d (parms[1], 4);

  net_domain_sphere_planes (parms[0] / 2, parms[1], eq);
  net_domain_clip (pDomain, eq, parms[1]);
  ut_free_2d (eq, parms[1]);

  return;
}
