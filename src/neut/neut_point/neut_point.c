/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_point_.h"

void
neut_point_set_zero (struct POINT *pPoint)
{
  (*pPoint).Dim = 0;
  (*pPoint).Periodic = NULL;
  (*pPoint).activedim = NULL;
  (*pPoint).BBox = NULL;
  (*pPoint).PointQty = 0;
  (*pPoint).PointCoo = NULL;
  (*pPoint).PointRad = NULL;

  return;
}

void
neut_point_free (struct POINT *pPoint)
{
  ut_free_2d_ (&(*pPoint).PointCoo, (*pPoint).PointQty + 1);
  ut_free_1d_ (&(*pPoint).PointRad);
  ut_free_1d_int_ (&(*pPoint).Periodic);
  ut_free_1d_int_ (&(*pPoint).activedim);
  ut_free_2d_ (&(*pPoint).BBox, 3);

  (*pPoint).PointQty = 0;
  (*pPoint).Dim = 0;

  return;
}

void
neut_point_centre (struct POINT Point, double *centre)
{
  int i;
  double size, totsize;
  double *tmp = ut_alloc_1d (3);
  int weight = (ut_array_1d_sum (Point.PointRad + 1, Point.PointQty) != 0);

  ut_array_1d_zero (centre, 3);
  totsize = 0;
  for (i = 1; i <= Point.PointQty; i++)
  {
    if (weight)
      neut_point_pt_size (Point, i, &size);
    else
      size = 1;

    ut_array_1d_memcpy (tmp, 3, Point.PointCoo[i]);
    ut_array_1d_scale (tmp, 3, size);

    ut_array_1d_add (centre, tmp, 3, centre);
    totsize += size;
  }
  ut_array_1d_scale (centre, 3, 1. / totsize);

  ut_free_1d (tmp);

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

  for (i = 1; i <= Point.PointQty; i++)
    for (j = 0; j < 3; j++)
    {
      bbox[j][0] = ut_num_min (bbox[j][0], Point.PointCoo[i][j]);
      bbox[j][1] = ut_num_max (bbox[j][1], Point.PointCoo[i][j]);
    }

  return;
}

void
neut_point_var_list (char ***pvar, int *pvarqty)
{
  int id = 0;

  (*pvarqty) = 6;
  (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
  strcpy ((*pvar)[id++], "id");
  strcpy ((*pvar)[id++], "x");
  strcpy ((*pvar)[id++], "y");
  strcpy ((*pvar)[id++], "z");
  strcpy ((*pvar)[id++], "poly");
  strcpy ((*pvar)[id++], "cell");

  return;
}

void
neut_point_var_list_mesh (char ***pvar, int *pvarqty)
{
  int id = 0;

  (*pvarqty) = 10;
  (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
  strcpy ((*pvar)[id++], "id");
  strcpy ((*pvar)[id++], "x");
  strcpy ((*pvar)[id++], "y");
  strcpy ((*pvar)[id++], "z");
  strcpy ((*pvar)[id++], "elt");
  strcpy ((*pvar)[id++], "elset");
  strcpy ((*pvar)[id++], "2dmeshp");
  strcpy ((*pvar)[id++], "2dmeshd");
  strcpy ((*pvar)[id++], "2dmeshv");
  strcpy ((*pvar)[id++], "2dmeshn");

  return;
}

int
neut_point_var_val (struct POINT Point, int id, struct TESS Tess,
		    struct NODES Nodes, struct MESH Mesh,
		    char *var, double **pvals, int *pvalqty, char **ptype)
{
  int i, status, tmp;

  (*ptype) = ut_alloc_1d_char (10);

  (*pvalqty) = 1;
  *pvals = ut_realloc_1d (*pvals, *pvalqty);

  status = 0;
  if (!strcmp (var, "id"))
  {
    (*pvals)[0] = id;
    strcpy (*ptype, "%d");
  }
  else if (!strcmp (var, "x"))
  {
    (*pvals)[0] = Point.PointCoo[id][0];
    strcpy (*ptype, "%f");
  }
  else if (!strcmp (var, "y"))
  {
    (*pvals)[0] = Point.PointCoo[id][1];
    strcpy (*ptype, "%f");
  }
  else if (!strcmp (var, "z"))
  {
    (*pvals)[0] = Point.PointCoo[id][2];
    strcpy (*ptype, "%f");
  }
  else if (!strcmp (var, "rad"))
  {
    (*pvals)[0] = Point.PointRad[id];
    strcpy (*ptype, "%f");
  }
  else if (!strcmp (var, "poly") || !strcmp (var, "cell"))
  {
    (*pvals)[0] = -1;
    for (i = 1; i <= Tess.PolyQty; i++)
      if (neut_tess_point_inpoly (Tess, Point.PointCoo[id], i) == 1)
      {
	(*pvals)[0] = i;
	break;
      }
    strcpy (*ptype, "%d");
  }
  else if (!strcmp (var, "elt"))
  {
    neut_mesh_point_elt (Mesh, Nodes, Point.PointCoo[id], &tmp);
    (*pvals)[0] = tmp;
    strcpy (*ptype, "%d");
  }
  else if (!strcmp (var, "elset"))
  {
    neut_mesh_point_elset (Mesh, Nodes, Point.PointCoo[id], NULL, 0, &tmp);
    (*pvals)[0] = tmp;
    strcpy (*ptype, "%d");
  }
  else
    status = -1;

  return status;
}

int
neut_point_var_val_one (struct POINT Point, int id, struct TESS Tess,
                        struct NODES Nodes, struct MESH Mesh,
                        char *var, double *pval, char **ptype)
{
  int status, qty;
  double *tmp = NULL;

  status = neut_point_var_val (Point, id, Tess, Nodes, Mesh, var, &tmp, &qty, ptype);

  if (qty == 1)
  {
    (*pval) = tmp[0];
    status = 0;
  }
  else
    status = -1;

  ut_free_1d (tmp);

  return status;
}

void
neut_point_addpoint (struct POINT *pPoint, double *coo, double rad)
{
  (*pPoint).PointQty++;
  if ((*pPoint).PointQty == 1)
    (*pPoint).PointCoo = ut_alloc_2d (1, 3);

  (*pPoint).PointCoo = ut_realloc_2d_addline ((*pPoint).PointCoo,
					      (*pPoint).PointQty + 1, 3);
  (*pPoint).PointRad = ut_realloc_1d ((*pPoint).PointRad,
				      (*pPoint).PointQty + 1);

  if (coo)
    ut_array_1d_memcpy ((*pPoint).PointCoo[(*pPoint).PointQty], 3, coo);
  (*pPoint).PointRad[(*pPoint).PointQty] = rad;

  return;
}

void
neut_point_pt_size (struct POINT Point, int pt, double *psize)
{
  (*psize) = 0;
  if (Point.Dim == 1)
    (*psize) = 2 * Point.PointRad[pt];
  else if (Point.Dim == 2)
    (*psize) = M_PI * pow (Point.PointRad[pt], 2);
  else if (Point.Dim == 3)
    (*psize) = (4. * M_PI / 3) * pow (Point.PointRad[pt], 3);

  return;
}

void
neut_point_shift (struct POINT *pPoint, double *s)
{
  int i;

  for (i = 1; i <= (*pPoint).PointQty; i++)
    ut_array_1d_add ((*pPoint).PointCoo[i], s, 3,
		     (*pPoint).PointCoo[i]);

  return;
}

void
neut_point_shifttocentre (struct POINT *pPoint)
{
  double *s = ut_alloc_1d (3);

  neut_point_centre (*pPoint, s);
  ut_array_1d_scale (s, 3, -1);
  neut_point_shift (pPoint, s);

  ut_free_1d (s);

  return;
}
