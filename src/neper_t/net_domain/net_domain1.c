/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_.h"

void
net_domain (struct IN_T In, struct MTESS *pMTess, struct TESS *pDomain)
{
  int i, curved, PrimQty = 0;
  struct POLY Poly;
  struct PRIM *Prim = NULL;
  char *domain = NULL, **strings = NULL;
  int stringqty = 0;

  ut_list_break (In.domain, NEUT_SEP_DEP, &strings, &stringqty);

  ut_string_function (strings[0], &domain, NULL, NULL, NULL);

  curved = net_domain_curved (In, strings, stringqty, domain);

  neut_poly_set_zero (&Poly);
  neut_tess_reset (pDomain);

  // build-in
  if (!strncmp (strings[0], "cube", 4) || !strncmp (strings[0], "square", 6)
   || !strncmp (strings[0], "euler", 5))
    net_domain_cube_string (strings[0], &Poly);

  else if (!strcmp (domain, "cylinder") || !strcmp (domain, "circle"))
    net_domain_cylinder_string (strings[0], In.n[1], curved, &Poly, &Prim,
                                &PrimQty);

  else if (!strcmp (domain, "sphere"))
    net_domain_sphere_string (strings[0], curved, &Poly, &Prim, &PrimQty);

  else if (!strcmp (domain, "hourglass"))
  {
    if (!curved)
      ut_print_message (2, 2, "Domain `hourglass' needs a single-scale, non-periodic 3D tessellation, and can only be translated or rotated.\n");
    net_domain_hourglass_string (strings[0], In.n[1], &Poly, &Prim, &PrimQty);
  }

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

  (*pDomain).DomPrimQty = PrimQty;
  (*pDomain).DomPrim = Prim;

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

// net_domain_fromtess sets a domain from the domain information of a
// tessellation (TESS.Dom*), typically read from a file.  As in net_domain, the
// domain is a polyhedron, and curved domain faces are described by primitives
// and a hull.  The domain is left void if it cannot be recovered, i.e. if the
// tessellation has no domain, if its plane faces do not define a convex
// polyhedron or if its curved faces do not match the tessellation.
void
net_domain_fromtess (struct TESS Tess, struct TESS *pDomain)
{
  int i, status = -1, eqqty = 0, PrimQty = 0;
  double **eqs = NULL;
  struct POLY Poly;
  struct PRIM *Prim = NULL;

  neut_poly_set_zero (&Poly);

  if (Tess.Dim == 3 && Tess.DomFaceQty > 0)
    status = net_domain_fromtess_3d (Tess, &eqs, &eqqty, &Prim, &PrimQty);
  else if (Tess.Dim == 2 && Tess.DomEdgeQty > 0)
    status = net_domain_fromtess_2d (Tess, &eqs, &eqqty, &Poly);

  if (!status)
    status = net_domain_fromtess_convex (Tess, eqs, eqqty);

  if (!status)
  {
    net_domain_clip (&Poly, eqs, eqqty);
    net_poly_tess (Poly, NULL, pDomain);
    status = neut_tess_isvoid (*pDomain);
  }

  if (!status)
  {
    ut_string_string (Tess.DomType, &(*pDomain).DomType);

    (*pDomain).DomPrimQty = PrimQty;
    (*pDomain).DomPrim = Prim;

    (*pDomain).VerDom = ut_alloc_2d_int ((*pDomain).VerQty + 1, 2);
    (*pDomain).EdgeDom = ut_alloc_2d_int ((*pDomain).EdgeQty + 1, 2);
    (*pDomain).FaceDom = ut_alloc_2d_int ((*pDomain).FaceQty + 1, 2);
    ut_array_2d_int_set ((*pDomain).VerDom + 1, (*pDomain).VerQty, 2, -1);
    ut_array_2d_int_set ((*pDomain).EdgeDom + 1, (*pDomain).EdgeQty, 2, -1);
    ut_array_2d_int_set ((*pDomain).FaceDom + 1, (*pDomain).FaceQty, 2, -1);

    for (i = 1; i <= (*pDomain).FaceQty; i++)
      (*pDomain).FacePoly[i][1] = -i;
  }

  else
  {
    for (i = 0; i < PrimQty; i++)
      neut_prim_free (Prim + i);
    free (Prim);
  }

  neut_poly_free (&Poly);
  ut_free_2d (&eqs, eqqty);

  return;
}

// net_domain_cut cuts a tessellation by the primitives of its domain, Dom,
// to get the actual (curved) domain.  The curved surfaces are described by
// one face per cell, so that this requires a sufficient number of cells
// (typically 100 or more).
void
net_domain_cut (struct TESS Dom, struct TESS *pTess)
{
  if (Dom.DomPrimQty == 0)
    return;

  ut_print_message (0, 1, "Cutting tessellation by the domain...\n");

  net_transform_tess_cut_prim (Dom.DomPrim, Dom.DomPrimQty, pTess);

  // (the faces of a cut domain keep their labels)
  if (strcmp (Dom.DomType, "cut"))
  {
    ut_string_string (Dom.DomType, &(*pTess).DomType);
    ut_free_2d_char (&(*pTess).DomFaceLabel, (*pTess).DomFaceQty + 1);
    neut_tess_init_domain_label_3d (pTess);
  }

  return;
}
