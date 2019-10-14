/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
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
  ut_free_1d_char ((*pPrim).Type);
  ut_free_1d ((*pPrim).Parms);
  ut_free_1d ((*pPrim).Eq);
  ut_free_1d ((*pPrim).Base);
  ut_free_1d ((*pPrim).Dir);
  ut_free_1d ((*pPrim).Rad);

  return;
}

int
neut_prim_sscanf (char *expr, struct PRIM *pPrim)
{
  int i, status = 0, valqty;
  char **vals = NULL;

  ut_string_function_separate_exprs (expr, &(*pPrim).Type, &vals, &valqty);

  if (!strcmp ((*pPrim).Type, "hspace")
   || !strcmp ((*pPrim).Type, "hspacei"))
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

      ut_array_1d_scale ((*pPrim).Eq, 4, 1. / ut_array_1d_norm ((*pPrim).Eq + 1, 3));
      ut_array_1d_memcpy ((*pPrim).Parms, (*pPrim).ParmQty, (*pPrim).Eq);
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

  ut_free_2d_char (vals, valqty);

  return status;
}

void
neut_prim_point_side (struct PRIM Prim, double *coo, int *pside)
{
  double dist;

  if (!strcmp (Prim.Type, "hspace"))
    *pside = -ut_space_planeside (Prim.Eq, coo - 1);
  else if (!strcmp (Prim.Type, "hspacei"))
    *pside = ut_space_planeside (Prim.Eq, coo - 1);
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

    ut_space_point_cyl2_tangentplane (coo, Prim.Base, Prim.Dir,
                                      Prim.Dir + 3, Prim.Dir + 6,
                                      Prim.Rad[0], Prim.Rad[1], plane);
    *pside = ut_space_planeside (plane, coo - 1);

    ut_free_1d (plane);
  }
  else if (!strcmp (Prim.Type, "ecylinderi"))
  {
    double *plane = ut_alloc_1d (4);

    ut_space_point_cyl2_tangentplane (coo, Prim.Base, Prim.Dir,
                                      Prim.Dir + 3, Prim.Dir + 6,
                                      Prim.Rad[0], Prim.Rad[1], plane);
    *pside = - ut_space_planeside (plane, coo - 1);

    ut_free_1d (plane);
  }
  else if (!strcmp (Prim.Type, "torus"))
  {
    ut_space_point_circle_dist (coo, Prim.Base, Prim.Dir, Prim.Rad[0], &dist, NULL);
    *pside = dist > Prim.Rad[1] ? 1 : -1;
  }
  else if (!strcmp (Prim.Type, "torusi"))
  {
    ut_space_point_circle_dist (coo, Prim.Base, Prim.Dir, Prim.Rad[0], &dist, NULL);
    *pside = dist < Prim.Rad[1] ? 1 : -1;
  }

  return;
}

void
neut_prim_point_mirror (struct PRIM Prim, double *coo, double *mirror)
{
  if (!strcmp (Prim.Type, "hspace")
   || !strcmp (Prim.Type, "hspacei"))
    ut_space_point_plane_mirror (coo, Prim.Eq, mirror);
  else if (!strcmp (Prim.Type, "sphere")
        || !strcmp (Prim.Type, "spherei"))
    ut_space_point_sphere_mirror (coo, Prim.Base, Prim.Rad[0], mirror);
  else if (!strcmp (Prim.Type, "cylinder")
        || !strcmp (Prim.Type, "cylinderi"))
    ut_space_point_cyl_mirror (coo, Prim.Base, Prim.Dir, Prim.Rad[0], mirror);
  else if (!strcmp (Prim.Type, "ecylinder")
        || !strcmp (Prim.Type, "ecylinderi"))
    ut_space_point_cyl2_mirror (coo, Prim.Base, Prim.Dir, Prim.Dir + 3, Prim.Dir + 6,
                                Prim.Rad[0], Prim.Rad[1], mirror);
  else if (!strcmp (Prim.Type, "torus")
        || !strcmp (Prim.Type, "torusi"))
    ut_space_point_torus_mirror (coo, Prim.Base, Prim.Dir, Prim.Rad[0], Prim.Rad[1], mirror);
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
neut_primparms_point_proj (char* type, double *parms, double *coo, double *proj)
{
  double dist, *coo2 = ut_alloc_1d (3);

  ut_array_1d_memcpy (coo2, 3, coo);

  if (!strcmp (type, "plane")
   || !strcmp (type, "planei")
   || !strcmp (type, "hspace")
   || !strcmp (type, "hspacei"))
    ut_space_point_plane_proj (coo, parms, proj);
  else if (!strcmp (type, "sphere")
        || !strcmp (type, "spherei"))
    ut_space_point_sphere_proj (coo, parms, parms[3], proj);
  else if (!strcmp (type, "cylinder")
        || !strcmp (type, "cylinderi"))
    ut_space_point_cyl_proj (coo, parms, parms + 3, parms[6], proj);
  else if (!strcmp (type, "ecylinder")
        || !strcmp (type, "ecylinderi"))
    ut_space_point_cyl2_proj (coo, parms, parms + 3, parms + 6, parms + 9,
                              parms[12], parms[13], proj);
  else if (!strcmp (type, "torus")
        || !strcmp (type, "torusi"))
    ut_space_point_torus_proj (coo, parms, parms + 3, parms[6], parms[7], proj);
  else
  {
    printf ("type = %s\n", type);
    abort ();
  }

  dist = ut_space_dist (coo2, proj);

  ut_free_1d (coo2);

  return dist;
}

void
neut_primparms_point_tangentplane (char* type, double *parms, double *coo, double *plane)
{
  if (!strcmp (type, "plane")
   || !strcmp (type, "planei")
   || !strcmp (type, "hspace")
   || !strcmp (type, "hspacei"))
    ut_array_1d_memcpy (plane, 4, parms);
  else if (!strcmp (type, "sphere")
        || !strcmp (type, "spherei"))
    ut_space_point_sphere_tangentplane (coo, parms, parms[3], plane);
  else if (!strcmp (type, "cylinder")
        || !strcmp (type, "cylinderi"))
    ut_space_point_cyl_tangentplane (coo, parms, parms + 3, parms[6], plane);
  else if (!strcmp (type, "ecylinder")
        || !strcmp (type, "ecylinderi"))
    ut_space_point_cyl2_tangentplane (coo, parms, parms + 3, parms + 6, parms + 9,
                                      parms[12], parms[13], plane);
  else if (!strcmp (type, "torus")
        || !strcmp (type, "torusi"))
    ut_space_point_torus_tangentplane (coo, parms, parms + 3, parms[6], parms[7], plane);
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
