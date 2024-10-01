/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_.h"

void
net_domain (struct IN_T In, struct MTESS *pMTess, struct TESS *pDomain)
{
  int i;
  struct POLY Poly;
  char *domain = NULL, **strings = NULL;
  int stringqty = 0;

  ut_list_break (In.domain, NEUT_SEP_DEP, &strings, &stringqty);

  ut_string_function (strings[0], &domain, NULL, NULL, NULL);

  neut_poly_set_zero (&Poly);
  neut_tess_set_zero (pDomain);

  // build-in
  if (!strncmp (strings[0], "cube", 4) || !strncmp (strings[0], "square", 6)
   || !strncmp (strings[0], "euler", 5))
    net_domain_cube_string (strings[0], &Poly);

  else if (!strcmp (domain, "cylinder") || !strcmp (domain, "circle"))
    net_domain_cylinder_string (strings[0], In.n[1], &Poly);

  else if (!strcmp (domain, "sphere"))
    net_domain_sphere_string (strings[0], &Poly);

  else if (!strcmp (domain, "rodrigues"))
    net_domain_rodrigues_string (strings[0], &Poly);

  else if (!strcmp (domain, "stdtriangle"))
    net_domain_stdtriangle (strings[0], &Poly);

  // custom
  else if (ut_file_exist (strings[0]))
    net_domain_fromfile (strings[0], pDomain);

  else if (!strcmp (domain, "planes"))
    net_domain_planes_string (strings[0], In.dim, &Poly);

  else
    ut_print_message (2, 0, "Domain type unknown (%s)\n", domain);

  if (neut_tess_isvoid (*pDomain))
    net_poly_tess (Poly, NULL, pDomain);

  if (!strstr (In.domain, "rodrigues") && !strstr (In.domain, "euler"))
    ut_string_string (domain, &(*pDomain).DomType);
  else
    ut_string_string (In.domain, &(*pDomain).DomType);

  (*pDomain).VerDom = ut_alloc_2d_int ((*pDomain).VerQty + 1, 2);
  (*pDomain).EdgeDom = ut_alloc_2d_int ((*pDomain).EdgeQty + 1, 2);
  (*pDomain).FaceDom = ut_alloc_2d_int ((*pDomain).FaceQty + 1, 2);
  ut_array_2d_int_set ((*pDomain).VerDom + 1, (*pDomain).VerQty, 2, -1);
  ut_array_2d_int_set ((*pDomain).EdgeDom + 1, (*pDomain).EdgeQty, 2, -1);
  ut_array_2d_int_set ((*pDomain).FaceDom + 1, (*pDomain).FaceQty, 2, -1);

  (*pMTess).DomTessFaceNb[0] = ut_alloc_2d_int ((*pDomain).FaceQty + 1, 2);

  for (i = 1; i < stringqty; i++)
    net_domain_transform (pDomain, In.dim, strings[i]);

  for (i = 1; i <= (*pDomain).FaceQty; i++)
    (*pDomain).FacePoly[i][1] = -i;

  neut_poly_free (&Poly);
  ut_free_1d_char (&domain);
  ut_free_2d_char (&strings, stringqty);

  return;
}
