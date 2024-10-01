/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_point_.h"

void
neut_point_set_zero (struct POINT *pPoint)
{
  (*pPoint).Name = NULL;
  ut_string_string ("point", &(*pPoint).Name);

  (*pPoint).Type = NULL;
  (*pPoint).Dim = 0;
  (*pPoint).Qty = 0;
  (*pPoint).Coo = NULL;
  (*pPoint).Rad = NULL;

  (*pPoint).crysym = NULL;

  return;
}

void
neut_point_free (struct POINT *pPoint)
{
  ut_free_1d_char (&(*pPoint).Name);
  ut_free_1d_char (&(*pPoint).Type);

  ut_free_2d (&(*pPoint).Coo, (*pPoint).Qty + 1);
  ut_free_1d (&(*pPoint).Rad);

  neut_point_set_zero (pPoint);

  ut_free_1d_char (&(*pPoint).crysym);

  return;
}

void
neut_point_centre (struct POINT Point, double *centre)
{
  int i;
  double size, totsize;
  double *tmp = ut_alloc_1d (3);
  int weight = (ut_array_1d_sum (Point.Rad + 1, Point.Qty) != 0);

  ut_array_1d_zero (centre, 3);
  totsize = 0;
  for (i = 1; i <= Point.Qty; i++)
  {
    if (weight)
      neut_point_pt_size (Point, i, &size);
    else
      size = 1;

    ut_array_1d_memcpy (Point.Coo[i], 3, tmp);
    ut_array_1d_scale (tmp, 3, size);

    ut_array_1d_add (centre, tmp, 3, centre);
    totsize += size;
  }
  ut_array_1d_scale (centre, 3, 1. / totsize);

  ut_free_1d (&tmp);

  return;
}

void
neut_point_bbox (struct POINT Point, double **bbox)
{
  int i, j;

  for (i = 0; i < 3; i++)
  {
    bbox[i][0] = DBL_MAX;
    bbox[i][1] = -DBL_MAX;
  }

  for (i = 1; i <= Point.Qty; i++)
    for (j = 0; j < 3; j++)
    {
      bbox[j][0] = ut_num_min (bbox[j][0], Point.Coo[i][j]);
      bbox[j][1] = ut_num_max (bbox[j][1], Point.Coo[i][j]);
    }

  return;
}

int
neut_point_var_val (struct POINT Point, int id, struct TESS Tess,
                    struct NODES Nodes, struct MESH Mesh, char *var,
                    double **pvals, int *pvalqty, char **ptype)
{
  int i, status, tmp;

  if (ptype)
    (*ptype) = ut_alloc_1d_char (10);

  (*pvalqty) = 1;
  *pvals = ut_realloc_1d (*pvals, *pvalqty);

  status = 0;
  if (!strcmp (var, "id"))
  {
    (*pvals)[0] = id;
    if (ptype)
      strcpy (*ptype, "%d");
  }
  else if (!strcmp (var, "x"))
  {
    (*pvals)[0] = Point.Coo[id][0];
    if (ptype)
      strcpy (*ptype, "%f");
  }
  else if (!strcmp (var, "y"))
  {
    (*pvals)[0] = Point.Coo[id][1];
    if (ptype)
      strcpy (*ptype, "%f");
  }
  else if (!strcmp (var, "z"))
  {
    (*pvals)[0] = Point.Coo[id][2];
    if (ptype)
      strcpy (*ptype, "%f");
  }
  else if (!strcmp (var, "rad"))
  {
    (*pvals)[0] = Point.Rad[id];
    if (ptype)
      strcpy (*ptype, "%f");
  }
  else if (!strcmp (var, "poly") || !strcmp (var, "cell"))
  {
    (*pvals)[0] = -1;
    for (i = 1; i <= Tess.PolyQty; i++)
      if (neut_tess_point_inpoly (Tess, Point.Coo[id], i) == 1)
      {
        (*pvals)[0] = i;
        break;
      }
    if (ptype)
      strcpy (*ptype, "%d");
  }
  else if (!strcmp (var, "elt"))
  {
    neut_mesh_point_elt (Nodes, Mesh, Point.Coo[id], &tmp);
    (*pvals)[0] = tmp;
    if (ptype)
      strcpy (*ptype, "%d");
  }
  else if (!strcmp (var, "elset"))
  {
    neut_mesh_point_elset (Nodes, Mesh, Point.Coo[id], NULL, 0, &tmp);
    (*pvals)[0] = tmp;
    if (ptype)
      strcpy (*ptype, "%d");
  }
  else
    status = -1;

  return status;
}

int
neut_point_var_val_one (struct POINT Point, struct TESS Tess,
                        struct NODES Nodes, struct MESH Mesh, int id, char *var,
                        double *pval, char **ptype)
{
  int status, qty;
  double *tmp = NULL;

  status =
    neut_point_var_val (Point, id, Tess, Nodes, Mesh, var, &tmp, &qty, ptype);

  if (qty == 1)
  {
    (*pval) = tmp[0];
    status = 0;
  }
  else
    status = -1;

  ut_free_1d (&tmp);

  return status;
}

void
neut_point_addpoint (struct POINT *pPoint, double *coo, double rad)
{
  (*pPoint).Qty++;
  if ((*pPoint).Qty == 1)
    (*pPoint).Coo = ut_alloc_2d (1, 3);

  (*pPoint).Coo =
    ut_realloc_2d_addline ((*pPoint).Coo, (*pPoint).Qty + 1, 3);
  (*pPoint).Rad =
    ut_realloc_1d ((*pPoint).Rad, (*pPoint).Qty + 1);

  if (coo)
    ut_array_1d_memcpy (coo, 3, (*pPoint).Coo[(*pPoint).Qty]);
  (*pPoint).Rad[(*pPoint).Qty] = rad;

  return;
}

void
neut_point_pt_size (struct POINT Point, int pt, double *psize)
{
  (*psize) = 0;
  if (Point.Dim == 1)
    (*psize) = 2 * Point.Rad[pt];
  else if (Point.Dim == 2)
    (*psize) = M_PI * pow (Point.Rad[pt], 2);
  else if (Point.Dim == 3)
    (*psize) = (4. * M_PI / 3) * pow (Point.Rad[pt], 3);

  return;
}

void
neut_point_shift (struct POINT *pPoint, double *s)
{
  int i;

  for (i = 1; i <= (*pPoint).Qty; i++)
    ut_array_1d_add ((*pPoint).Coo[i], s, 3, (*pPoint).Coo[i]);

  return;
}

void
neut_point_shifttocentre (struct POINT *pPoint)
{
  double *s = ut_alloc_1d (3);

  neut_point_centre (*pPoint, s);
  ut_array_1d_scale (s, 3, -1);
  neut_point_shift (pPoint, s);

  ut_free_1d (&s);

  return;
}

void
neut_point_entity_expr_val (struct POINT Point, struct TESS Tess, struct NODES Nodes,
                            struct MESH Mesh, char *expr, double *val, char **ptype)
{
  int j, k, status, varqty;
  char **vars = NULL;
  double *vals = NULL;
  FILE *file = NULL;
  char *type = NULL;

  if (ptype)
  {
    if (strstr (expr, "."))
      ut_string_string ("%f", ptype);
    else
      ut_string_string ("%d", ptype);
  }

  ut_math_vars (expr, &vars, &varqty);
  vals = ut_alloc_1d (varqty);

  if (ut_string_isfilename (expr))
  {
    file = ut_file_open (expr, "R");
    ut_array_1d_fscanf (file, val + 1, Point.Qty);
    ut_file_close (file, expr, "R");
    ut_string_string ("%f", ptype);
  }
  else
  {
    for (j = 1; j <= Point.Qty; j++)
    {
      for (k = 0; k < varqty; k++)
      {
        if (!strcmp (vars[k], "default"))
          vals[k] = val[j];

        neut_point_var_val_one (Point, Tess, Nodes, Mesh, j, vars[k],
                                vals + k, &type);
        if (ptype && !strcmp (type, "%f"))
          ut_string_string ("%f", ptype);
      }

      status = ut_math_eval (expr, varqty, vars, vals, val + j);
      if (status == -1)
        abort ();
    }
  }

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);
  ut_free_1d_char (&type);

  return;
}

int
neut_point_isvoid (struct POINT Point)
{
  return Point.Qty == 0;
}
