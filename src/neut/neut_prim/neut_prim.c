/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_prim_.h"

void
neut_prim_set_zero (struct PRIM *pPrim)
{
  (*pPrim).Type = NULL;
  (*pPrim).Parms = NULL;
  (*pPrim).ParmQty = 0;
  (*pPrim).Eq = NULL;
  (*pPrim).Base = NULL;
  (*pPrim).Dir = NULL;
  (*pPrim).Rad = NULL;

  return;
}

void
neut_prim_free (struct PRIM *pPrim)
{
  ut_free_1d_char (&(*pPrim).Type);
  ut_free_1d (&(*pPrim).Parms);
  ut_free_1d (&(*pPrim).Eq);
  ut_free_1d (&(*pPrim).Base);
  ut_free_1d (&(*pPrim).Dir);
  ut_free_1d (&(*pPrim).Rad);

  return;
}

void
neut_prim_memcpy (struct PRIM PrimA, struct PRIM *pPrimB)
{
  int dirqty, radqty;

  neut_prim_free (pPrimB);

  ut_string_string (PrimA.Type, &(*pPrimB).Type);

  (*pPrimB).ParmQty = PrimA.ParmQty;
  if (PrimA.Parms)
  {
    (*pPrimB).Parms = ut_alloc_1d (PrimA.ParmQty);
    ut_array_1d_memcpy (PrimA.Parms, PrimA.ParmQty, (*pPrimB).Parms);
  }

  // array sizes are those of neut_prim_sscanf
  dirqty = !strncmp (PrimA.Type, "ecylinder", 9) ? 9 : 3;
  radqty = !strncmp (PrimA.Type, "ecylinder", 9) ? 3
         : !strncmp (PrimA.Type, "torus", 5) ? 2 : 1;

  if (PrimA.Eq)
  {
    (*pPrimB).Eq = ut_alloc_1d (4);
    ut_array_1d_memcpy (PrimA.Eq, 4, (*pPrimB).Eq);
  }

  if (PrimA.Base)
  {
    (*pPrimB).Base = ut_alloc_1d (3);
    ut_array_1d_memcpy (PrimA.Base, 3, (*pPrimB).Base);
  }

  if (PrimA.Dir)
  {
    (*pPrimB).Dir = ut_alloc_1d (dirqty);
    ut_array_1d_memcpy (PrimA.Dir, dirqty, (*pPrimB).Dir);
  }

  if (PrimA.Rad)
  {
    (*pPrimB).Rad = ut_alloc_1d (radqty);
    ut_array_1d_memcpy (PrimA.Rad, radqty, (*pPrimB).Rad);
  }

  return;
}

// type is "hspace" or "hspacei", as in neut_prim_sscanf; eq is not normalized
void
neut_prim_set_hspace (char *type, double *eq, struct PRIM *pPrim)
{
  ut_string_string (type, &(*pPrim).Type);

  (*pPrim).ParmQty = 4;
  (*pPrim).Parms = ut_alloc_1d ((*pPrim).ParmQty);

  (*pPrim).Eq = ut_alloc_1d (4);
  ut_array_1d_memcpy (eq, 4, (*pPrim).Eq);
  if (!strcmp ((*pPrim).Type, "hspacei"))
    ut_array_1d_scale ((*pPrim).Eq, 4, -1);
  ut_array_1d_memcpy ((*pPrim).Eq, (*pPrim).ParmQty, (*pPrim).Parms);

  return;
}

// neut_prim_shift translates a primitive by shift (not for cubes).
void
neut_prim_shift (struct PRIM *pPrim, double *shift)
{
  int i;

  if (!strncmp ((*pPrim).Type, "cube", 4))
    ut_print_neperbug ();

  if ((*pPrim).Eq)
  {
    (*pPrim).Eq[0] += ut_array_1d_scalprod (shift, (*pPrim).Eq + 1, 3);
    (*pPrim).Parms[0] = (*pPrim).Eq[0];
  }

  // the base point is the first 3 parameters
  if ((*pPrim).Base)
    for (i = 0; i < 3; i++)
    {
      (*pPrim).Base[i] += shift[i];
      (*pPrim).Parms[i] = (*pPrim).Base[i];
    }

  return;
}

// neut_prim_rotate rotates a primitive about the origin, a vector v
// becoming g v (not for cubes).
void
neut_prim_rotate (struct PRIM *pPrim, double **g)
{
  int i, dirqty;

  if (!strncmp ((*pPrim).Type, "cube", 4))
    ut_print_neperbug ();

  if ((*pPrim).Eq)
  {
    ut_mat_vect_product (g, 3, 3, (*pPrim).Eq + 1, 3, (*pPrim).Eq + 1);
    ut_array_1d_memcpy ((*pPrim).Eq, 4, (*pPrim).Parms);
  }

  if ((*pPrim).Base)
  {
    ut_mat_vect_product (g, 3, 3, (*pPrim).Base, 3, (*pPrim).Base);
    ut_array_1d_memcpy ((*pPrim).Base, 3, (*pPrim).Parms);
  }

  // the direction vectors are the parameters that follow the base point
  if ((*pPrim).Dir)
  {
    dirqty = !strncmp ((*pPrim).Type, "ecylinder", 9) ? 9 : 3;
    for (i = 0; i < dirqty; i += 3)
    {
      ut_mat_vect_product (g, 3, 3, (*pPrim).Dir + i, 3, (*pPrim).Dir + i);
      ut_array_1d_memcpy ((*pPrim).Dir + i, 3, (*pPrim).Parms + 3 + i);
    }
  }

  return;
}

int
neut_prim_sscanf (char *expr, struct PRIM *pPrim)
{
  int i, status = 0, valqty;
  char **vals = NULL;

  ut_string_function_expr (expr, &(*pPrim).Type, &vals, &valqty);

  if (!strcmp ((*pPrim).Type, "hspace") || !strcmp ((*pPrim).Type, "hspacei"))
  {
    if (valqty != 4)
      status = -1;
    else
    {
      (*pPrim).ParmQty = 4;
      (*pPrim).Parms = ut_alloc_1d ((*pPrim).ParmQty);

      (*pPrim).Eq = ut_alloc_1d (4);
      for (i = 0; i < 4; i++)
        sscanf (vals[i], "%lf", (*pPrim).Eq + i);

      ut_array_1d_scale ((*pPrim).Eq, 4,
                         1. / ut_array_1d_norm ((*pPrim).Eq + 1, 3));
      if (!strcmp ((*pPrim).Type, "hspacei"))
        ut_array_1d_scale ((*pPrim).Eq, 4, -1);
      ut_array_1d_memcpy ((*pPrim).Eq, (*pPrim).ParmQty, (*pPrim).Parms);
    }
  }

  else if (!strcmp ((*pPrim).Type, "sphere")
           || !strcmp ((*pPrim).Type, "spherei"))
  {
    if (valqty != 4)
      status = -1;
    else
    {
      (*pPrim).ParmQty = 4;
      (*pPrim).Parms = ut_alloc_1d ((*pPrim).ParmQty);
      (*pPrim).Base = ut_alloc_1d (3);
      (*pPrim).Rad = ut_alloc_1d (1);
      for (i = 0; i < (*pPrim).ParmQty; i++)
        sscanf (vals[i], "%lf", (*pPrim).Parms + i);
      for (i = 0; i < 3; i++)
        sscanf (vals[i], "%lf", (*pPrim).Base + i);
      sscanf (vals[3], "%lf", (*pPrim).Rad);
    }
  }

  else if (!strcmp ((*pPrim).Type, "cube")
           || !strcmp ((*pPrim).Type, "cubei"))
  {
    if (valqty != 6 && valqty != 7)
      status = -1;
    else
    {
      (*pPrim).ParmQty = 7;
      (*pPrim).Parms = ut_alloc_1d ((*pPrim).ParmQty);
      (*pPrim).Base = ut_alloc_1d (3);
      (*pPrim).Rad = ut_alloc_1d (1);
      for (i = 0; i < valqty; i++)
        sscanf (vals[i], "%lf", (*pPrim).Parms + i);
      for (i = 0; i < 3; i++)
        (*pPrim).Base[i] = (*pPrim).Parms[i] + (*pPrim).Parms[i + 3];
      (*pPrim).Rad[0] = (*pPrim).Parms[6];
    }
  }

  else if (!strcmp ((*pPrim).Type, "cylinder")
           || !strcmp ((*pPrim).Type, "cylinderi"))
  {
    if (valqty != 7)
      status = -1;
    else
    {
      (*pPrim).ParmQty = 7;
      (*pPrim).Parms = ut_alloc_1d ((*pPrim).ParmQty);
      (*pPrim).Base = ut_alloc_1d (3);
      (*pPrim).Rad = ut_alloc_1d (1);
      (*pPrim).Dir = ut_alloc_1d (3);
      for (i = 0; i < (*pPrim).ParmQty; i++)
        sscanf (vals[i], "%lf", (*pPrim).Parms + i);
      ut_array_1d_normalize ((*pPrim).Parms + 3, 3);
      for (i = 0; i < 3; i++)
        sscanf (vals[i], "%lf", (*pPrim).Base + i);
      for (i = 3; i < 6; i++)
        sscanf (vals[i], "%lf", (*pPrim).Dir + i - 3);
      ut_array_1d_normalize ((*pPrim).Dir, 3);
      sscanf (vals[6], "%lf", (*pPrim).Rad);
    }
  }

  else if (!strcmp ((*pPrim).Type, "ecylinder")
           || !strcmp ((*pPrim).Type, "ecylinderi"))
  {
    if (valqty != 14)
      status = -1;
    else
    {
      (*pPrim).ParmQty = 14;
      (*pPrim).Parms = ut_alloc_1d ((*pPrim).ParmQty);
      (*pPrim).Base = ut_alloc_1d (3);
      (*pPrim).Rad = ut_alloc_1d (3);
      (*pPrim).Dir = ut_alloc_1d (9);
      for (i = 0; i < (*pPrim).ParmQty; i++)
        sscanf (vals[i], "%lf", (*pPrim).Parms + i);
      ut_array_1d_normalize ((*pPrim).Parms + 3, 3);
      for (i = 0; i < 3; i++)
        sscanf (vals[i], "%lf", (*pPrim).Base + i);
      for (i = 3; i < 12; i++)
        sscanf (vals[i], "%lf", (*pPrim).Dir + i - 3);
      ut_array_1d_normalize ((*pPrim).Dir, 3);
      ut_array_1d_normalize ((*pPrim).Dir + 3, 3);
      ut_array_1d_normalize ((*pPrim).Dir + 6, 3);
      for (i = 12; i < 14; i++)
        sscanf (vals[i], "%lf", (*pPrim).Rad + i - 12);
    }
  }

  else if (!strcmp ((*pPrim).Type, "torus")
           || !strcmp ((*pPrim).Type, "torusi"))
  {
    if (valqty != 8)
      status = -1;
    else
    {
      (*pPrim).ParmQty = 8;
      (*pPrim).Parms = ut_alloc_1d ((*pPrim).ParmQty);
      (*pPrim).Base = ut_alloc_1d (3);
      (*pPrim).Rad = ut_alloc_1d (2);
      (*pPrim).Dir = ut_alloc_1d (3);
      for (i = 0; i < (*pPrim).ParmQty; i++)
        sscanf (vals[i], "%lf", (*pPrim).Parms + i);
      ut_array_1d_normalize ((*pPrim).Parms + 3, 3);
      for (i = 0; i < 3; i++)
        sscanf (vals[i], "%lf", (*pPrim).Base + i);
      for (i = 3; i < 6; i++)
        sscanf (vals[i], "%lf", (*pPrim).Dir + i - 3);
      ut_array_1d_normalize ((*pPrim).Dir, 3);
      sscanf (vals[6], "%lf", (*pPrim).Rad);
      sscanf (vals[7], "%lf", (*pPrim).Rad + 1);
    }
  }

  ut_free_2d_char (&vals, valqty);

  return status;
}

void
neut_prim_point_side (struct PRIM Prim, double *coo, int *pside)
{
  double dist;

  if (!strcmp (Prim.Type, "hspace"))
    *pside = ut_space_point_plane_side (coo - 1, Prim.Eq);
  else if (!strcmp (Prim.Type, "hspacei"))
    *pside = ut_space_point_plane_side (coo - 1, Prim.Eq);
  else if (!strcmp (Prim.Type, "sphere"))
    *pside = ut_space_dist (coo, Prim.Base) > Prim.Rad[0] ? 1 : -1;
  else if (!strcmp (Prim.Type, "spherei"))
    *pside = ut_space_dist (coo, Prim.Base) < Prim.Rad[0] ? 1 : -1;
  else if (!strcmp (Prim.Type, "cylinder"))
  {
    ut_space_point_line_dist (coo, Prim.Base, Prim.Dir, &dist);
    *pside = dist > Prim.Rad[0] ? 1 : -1;
  }
  else if (!strcmp (Prim.Type, "cylinderi"))
  {
    ut_space_point_line_dist (coo, Prim.Base, Prim.Dir, &dist);
    *pside = dist < Prim.Rad[0] ? 1 : -1;
  }
  else if (!strcmp (Prim.Type, "ecylinder"))
  {
    double *plane = ut_alloc_1d (4);

    ut_space_point_ecyl_tangentplane (coo, Prim.Base, Prim.Dir, Prim.Dir + 3,
                                      Prim.Dir + 6, Prim.Rad[0], Prim.Rad[1],
                                      plane);
    *pside = ut_space_point_plane_side (coo - 1, plane);

    ut_free_1d (&plane);
  }
  else if (!strcmp (Prim.Type, "ecylinderi"))
  {
    double *plane = ut_alloc_1d (4);

    ut_space_point_ecyl_tangentplane (coo, Prim.Base, Prim.Dir, Prim.Dir + 3,
                                      Prim.Dir + 6, Prim.Rad[0], Prim.Rad[1],
                                      plane);
    *pside = -ut_space_point_plane_side (coo - 1, plane);

    ut_free_1d (&plane);
  }
  else if (!strcmp (Prim.Type, "cube"))
  {
    double *plane = ut_alloc_1d (4);

    ut_space_point_cube_tangentplane (coo, Prim.Parms, plane);
    *pside = ut_space_point_plane_side (coo - 1, plane);

    ut_free_1d (&plane);
  }
  else if (!strcmp (Prim.Type, "cubei"))
  {
    double *plane = ut_alloc_1d (4);

    ut_space_point_cube_tangentplane (coo, Prim.Parms, plane);
    *pside = -ut_space_point_plane_side (coo - 1, plane);

    ut_free_1d (&plane);
  }
  else if (!strcmp (Prim.Type, "torus"))
  {
    ut_space_point_circle_dist (coo, Prim.Base, Prim.Dir, Prim.Rad[0], &dist,
                                NULL);
    *pside = dist > Prim.Rad[1] ? 1 : -1;
  }
  else if (!strcmp (Prim.Type, "torusi"))
  {
    ut_space_point_circle_dist (coo, Prim.Base, Prim.Dir, Prim.Rad[0], &dist,
                                NULL);
    *pside = dist < Prim.Rad[1] ? 1 : -1;
  }

  return;
}

// neut_primparms_point_side is neut_prim_point_side for a primitive given
// by its type and parameters (as recorded for the domain faces).
void
neut_primparms_point_side (char *type, double *parms, double *coo, int *pside)
{
  double dir[9];
  struct PRIM Prim;

  neut_prim_set_zero (&Prim);
  Prim.Type = type;
  Prim.Parms = parms;

  if (!strncmp (type, "hspace", 6))
    Prim.Eq = parms;

  else if (!strncmp (type, "sphere", 6))
  {
    Prim.Base = parms;
    Prim.Rad = parms + 3;
  }

  else if (!strncmp (type, "cylinder", 8) || !strncmp (type, "torus", 5))
  {
    Prim.Base = parms;
    Prim.Dir = parms + 3;
    Prim.Rad = parms + 6;
  }

  else if (!strncmp (type, "ecylinder", 9))
  {
    ut_array_1d_memcpy (parms + 3, 9, dir);
    ut_array_1d_normalize (dir, 3);
    ut_array_1d_normalize (dir + 3, 3);
    ut_array_1d_normalize (dir + 6, 3);
    Prim.Base = parms;
    Prim.Dir = dir;
    Prim.Rad = parms + 12;
  }

  neut_prim_point_side (Prim, coo, pside);

  return;
}

void
neut_prim_point_mirror (struct PRIM Prim, double *coo, double *mirror)
{
  if (!strcmp (Prim.Type, "hspace") || !strcmp (Prim.Type, "hspacei"))
    ut_space_point_plane_mirror (coo, Prim.Eq, mirror);
  else if (!strcmp (Prim.Type, "sphere") || !strcmp (Prim.Type, "spherei"))
    ut_space_point_sphere_mirror (coo, Prim.Base, Prim.Rad[0], mirror);
  else if (!strcmp (Prim.Type, "cylinder")
           || !strcmp (Prim.Type, "cylinderi"))
    ut_space_point_cyl_mirror (coo, Prim.Base, Prim.Dir, Prim.Rad[0], mirror);
  else if (!strcmp (Prim.Type, "cube") || !strcmp (Prim.Type, "cubei"))
    ut_space_point_cube_mirror (coo, Prim.Parms, mirror);
  else if (!strcmp (Prim.Type, "ecylinder")
           || !strcmp (Prim.Type, "ecylinderi"))
    ut_space_point_ecyl_mirror (coo, Prim.Base, Prim.Dir, Prim.Dir + 3,
                                Prim.Dir + 6, Prim.Rad[0], Prim.Rad[1],
                                mirror);
  else if (!strcmp (Prim.Type, "torus") || !strcmp (Prim.Type, "torusi"))
    ut_space_point_torus_mirror (coo, Prim.Base, Prim.Dir, Prim.Rad[0],
                                 Prim.Rad[1], mirror);
  else
    abort ();

  return;
}

double
neut_prim_point_proj (struct PRIM Prim, double *coo, double *proj)
{
  return neut_primparms_point_proj (Prim.Type, Prim.Parms, coo, proj);
}

double
neut_primparms_point_proj (char *type, double *parms, double *coo,
                           double *proj)
{
  double dist, *coo2 = ut_alloc_1d (3);

  ut_array_1d_memcpy (coo, 3, coo2);

  if (!strcmp (type, "plane") || !strcmp (type, "planei")
      || !strcmp (type, "hspace") || !strcmp (type, "hspacei"))
    ut_space_point_plane_proj (coo, parms, proj);
  else if (!strcmp (type, "sphere") || !strcmp (type, "spherei"))
    ut_space_point_sphere_proj (coo, parms, parms[3], proj);
  else if (!strcmp (type, "cylinder") || !strcmp (type, "cylinderi"))
    ut_space_point_cyl_proj (coo, parms, parms + 3, parms[6], proj);
  else if (!strcmp (type, "cube") || !strcmp (type, "cubei"))
    ut_space_point_cube_proj (coo, parms, proj);
  else if (!strcmp (type, "ecylinder") || !strcmp (type, "ecylinderi"))
    ut_space_point_ecyl_proj (coo, parms, parms + 3, parms + 6, parms + 9,
                              parms[12], parms[13], proj);
  else if (!strcmp (type, "torus") || !strcmp (type, "torusi"))
    ut_space_point_torus_proj (coo, parms, parms + 3, parms[6], parms[7],
                               proj);
  else
  {
    printf ("type = %s\n", type);
    abort ();
  }

  dist = ut_space_dist (coo2, proj);

  ut_free_1d (&coo2);

  return dist;
}

void
neut_primparms_point_tangentplane (char *type, double *parms, double *coo,
                                   double *plane)
{
  if (!strcmp (type, "plane") || !strcmp (type, "planei")
      || !strcmp (type, "hspace") || !strcmp (type, "hspacei"))
    ut_array_1d_memcpy (parms, 4, plane);
  else if (!strcmp (type, "sphere") || !strcmp (type, "spherei"))
    ut_space_point_sphere_tangentplane (coo, parms, parms[3], plane);
  else if (!strcmp (type, "cylinder") || !strcmp (type, "cylinderi"))
    ut_space_point_cyl_tangentplane (coo, parms, parms + 3, parms[6], plane);
  else if (!strcmp (type, "cube") || !strcmp (type, "cubei"))
    ut_space_point_cube_tangentplane (coo, parms, plane);
  else if (!strcmp (type, "ecylinder") || !strcmp (type, "ecylinderi"))
    ut_space_point_ecyl_tangentplane (coo, parms, parms + 3, parms + 6,
                                      parms + 9, parms[12], parms[13], plane);
  else if (!strcmp (type, "torus") || !strcmp (type, "torusi"))
    ut_space_point_torus_tangentplane (coo, parms, parms + 3, parms[6],
                                       parms[7], plane);
  else
    abort ();

  return;
}

/*
void
neut_primparms_tangentplane_point_proj (char *type, double *parms,
                                        double *plane, double *point,
                                        double *proj)
{
  if (!strcmp (type, "plane"))
  {

  }

  return;
}

void
neut_primparms_tangentplane_proj_point (char *type, double *parms,
                                        double *plane, double *proj,
                                        double *point)
{


  return;
}
*/
