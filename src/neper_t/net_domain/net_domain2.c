/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_domain_.h"

void
net_domain_fromfile (char *filename, struct TESS *pDomain)
{
  if (ut_file_testformat (filename, "tess"))
  {
    neut_tess_fnscanf (filename, pDomain, "r");
    (*pDomain).Level = 0;
    (*pDomain).TessId = 0;
  }

  else if (ut_file_testformat (filename, "obj"))
    neut_tess_fnscanf_obj (filename, pDomain);

  else if (ut_file_testformat (filename, "ovm"))
    neut_tess_fnscanf_ovm (filename, pDomain);

  else
    ut_print_message (2, 0, "Unknown file type (%s)\n", filename);

  return;
}

// net_domain_fromtess_3d returns the plane equations and primitives of the
// domain of a 3D tessellation.  Curved faces are replaced by a hull (see
// net_domain_fromtess_hull).  Returns 0 on success and -1 otherwise.
int
net_domain_fromtess_3d (struct TESS Tess, double ***peqs, int *peqqty,
                        struct PRIM **pPrim, int *pPrimQty)
{
  int i, j, status = 0;
  double size, *eq = ut_alloc_1d (4), **bbox = ut_alloc_2d (3, 2);

  for (i = 1; i <= Tess.DomFaceQty && !status; i++)
    if (Tess.DomFaceType && !Tess.DomFaceType[i])
      status = -1;
    else if (!Tess.DomFaceType || !strcmp (Tess.DomFaceType[i], "plane"))
      net_domain_fromtess_addeq (Tess.DomFaceEq[i], peqs, peqqty);
    else
    {
      (*pPrim) = realloc (*pPrim, (*pPrimQty + 1) * sizeof (struct PRIM));
      neut_prim_set_zero ((*pPrim) + *pPrimQty);
      (*pPrimQty)++;
      status = net_domain_fromtess_prim (Tess, i, (*pPrim) + *pPrimQty - 1);

      if (!status)
        net_domain_fromtess_hull (Tess, i, (*pPrim)[*pPrimQty - 1], peqs, peqqty);
    }

  // box that bounds the domain whatever the primitives
  if (!status && *pPrimQty > 0)
  {
    size = net_domain_fromtess_bbox (Tess, bbox);

    if (size == 0)
      status = -1;
    else
      for (i = 0; i < 3; i++)
        for (j = 0; j < 2; j++)
        {
          ut_array_1d_zero (eq, 4);
          eq[i + 1] = 2 * j - 1;
          eq[0] = (2 * j - 1) * bbox[i][j] + size;
          net_domain_fromtess_addeq (eq, peqs, peqqty);
        }
  }

  ut_free_1d (&eq);
  ut_free_2d (&bbox, 3);

  return status;
}

// net_domain_fromtess_2d returns the plane equations of the domain of a 2D
// tessellation, i.e. those of its edges and of the z planes of a thin domain
// (see net_domain_cubeparms).  Returns 0 on success and -1 otherwise.
int
net_domain_fromtess_2d (struct TESS Tess, double ***peqs, int *peqqty,
                        struct POLY *pPoly)
{
  int i, j, ver;
  double thickness, size = 0, z, norm;
  double *c = ut_alloc_1d (3), *v = ut_alloc_1d (3), **bbox = ut_alloc_2d (3, 2);

  if (Tess.DomVerQty == 0)
  {
    ut_free_1d (&c);
    ut_free_1d (&v);
    ut_free_2d (&bbox, 3);
    return -1;
  }

  size = net_domain_fromtess_bbox (Tess, bbox);
  z = Tess.DomVerCoo[1][2];

  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    if (fabs (Tess.DomVerCoo[i][2] - z) > 1e-6 * size)
      size = 0;
    ut_array_1d_add (c, Tess.DomVerCoo[i], 3, c);
  }
  ut_array_1d_scale (c, 3, 1. / Tess.DomVerQty);

  for (i = 1; i <= Tess.DomEdgeQty; i++)
    if (Tess.DomEdgeVerQty[i] != 2 || strcmp (Tess.DomEdgeType[i], "line"))
      size = 0;

  if (size == 0)
  {
    ut_free_1d (&c);
    ut_free_1d (&v);
    ut_free_2d (&bbox, 3);
    return -1;
  }

  (*peqqty) = Tess.DomEdgeQty + 2;
  (*peqs) = ut_alloc_2d (*peqqty, 4);

  thickness = 1e-6 * ut_num_min (bbox[0][1] - bbox[0][0], bbox[1][1] - bbox[1][0]);

  (*peqs)[0][0] = -z;
  (*peqs)[0][3] = -1;
  (*peqs)[1][0] = z + thickness;
  (*peqs)[1][3] = 1;

  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    double *eq = (*peqs)[i + 1];

    ver = Tess.DomEdgeVerNb[i][0];
    ut_array_1d_sub (Tess.DomVerCoo[ver], Tess.DomVerCoo[Tess.DomEdgeVerNb[i][1]], 3, v);

    norm = ut_array_1d_norm (v, 2);
    eq[1] = v[1] / norm;
    eq[2] = -v[0] / norm;
    for (j = 0; j < 2; j++)
      eq[0] += eq[j + 1] * Tess.DomVerCoo[ver][j];

    // normal pointing outwards
    if (eq[1] * c[0] + eq[2] * c[1] > eq[0])
      ut_array_1d_scale (eq, 4, -1);
  }

  (*pPoly).PseudoDim = 2;
  (*pPoly).PseudoSize = thickness;

  ut_free_1d (&c);
  ut_free_1d (&v);
  ut_free_2d (&bbox, 3);

  return 0;
}

// net_domain_fromtess_convex returns 0 if all domain vertices are in the
// half-spaces, and -1 otherwise (the domain is not convex).  The tessellation
// vertices cannot be used, as periodic cells extend beyond the domain.
int
net_domain_fromtess_convex (struct TESS Tess, double **eqs, int eqqty)
{
  int i, j, status = 0;
  double eps, **bbox = ut_alloc_2d (3, 2);

  eps = 1e-6 * net_domain_fromtess_bbox (Tess, bbox);

  for (i = 0; i < eqqty && !status; i++)
    for (j = 1; j <= Tess.DomVerQty && !status; j++)
      if (ut_vector_scalprod (eqs[i] + 1, Tess.DomVerCoo[j]) > eqs[i][0] + eps)
        status = -1;

  ut_free_2d (&bbox, 3);

  return status;
}

// net_domain_curved returns 1 if the domain is to be generated as a faceted
// hull, with its primitives recorded so that the tessellation can be cut by
// them at the end (see net_domain_cut), and 0 if it is to be faceted.  This
// applies to the cylinder, sphere and hourglass domains of single-scale
// 3D tessellations, except for a cylinder or a sphere whose number of
// facets is provided, for a domain transformed otherwise than by
// translations and rotations, and for a periodic domain, unless it is an
// untransformed cylinder periodic along z only (strings are the domain
// expression and its transformations).
int
net_domain_curved (struct IN_T In, char **strings, int stringqty,
                   char *domain)
{
  int i, varqty, status = 1;
  char **vars = NULL;

  if (strcmp (domain, "cylinder") && strcmp (domain, "sphere")
      && strcmp (domain, "hourglass"))
    return 0;

  if (In.dim != 3 || In.levelqty != 1)
    return 0;

  // periodicity is only possible along the (z) axis of an untransformed
  // cylinder, which is invariant by translation along it (periodic cells
  // extend beyond the domain ends)
  if (In.periodic && ut_array_1d_int_sum (In.periodic, 3) > 0
      && (strcmp (domain, "cylinder") || In.periodic[0] || In.periodic[1]
          || stringqty > 1))
    return 0;

  // translations and rotations are applied to the primitives as well
  for (i = 1; i < stringqty; i++)
    if (strncmp (strings[i], "translate(", 10)
        && strncmp (strings[i], "rotate(", 7))
      return 0;

  ut_string_function (strings[0], NULL, NULL, &vars, &varqty);

  if ((!strcmp (domain, "cylinder") && varqty == 3)
      || (!strcmp (domain, "sphere") && varqty == 2))
    status = 0;

  ut_free_2d_char (&vars, varqty);

  return status;
}

void
net_domain_cylinder_string (char *domain, char *nstring, int curved,
                            struct POLY *pPoly, struct PRIM **pPrim,
                            int *pPrimQty)
{
  int pseudodim;
  double pseudosize;
  double *parms = ut_alloc_1d (10);

  net_domain_cylinderparms (domain, nstring, parms, &pseudodim, &pseudosize);
  net_domain_cylinder (parms, curved, pPoly);

  if (curved)
    net_domain_cylinder_prim (parms, pPrim, pPrimQty);

  if (pseudodim != -1)
  {
    (*pPoly).PseudoDim = pseudodim;
    (*pPoly).PseudoSize = pseudosize;
  }

  ut_free_1d (&parms);

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

  ut_string_function (domain, &fct, NULL, &vars, &varqty);

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
    ut_print_neperbug ();

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
      ut_print_messagewnc (2, 72,
                           "The number of facets must be specified in `-morpho cylinder' when using `-n from_morpho'.");

    ut_math_eval (nstring, 1, vars, vals, &res);
    res = ut_num_max (res, 1);

    ut_free_2d_char (&vars, varqty);
    ut_free_1d (&vals);

    if (!strcmp (fct, "cylinder"))
      rcl2cl (1, 3, M_PI * parms[0] * parms[0] * parms[1] / 4,
              ut_num_d2ri (res), NULL, &cl);
    else if (!strcmp (fct, "circle"))
      rcl2cl (1, 3, M_PI * parms[1] / 4, ut_num_d2ri (res), NULL, &cl);
    else
      abort ();

    parms[2] = (int) floor (M_PI * parms[1] / cl);
    parms[2] = ut_num_max_int (parms[2], 12);
  }
  parms[2] += 2;

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, varqty);

  return;
}

void
net_domain_cylinder (double *parms, int curved, struct POLY *pPoly)
{
  double **eq = ut_alloc_2d (ut_num_d2ri (parms[2]), 4);

  if (!curved)
    net_domain_cylinder_planes (parms[0], parms[1] / 2, parms[2] - 2, eq);
  else
    net_domain_cylinder_hullplanes (parms[0], parms[1] / 2, parms[2] - 2, eq);
  net_domain_clip (pPoly, eq, parms[2]);
  ut_free_2d (&eq, parms[2]);

  return;
}

// The hourglass is generated from a faceted cylinder of the same height
// and end diameter, which is cut by a torus.
void
net_domain_hourglass_string (char *domain, char *nstring, struct POLY *pPoly,
                             struct PRIM **pPrim, int *pPrimQty)
{
  double *parms = ut_alloc_1d (10);

  net_domain_hourglassparms (domain, nstring, parms);
  net_domain_cylinder (parms, 1, pPoly);
  net_domain_hourglass_prim (parms, pPrim, pPrimQty);

  ut_free_1d (&parms);

  return;
}

// parms are those of the cylinder (height, end diameter and number of
// facets + 2), followed by the centre diameter (parms[3])
void
net_domain_hourglassparms (char *domain, char *nstring, double *parms)
{
  int varqty, pseudodim;
  double pseudosize;
  char **vars = NULL;
  char *cylinder = ut_alloc_1d_char (strlen (domain) + 10);

  ut_string_function (domain, NULL, NULL, &vars, &varqty);

  if (varqty != 3)
    ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

  sprintf (cylinder, "cylinder(%s,%s,%d)", vars[0], vars[1],
           NET_DOMAIN_HOURGLASS_FACETQTY);

  net_domain_cylinderparms (cylinder, nstring, parms, &pseudodim,
                            &pseudosize);

  ut_string_real (vars[2], parms + 3);

  if (parms[0] <= 0 || parms[3] <= 0 || parms[3] >= parms[1])
    ut_print_message (2, 0,
                      "`%s': the centre diameter must be positive and lower than the end diameter.\n",
                      domain);

  ut_free_1d_char (&cylinder);
  ut_free_2d_char (&vars, varqty);

  return;
}

void
net_domain_sphere_string (char *domain, int curved, struct POLY *pPoly,
                          struct PRIM **pPrim, int *pPrimQty)
{
  double *parms = ut_alloc_1d (2);

  net_domain_sphereparms (domain, parms);
  net_domain_sphere (parms, curved, pPoly);

  if (curved)
    net_domain_sphere_prim (parms, pPrim, pPrimQty);

  ut_free_1d (&parms);

  return;
}

void
net_domain_sphereparms (char *domain, double *parms)
{
  int i, varqty;
  char **vars = NULL;

  parms[1] = 100;

  ut_string_function (domain, NULL, NULL, &vars, &varqty);

  if (varqty != 1 && varqty != 2)
    ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

  for (i = 0; i < varqty; i++)
    ut_string_real (vars[i], parms + i);

  if (parms[1] != -1 && parms[1] < 4)
  {
    ut_print_message (1, 2, "Increasing the number of facets to 4.\n");
    parms[1] = 4;
  }

  ut_free_2d_char (&vars, varqty);

  return;
}

void
net_domain_sphere (double *parms, int curved, struct POLY *pPoly)
{
  double **eq = ut_alloc_2d (parms[1], 4);

  if (!curved)
    net_domain_sphere_planes (parms[0] / 2, parms[1], eq);
  else
    net_domain_sphere_hullplanes (parms[0] / 2, parms[1], eq);
  net_domain_clip (pPoly, eq, parms[1]);

  ut_free_2d (&eq, parms[1]);

  return;
}

void
net_domain_rodrigues_string (char *domain, struct POLY *pPoly)
{
  char *crysym = NULL;

  net_domain_rodriguesparms (domain, &crysym);
  net_domain_rodrigues (crysym, pPoly);

  ut_free_1d_char (&crysym);

  return;
}

void
net_domain_rodriguesparms (char *domain, char **pcrysym)
{
  int varqty;
  char **vars = NULL;

  ut_string_function (domain, NULL, NULL, &vars, &varqty);

  if (varqty != 1)
    ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

  ut_string_string (vars[0], pcrysym);

  ut_free_2d_char (&vars, varqty);

  return;
}

void
net_domain_rodrigues (char *crysym, struct POLY *pPoly)
{
  int i, eqqty, n = ol_crysym_qty (crysym);
  double **eq = NULL;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();
  double *rtheta = ol_rtheta_alloc ();
  double theta, thetamax;
  double **cubesize = ut_alloc_2d (3, 2);
  srand48 (1);

  thetamax = tan (0.99 * M_PI * 0.5);
  for (i = 0; i < 3; i++)
    ut_array_1d_set_2 (cubesize[i], -thetamax, thetamax);
  net_domain_cube (cubesize, pPoly);

  if (!strcmp (crysym, "m-3"))
    ut_print_message (1, 2,
                      "Face positions are randomized a bit to avoid degeneracy.\n");

  eqqty = 0;
  for (i = 2; i <= n; i++)
  {
    ol_q_crysym (q1, crysym, i, q2);
    ol_q_rtheta_rad (q2, rtheta);
    theta = rtheta[3];
    if (!strcmp (crysym, "m-3"))
      theta += 1e-3 * (2 * drand48 () - 1);

    eq = ut_realloc_2d_addline (eq, ++eqqty, 4);

    ut_array_1d_memcpy (rtheta, 3, eq[eqqty - 1] + 1);
    eq[eqqty - 1][0] = tan (theta / 4);
    if (eq[eqqty - 1][0] < 0)
      ut_array_1d_scale (eq[eqqty - 1], 4, -1);

    if (theta > M_PI - OL_EPS)
    {
      eq = ut_realloc_2d_addline (eq, ++eqqty, 4);
      ut_array_1d_memcpy (eq[eqqty - 2], 4, eq[eqqty - 1]);
      ut_array_1d_scale (eq[eqqty - 1] + 1, 3, -1);
    }
  }

  if (eqqty > 0)
    net_domain_clip (pPoly, eq, eqqty);

  ut_free_2d (&cubesize, 3);
  ol_rtheta_free (&rtheta);
  ol_q_free (&q1);
  ol_q_free (&q2);
  ut_free_2d (&eq, eqqty);

  return;
}

void
net_domain_planes_string (char *domain, int dim, struct POLY *pPoly)
{
  int eqqty;
  double **eqs = NULL;

  net_domain_planesparms (domain, dim, &eqs, &eqqty);
  net_domain_planes (eqs, eqqty, pPoly);

  ut_free_2d (&eqs, eqqty);

  return;
}

void
net_domain_planesparms (char *domain, int dim, double ***peqs, int *peqqty)
{
  int i, varqty, shift;
  char **vars = NULL, *filename = NULL;
  FILE *file = NULL;

  ut_string_function (domain, NULL, NULL, &vars, &varqty);

  if (varqty != 1)
    ut_print_message (2, 0, "Unknown expression `%s'.\n", domain);

  ut_string_string (vars[0], &filename);

  file = ut_file_open (filename, "r");

  if (fscanf (file, "%d", peqqty) != 1)
    abort ();

  if (dim == 2)
  {
    shift = 2;
    (*peqqty) += 2;
  }
  else
    shift = 0;

  (*peqs) = ut_alloc_2d (*peqqty, 4);

  // recording z0 and z1 faces
  if (dim == 2)
  {
    (*peqs)[0][3] = -1;
    (*peqs)[1][3] = 1;
    (*peqs)[1][0] = 1e-6;
  }

  // recording faces
  for (i = shift; i < *peqqty; i++)
  {
    ut_array_1d_fscanf (file, (*peqs)[i], 4);
    if (ut_array_1d_norm ((*peqs)[i] + 1, 3) == 0)
      ut_print_message (2, 3, "Face %d: normal norm is zero.\n", i - 1);
    ut_array_1d_scale ((*peqs)[i], 4, 1. / ut_vector_norm ((*peqs)[i] + 1));
  }

  ut_free_2d_char (&vars, varqty);
  ut_file_close (file, filename, "r");
  ut_free_1d_char (&filename);

  return;
}

void
net_domain_planes (double **eqs, int eqqty, struct POLY *pPoly)
{
  net_domain_clip (pPoly, eqs, eqqty);

  return;
}

void
net_domain_transform (struct TESS *pPoly, int dim, char *string)
{
  int i, dir;
  double *v = ol_rtheta_alloc ();
  double **g = ol_g_alloc ();
  double *eq = ut_alloc_1d (4);
  double *coo = ut_alloc_1d (3);
  char *fct = NULL;
  char **vars = NULL;
  int varqty = 0;

  ut_string_function (string, &fct, NULL, &vars, &varqty);

  if (!strcmp (fct, "rotate"))
  {
    if ((dim == 3 && varqty != 4) || (dim == 2 && varqty != 1))
      ut_print_message (2, 3, "Failed to parse expression `%s'.\n", string);

    ut_array_1d_set_3 (v, 0., 0., 1.);

    for (i = 0; i < varqty - 1; i++)
      ut_string_real (vars[i], v + i);
    ut_string_real (vars[varqty - 1], v + 3);

    ol_r_set_unit (v);
    ol_rtheta_g (v, g);
    neut_tess_rotate (pPoly, g);
  }

  else if (!strcmp (fct, "translate"))
  {
    if (varqty != dim)
      ut_print_message (2, 3, "Failed to parse expression `%s'.\n", string);

    ut_array_1d_set (v, 3, 0.);

    for (i = 0; i < dim; i++)
      ut_string_real (vars[i], v + i);

    neut_tess_shift (pPoly, v[0], v[1], v[2]);
  }

  else if (!strcmp (fct, "scale"))
  {
    if (varqty != dim)
      ut_print_message (2, 3, "Failed to parse expression `%s'.\n", string);

    ut_array_1d_set (v, 3, 1.);

    for (i = 0; i < dim; i++)
      ut_string_real (vars[i], v + i);

    neut_tess_scale (pPoly, v[0], v[1], v[2]);
  }

  else if (!strcmp (fct, "split"))
  {
    if (varqty != 1)
      ut_print_message (2, 3, "Failed to parse expression `%s'.\n", string);

    double **bbox = ut_alloc_2d (3, 2);
    char *domtype = NULL;
    neut_tess_bbox (*pPoly, bbox);
    dir = vars[0][0] - 'x';
    if (dir < 0 || dir > 2)
      ut_print_message (2, 3, "Cannot read `%s'.\n", string);

    eq[0] = -0.5 * (bbox[dir][1] + bbox[dir][0]);
    eq[dir + 1] = -1;
    ut_string_string ((*pPoly).DomType, &domtype);
    net_tess_clip (pPoly, eq);
    if (!strcmp (domtype, "cube"))
      ut_string_string (domtype, &(*pPoly).DomType);
    ut_free_1d_char (&domtype);
  }

  else if (!strcmp (fct, "cell"))
  {
    if (varqty != 1)
      ut_print_message (2, 3, "Failed to parse expression `%s'.\n", string);

    neut_tess_poly_tess (*pPoly, atoi (vars[0]), pPoly);
  }

  else
    abort ();

  ol_rtheta_free (&v);
  ol_g_free (&g);
  ut_free_1d (&eq);
  ut_free_1d (&coo);
  ut_free_2d_char (&vars, varqty);

  return;
}
