/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_.h"

void
net_domain (struct IN_T In, struct MTESS *pMTess, struct TESS *pDomain)
{
  int i, qty, id, qty0, pseudodim = -1;
  double h, d, pseudosize = -DBL_MAX;
  double **size = ut_alloc_2d (3, 2);
  struct POLY Poly;
  char *tmp = ut_alloc_1d_char (strlen (In.domain) + 1);
  char *domtype = ut_alloc_1d_char (strlen (In.domain) + 1);
  char **sizestring = NULL;
  char *crysym = ut_alloc_1d_char (100);
  double *coo = ut_alloc_1d (3);
  char **strings = NULL;
  int stringqty;

  ut_string_separate (In.domain, NEUT_SEP_DEP, &strings, &stringqty);

  strcpy (tmp, strings[0]);
  ut_string_fnr (tmp, '(', ' ');
  ut_string_fnr (tmp, ')', ' ');
  ut_string_fnr (tmp, ',', ' ');
  sscanf (tmp, "%s", domtype);
  sizestring = ut_alloc_2d_char (3, strlen (tmp) + 1);

  neut_poly_set_zero (&Poly);

  if (!strcmp (domtype, "cube"))
  {
    if (ut_string_nbwords (tmp) != 4
	|| sscanf (tmp, "%*s%s%s%s",
		   sizestring[0], sizestring[1], sizestring[2]) != 3)
      ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

    ut_string_real (sizestring[0], &(size[0][1]));
    ut_string_real (sizestring[1], &(size[1][1]));
    ut_string_real (sizestring[2], &(size[2][1]));

    net_domain_cube (size, &Poly);
    neut_poly_centroid (Poly, coo);
  }
  else if (!strcmp (domtype, "square"))
  {
    if (ut_string_nbwords (tmp) != 3
	|| sscanf (tmp, "%*s%s%s", sizestring[0], sizestring[1]) != 2)
      ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

    ut_string_real (sizestring[0], &(size[0][1]));
    ut_string_real (sizestring[1], &(size[1][1]));
    size[2][1] = 1e-6 * ut_num_min (size[0][1], size[1][1]);

    pseudodim = 2;
    pseudosize = size[2][1];

    net_domain_cube (size, &Poly);
    neut_poly_centroid (Poly, coo);
  }
  else if (!strcmp (domtype, "cylinder") || !strcmp (domtype, "circle"))
  {
    if (!strcmp (domtype, "cylinder"))
    {
      qty = -1;
      if (ut_string_nbwords (tmp) == 3)
      {
	if (sscanf (tmp, "%*s%s%s", sizestring[0], sizestring[1]) != 2)
	  ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

	ut_string_real (sizestring[0], &h);
	ut_string_real (sizestring[1], &d);
      }
      else if (ut_string_nbwords (tmp) == 4)
      {
	if (sscanf (tmp, "%*s%s%s%s", sizestring[0], sizestring[1],
		    sizestring[2]) != 3)
	  ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

	ut_string_real (sizestring[0], &h);
	ut_string_real (sizestring[1], &d);
	ut_string_int (sizestring[2], &qty);
      }
    }
    else if (!strcmp (domtype, "circle"))
    {
      h = 1;
      qty = -1;
      if (ut_string_nbwords (tmp) == 2)
      {
	if (sscanf (tmp, "%*s%s", sizestring[0]) != 1)
	  ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

	ut_string_real (sizestring[0], &d);
      }
      else if (ut_string_nbwords (tmp) == 3)
      {
	if (sscanf (tmp, "%*s%s%s", sizestring[0], sizestring[1]) != 2)
	  ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

	ut_string_real (sizestring[0], &d);
	ut_string_int (sizestring[1], &qty);
      }
    }
    else
      ut_error_reportbug ();

    if (qty != -1 && qty < 3)
    {
      ut_print_message (1, 2, "Increasing the number of facets to 3.\n");
      qty = 3;
    }

    if (qty == -1)
    {
      double cl;
      int varqty = 1;
      char **vars = ut_alloc_2d_char (1, 10);
      double *vals = ut_alloc_1d (1);
      double res;

      strcpy (vars[0], "vol");
      vals[0] = M_PI * d * d * h / 4;

      if (!strcmp (In.n[1], "from_morpho"))
        ut_print_messagewnc (2, 72, "The number of facets must be specified in `-morpho cylinder' when using `-n from_morpho'.");

      ut_math_eval (In.n[1], 1, vars, vals, &res);
      res = ut_num_max (res, 1);

      ut_free_2d_char (vars, varqty);
      ut_free_1d (vals);

      rcl2cl (1, 3, M_PI * d * d * h / 4, ut_num_d2ri (res), NULL, &cl);

      qty = (int) floor (M_PI * d / cl);
      qty = ut_num_max_int (qty, 12);
    }
    qty += 2;

    double **eq = ut_alloc_2d (qty, 4);

    if (!strcmp (domtype, "circle"))
    {
      h = 1e-6 * d;

      pseudodim = 2;
      pseudosize = h;
    }

    net_domain_cylinder_planes (h, d / 2, qty - 2, eq);
    net_domain_clip (&Poly, eq, qty);
    neut_poly_centroid (Poly, coo);
    ut_free_2d (eq, qty);
  }
  else if (!strcmp (domtype, "stdtriangle"))
  {
    qty = 10;
    if (ut_string_nbwords (tmp) == 2)
    {
      if (sscanf (tmp, "%*s%s", sizestring[0]) != 1)
        ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

      ut_string_int (sizestring[0], &qty);
    }
    pseudodim = 2;
    pseudosize = 1e-6;

    qty += 4;

    double **eq = ut_alloc_2d (qty, 4);

    net_domain_stdtriangle_planes (qty - 4, eq);
    net_domain_clip (&Poly, eq, qty);
    neut_poly_centroid (Poly, coo);
    ut_free_2d (eq, qty);
  }
  else if (!strcmp (domtype, "sphere"))
  {
    qty = 100;

    if (ut_string_nbwords (tmp) == 2)
    {
      if (sscanf (tmp, "%*s%s", sizestring[0]) != 1)
	ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

      ut_string_real (sizestring[0], &d);
    }
    else if (ut_string_nbwords (tmp) == 3)
    {
      if (sscanf (tmp, "%*s%s%s", sizestring[0], sizestring[1]) != 2)
	ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

      ut_string_real (sizestring[0], &d);
      ut_string_int (sizestring[1], &qty);
    }

    if (qty != -1 && qty < 4)
    {
      ut_print_message (1, 2, "Increasing the number of facets to 4.\n");
      qty = 4;
    }

    double **eq = ut_alloc_2d (qty, 4);

    net_domain_sphere_planes (d / 2, qty, eq);
    net_domain_clip (&Poly, eq, qty);
    neut_poly_centroid (Poly, coo);
    ut_free_2d (eq, qty);
  }
  else if (!strcmp (domtype, "rodrigues"))
  {
    if (ut_string_nbwords (tmp) != 2 || sscanf (tmp, "%*s%s", crysym) != 1)
      ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

    net_domain_rodrigues (&Poly, crysym);
    neut_poly_centroid (Poly, coo);
  }
  else if (!strcmp (domtype, "planes"))
  {
    int i, qty;
    char *filename = ut_alloc_1d_char (strlen (tmp) + 1);
    FILE *file = NULL;

    if (ut_string_nbwords (tmp) != 2)
      ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);
    sscanf (tmp, "%*s%s", filename);
    file = ut_file_open (filename, "r");

    if (fscanf (file, "%d", &qty) != 1)
      abort ();

    qty0 = (In.dim == 2) ? 2 : 0;

    double **eq = ut_alloc_2d (qty + qty0, 4);

    // recording z0 and z1 faces
    if (In.dim == 2)
    {
      eq[0][3] = -1;
      eq[1][3] = 1;
      eq[1][0] = 1e-6;
    }

    // recording faces
    for (i = 0; i < qty; i++)
    {
      id = qty0 + i;
      ut_array_1d_fscanf (file, eq[id], 4);
      if (ut_array_1d_norm (eq[id] + 1, 3) == 0)
	ut_print_message (2, 3, "Face %d: normal norm is zero.\n", id - 1);
      ut_array_1d_scale (eq[id], 4, 1. / ut_vector_norm (eq[id] + 1));
    }

    net_domain_clip (&Poly, eq, qty + qty0);
    neut_poly_centroid (Poly, coo);

    ut_free_2d (eq, qty);

    ut_file_close (file, filename, "r");
    ut_free_1d_char (filename);
  }
  else if (!strcmp (domtype, "cell"))
  {
    int cell;
    char *filename = ut_alloc_1d_char (strlen (tmp) + 1);
    struct TESS Tessb;

    neut_tess_set_zero (&Tessb);

    if (ut_string_nbwords (tmp) != 3
	|| sscanf (tmp, "%*s%s%s", filename, sizestring[1]) != 2)
      ut_print_message (2, 0, "Unknown expression `%s'.\n", In.domain);

    neut_tess_name_fscanf (filename, &Tessb);
    sscanf (sizestring[1], "%d", &cell);
    net_tess_poly (Tessb, cell, &Poly);
    neut_poly_centroid (Poly, coo);

    neut_tess_free (&Tessb);
    ut_free_1d_char (filename);
  }
  else
  {
    ut_print_message (2, 0, "Domain type unknown (%s)\n", domtype);
    abort ();
  }

  if (Poly.VerQty > 0)
    net_poly_tess (Poly, coo, pDomain);
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
  ut_free_1d (coo);
  ut_free_2d_char (strings, stringqty);
  ut_free_1d_char (crysym);

  return;
}
