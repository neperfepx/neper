/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_coo_.h"

void
net_tess_opt_init_sset_coo_cluster (int dim, gsl_rng *r2, int qty, double dist,
				    double rad, struct POINT *pPoint)
{
  int i;
  double *coo = ut_alloc_1d (3);
  int dimqty, *dims = ut_alloc_1d_int (3);

  // neut_seedset_activedim (SSet, "equiaxed", &dims, &dimqty);
  dimqty = dim;
  ut_array_1d_int_set_id (dims, dimqty);

  neut_point_set_zero (pPoint);
  (*pPoint).Dim = dim;

  for (i = 1; i <= qty; i++)
  {
    ut_space_random (r2, dims, dimqty, dist, dist, coo);
    neut_point_addpoint (pPoint, coo, rad);
  }

  neut_point_shifttocentre (pPoint);

  ut_free_1d (coo);
  ut_free_1d_int (dims);

  return;
}

void
net_tess_opt_init_sset_coo_centre (struct TOPT *pTOpt, gsl_rng *r,
                                   char *var, int pos, char *cooexpr,
				   struct POINT Point,
				   struct POINT Point2, int cell, double *centre)
{
  int id, status, iter, iter_max = 1000;
  double *dist = ut_alloc_1d (iter_max + 1);
  double **coo = ut_alloc_2d (iter_max + 1, 3);

  if (!strcmp (var, "none") || !strcmp (var, "size") || !strcmp (var, "diameq")
   || (!strcmp (var, "centroid") && !strcmp (cooexpr, "seed")))
  {
    iter = 0;
    do
    {
      iter++;
      status = neut_tess_dom_pt_randpt_cluster ((*pTOpt).Dom, Point, r,
						Point2, 0, coo[iter],
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
      else
	abort ();
    }
    while (status != 0 && iter < iter_max);

    id =
      (status == 0) ? iter : 1 + ut_array_1d_max_index (dist + 1, iter);
    ut_array_1d_memcpy (centre, 3, coo[id]);
  }

  else if (!strncmp (var, "centroid", 8))
  {
    if (!strcmp (cooexpr, "centroid") || !strcmp (cooexpr, "LLLFP2011"))
      ut_array_1d_memcpy (centre, (*pTOpt).Dim, (*pTOpt).tarcellval[pos][cell]);
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

  ut_free_1d (dist);
  ut_free_2d (coo, iter_max + 1);

  return;
}

void
net_tess_opt_init_sset_coo_record (struct TOPT *pTOpt, int cell, struct POINT
				   *pPoint, double *centre, struct POINT
				   *pPoint2)
{
  int i;

  neut_point_shift (pPoint2, centre);
  for (i = 1; i <= (*pPoint2).PointQty; i++)
  {
    neut_point_addpoint (pPoint, (*pPoint2).PointCoo[i], (*pPoint2).PointRad[i]);
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

  net_tess3d ((*pTOpt).Dom, 1, (*pTOpt).SSet, "ann", 1, NULL, &T);

  for (i = 1; i <= (*pTOpt).SSet.N; i++)
  {
    neut_tess_cell_centroid (T, i, centroid);
    for (j = 0; j < (*pTOpt).Dim; j++)
      (*pTOpt).SSet.SeedCoo[i][j] = 2 * (*pTOpt).SSet.SeedCoo[i][j] - centroid[j];
  }

  ut_free_1d (centroid);
  neut_tess_free (&T);

  return;
}
