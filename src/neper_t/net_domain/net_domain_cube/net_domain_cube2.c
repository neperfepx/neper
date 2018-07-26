/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_cube_.h"

void
net_domain_cubeparms (char *domain, double **size, int *ppseudodim, double *ppseudosize)
{
  int i, varqty;
  char **vars = NULL;
  char *fct = NULL;

  ut_array_2d_zero (size, 3, 2);

  ut_string_function_separate (domain, &fct, NULL, &vars, &varqty);

  if ((!strcmp (fct, "square") && varqty != 2)
   || (!strcmp (fct, "cube") && varqty != 3))
    ut_print_message (2, 0, "Failed to process expression `%s'.\n", domain);

  if (!strcmp (fct, "cube"))
  {
    for (i = 0; i < 3; i++)
      ut_string_real (vars[i], &(size[i][1]));

    *ppseudodim = -1;
    *ppseudosize = -DBL_MAX;
  }

  else if (!strcmp (fct, "square"))
  {
    for (i = 0; i < 2; i++)
      ut_string_real (vars[i], &(size[i][1]));
    size[2][1] = 1e-6 * ut_num_min (size[0][1], size[1][1]);

    *ppseudodim = 2;
    *ppseudosize = size[2][1];
  }

  else
    abort ();

  ut_free_1d_char (fct);
  ut_free_2d_char (vars, varqty);

  return;
}

void
CubeVer (double **size, struct POLY *pDomain)
{
  IniVerQty (pDomain);
  IniVerFace (pDomain);
  IniVerCoo (size, pDomain);

  return;
}

void
CubeFaces (double **size, struct POLY *pDomain)
{
  IniFaceQty (pDomain);
  IniFaceEq (size, pDomain);
  IniFacePoly (pDomain);
  IniFaceVerQty (pDomain);
  IniFaceVerNb (pDomain);

  return;
}
