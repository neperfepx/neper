/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_stdtriangle_.h"

void
net_domain_stdtriangle_parms (char *stdtriangle, struct PF *pPf)
{
  int i, valqty;
  char **vars = NULL, **vals = NULL;

  ut_string_string ("cubic", &(*pPf).crysym);
  ut_string_string ("stereographic", &(*pPf).projection);
  (*pPf).ipfptqty = 22;

  ut_string_function (stdtriangle, NULL, &vars, &vals, &valqty);

  for (i = 0; i < valqty; i++)
  {
    if (!vars[i] || !strcmp (vars[i], "segmentnb"))
      (*pPf).ipfptqty = atoi (vals[i]) + 2;
    else if (!strcmp (vars[i], "crysym"))
      ut_string_string (vals[i], &(*pPf).crysym);
    else if (!strcmp (vars[i], "projection"))
      ut_string_string (vals[i], &(*pPf).projection);
    else
      ut_print_message (2, 0, "Unknown expression `%s'.\n", vars[i]);
  }

  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);

  neut_pf_init_ipfborder (pPf);

  return;
}

void
net_domain_stdtriangle_compute (struct PF Pf, struct POLY *pPoly)
{
  double **eq = ut_alloc_2d (Pf.ipfptqty + 2, 4);

  net_domain_stdtriangle_compute_planes (Pf, eq);
  net_domain_clip (pPoly, eq, Pf.ipfptqty + 2);

  (*pPoly).PseudoDim = 2;
  (*pPoly).PseudoSize = 1e-6;

  ut_free_2d (&eq, Pf.ipfptqty + 2);

  return;
}
