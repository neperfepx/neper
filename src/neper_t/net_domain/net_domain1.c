/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_.h"

void
net_domain (struct IN_T In, struct MTESS *pMTess, struct TESS *pDomain)
{
  int i, pseudodim = -1;
  double pseudosize = -DBL_MAX;
  double **size = ut_alloc_2d (3, 2);
  struct POLY Poly;
  char *tmp = ut_alloc_1d_char (strlen (In.domain) + 1);
  char *domtype = ut_alloc_1d_char (strlen (In.domain) + 1);
  char **sizestring = NULL;
  char *crysym = ut_alloc_1d_char (100);
  char **strings = NULL;
  int stringqty;
  double *parms = ut_alloc_1d (10);

  ut_string_separate (In.domain, NEUT_SEP_DEP, &strings, &stringqty);

  strcpy (tmp, strings[0]);
  ut_string_fnr (tmp, '(', ' ');
  ut_string_fnr (tmp, ')', ' ');
  ut_string_fnr (tmp, ',', ' ');
  sscanf (tmp, "%s", domtype);
  sizestring = ut_alloc_2d_char (3, strlen (tmp) + 1);

  neut_poly_set_zero (&Poly);

  if (!strncmp (In.domain, "cube", 4) || !strncmp (In.domain, "square", 6))
    net_domain_cube_string (In.domain, &Poly);

  else if (!strcmp (domtype, "cylinder") || !strcmp (domtype, "circle"))
    net_domain_cylinder_string (In.domain, In.n[1], &Poly);

  else if (!strcmp (domtype, "stdtriangle"))
    net_domain_stdtriangle_string (In.domain, &Poly);

  else if (!strcmp (domtype, "sphere"))
    net_domain_sphere_string (In.domain, &Poly);

  else if (!strcmp (domtype, "rodrigues"))
    net_domain_rodrigues_string (In.domain, &Poly);

  else if (!strcmp (domtype, "planes"))
    net_domain_planes_string (In.domain, In.dim, &Poly);

  else if (!strcmp (domtype, "cell"))
    net_domain_cell_string (In.domain, &Poly);

  else
    ut_print_message (2, 0, "Domain type unknown (%s)\n", domtype);

  if (Poly.VerQty > 0)
  {
    double *coo = ut_alloc_1d (3);
    neut_poly_centroid (Poly, coo);
    net_poly_tess (Poly, coo, pDomain);
    ut_free_1d (coo);
  }

  (*pDomain).Level = 0;
  (*pDomain).PseudoDim = pseudodim;
  if (pseudodim != -1)
  {
    if (pseudosize < 0)
      ut_error_reportbug ();
    else
      (*pDomain).PseudoSize = pseudosize;
  }

  (*pDomain).DomType = ut_alloc_1d_char (strlen (In.domain) + 1);
  strcpy ((*pDomain).DomType, domtype);

  (*pDomain).VerDom = ut_alloc_2d_int ((*pDomain).VerQty + 1, 2);
  (*pDomain).EdgeDom = ut_alloc_2d_int ((*pDomain).EdgeQty + 1, 2);
  (*pDomain).FaceDom = ut_alloc_2d_int ((*pDomain).FaceQty + 1, 2);
  ut_array_2d_int_set ((*pDomain).VerDom + 1, (*pDomain).VerQty, 2, -1);
  ut_array_2d_int_set ((*pDomain).EdgeDom + 1, (*pDomain).EdgeQty, 2, -1);
  ut_array_2d_int_set ((*pDomain).FaceDom + 1, (*pDomain).FaceQty, 2, -1);

  (*pMTess).DomTessFaceNb[0] = ut_alloc_2d_int ((*pDomain).FaceQty + 1, 2);

  for (i = 1; i < stringqty; i++)
    net_domain_transform (pDomain, strings[i]);

  ut_free_2d (size, 3);
  neut_poly_free (&Poly);
  ut_free_1d_char (tmp);
  ut_free_1d_char (domtype);
  ut_free_2d_char (sizestring, 3);
  ut_free_2d_char (strings, stringqty);
  ut_free_1d_char (crysym);
  ut_free_1d (parms);

  return;
}
