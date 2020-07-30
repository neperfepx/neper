/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_coo_.h"

void
net_tess_opt_init_sset_coo_cluster (int dim, int *activedim, gsl_rng * r2,
                                    int qty, double dist, double rad,
                                    struct POINT *pPoint)
{
  neut_point_set_zero (pPoint);
  (*pPoint).Dim = dim;
  if (activedim)
  {
    (*pPoint).activedim = ut_alloc_1d_int (3);
    ut_array_1d_int_memcpy (activedim, 3, (*pPoint).activedim);
  }

  if (qty == 1)
    neut_point_addpoint (pPoint, NULL, rad);

  else
  {
    double *coo = ut_alloc_1d (3);
    int i, j, dimqty, *dims = ut_alloc_1d_int (3);

    dimqty = dim;
    ut_array_1d_int_set_id (dims, dimqty);

    for (i = 1; i <= qty; i++)
    {
      ut_space_random (r2, dims, dimqty, dist, dist, coo);
      if (activedim)
        for (j = 0; j < dim; j++)
          if (!activedim[j])
            coo[i] = 0;
      neut_point_addpoint (pPoint, coo, rad);
    }

    neut_point_shifttocentre (pPoint);

    ut_free_1d (&coo);
    ut_free_1d_int (&dims);
  }

  return;
}

void
net_tess_opt_init_sset_coo_centre (struct TOPT *pTOpt, gsl_rng * r, char *var,
                                   int pos, char *cooexpr, struct POINT Point,
                                   struct POINT Point2, int cell,
                                   double *centre)
{
  int id, status, iter, iter_max = 1000;
  double *dist = ut_alloc_1d (iter_max + 1);
  double **coo = ut_alloc_2d (iter_max + 1, 3);

  if (!strcmp (var, "none") || !strcmp (var, "size")
      || !strcmp (var, "diameq") || (!strcmp (var, "centroid")
                                     && !strcmp (cooexpr, "seed")))
  {
    iter = 0;
    do
    {
      iter++;
      if (Point2.PointQty == 1 && Point2.PointRad[1] == 0)
        status =
          net_tess_opt_init_sset_coo_centre_randpt (pTOpt, Point, r, 0, 0,
                                                    coo[iter], dist + iter);
      else
        status =
          net_tess_opt_init_sset_coo_centre_randpt_cluster ((*pTOpt).DomPoly,
                                                            Point, r, Point2,
                                                            0, coo[iter],
                                                            dist + iter);

      // if random positions, we don't care if there is some overlap
      // (status = -1), but we do care if the point is not in the domain
      // (status = -2)
      if (!strcmp (cooexpr, "random") || !strcmp (cooexpr, "seed"))
      {
        if (status == -1)
          status = 0;
      }
      else if (!strcmp (cooexpr, "packing"))
      {
      }
      else if (!strncmp (cooexpr, "bcc", 3))
      {
      }
      else
        abort ();
    }
    while (status != 0 && iter < iter_max);

    id = (status == 0) ? iter : 1 + ut_array_1d_max_index (dist + 1, iter);
    ut_array_1d_memcpy (coo[id], 3, centre);
  }

  else if (!strncmp (var, "centroid", 8))
  {
    if (!strcmp (cooexpr, "centroid") || !strcmp (cooexpr, "LLLFP2011"))
    {
      if (strcmp (var, "centroidtol")
          || (*pTOpt).tarcellval[pos][cell][(*pTOpt).tarcellvalqty[pos] - 1] <
          1000)
        ut_array_1d_memcpy ((*pTOpt).tarcellval[pos][cell], (*pTOpt).Dim,
                            centre);
      else
        net_tess_opt_init_sset_coo_centre_randpt_pick (Point, r, centre);
    }
    else
      abort ();
  }

  else if (!strcmp (var, "tesr"))
  {
    if (!strcmp (cooexpr, "centroid") || !strcmp (cooexpr, "LLLFP2011"))
      neut_tesr_cell_centre ((*pTOpt).tartesr, cell, centre);
    else
      abort ();
  }

  else
    abort ();

  ut_free_1d (&dist);
  ut_free_2d (&coo, iter_max + 1);

  return;
}

void
net_tess_opt_init_sset_coo_record (struct TOPT *pTOpt, int cell,
                                   struct POINT *pPoint, double *centre,
                                   struct POINT *pPoint2)
{
  int i;

  neut_point_shift (pPoint2, centre);
  for (i = 1; i <= (*pPoint2).PointQty; i++)
  {
    neut_point_addpoint (pPoint, (*pPoint2).PointCoo[i],
                         (*pPoint2).PointRad[i]);
    neut_topt_seed_subcell_add (pTOpt, (*pPoint2).PointCoo[i],
                                (*pPoint2).PointRad[i], cell);
  }

  return;
}

void
net_tess_opt_init_sset_coo_lllfp2011 (struct TOPT *pTOpt)
{
  int i, j;
  double *centroid = ut_alloc_1d (3);
  struct TESS T;
  neut_tess_set_zero (&T);

  net_tess3d ((*pTOpt).Dom, 1, (*pTOpt).SSet, 1, NULL, &T);

  for (i = 1; i <= (*pTOpt).SSet.N; i++)
  {
    neut_tess_cell_centroid (T, i, centroid);
    for (j = 0; j < (*pTOpt).Dim; j++)
      (*pTOpt).SSet.SeedCoo[i][j] =
        2 * (*pTOpt).SSet.SeedCoo[i][j] - centroid[j];
  }

  ut_free_1d (&centroid);
  neut_tess_free (&T);

  return;
}

int
net_tess_opt_init_sset_coo_bcc_expr (struct TESS Dom, char *cooexpr,
                                     double ***pcoo)
{
  int n;

  sscanf (cooexpr, "bcc(%d)", &n);

  return net_tess_opt_init_sset_coo_bcc (Dom, n, pcoo);
}

int
net_tess_opt_init_sset_coo_bcc (struct TESS Dom, int n, double ***pcoo)
{
  int i, x, y, z, nb, N = ut_num_d2ri (pow (n, 3) + pow (n + 1, 3));
  double step[3], **bbox = ut_alloc_2d (3, 2);

  (*pcoo) = ut_alloc_2d (N + 1, 3);

  neut_tess_bbox (Dom, bbox);
  for (i = 0; i < 3; i++)
    step[i] = (bbox[i][1] - bbox[i][0]) / n;

  nb = 0;
  for (x = 1; x <= n; x++)
    for (y = 1; y <= n; y++)
      for (z = 1; z <= n; z++)
      {
        nb++;
        (*pcoo)[nb][0] = bbox[0][0] + (x - 0.5) * step[0];
        (*pcoo)[nb][1] = bbox[1][0] + (y - 0.5) * step[1];
        (*pcoo)[nb][2] = bbox[2][0] + (z - 0.5) * step[2];
      }

  for (x = 0; x <= n; x++)
    for (y = 0; y <= n; y++)
      for (z = 0; z <= n; z++)
      {
        nb++;
        (*pcoo)[nb][0] = bbox[0][0] + x * step[0];
        (*pcoo)[nb][1] = bbox[1][0] + y * step[1];
        (*pcoo)[nb][2] = bbox[2][0] + z * step[2];
      }

  ut_free_2d (&bbox, 3);

  return nb;
}
