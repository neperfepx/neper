/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_coo_.h"

int
net_tess_opt_init_sset_coo_centre_randpt (struct TOPT *pTOpt,
                                          struct POINT Point, gsl_rng * r,
                                          double rad, double penetration,
                                          double *coo, double *pdist)
{
  int status = 0;

  net_tess_opt_init_sset_coo_centre_randpt_pick (pTOpt, Point, (*pTOpt).activedim, r, coo);

  if (!(*pTOpt).DomType || strcmp ((*pTOpt).DomType, "cube"))
    status =
      net_tess_opt_init_sset_coo_centre_randpt_test (pTOpt, Point,
                                                     rad, penetration, coo,
                                                     pdist);

  return status;
}

int
net_tess_opt_init_sset_coo_centre_randpt_cluster (struct TOPT *pTOpt,
                                                  struct POINT Point,
                                                  gsl_rng * r,
                                                  struct POINT Point2,
                                                  double penetration,
                                                  double *coo, double *pdist)
{
  int i, status = 0;
  double rad2, *coo2 = ut_alloc_1d (3);
  double *val = ut_alloc_1d (Point2.Qty + 1);

  net_tess_opt_init_sset_coo_centre_randpt_pick (pTOpt, Point, (*pTOpt).activedim, r, coo);

  for (i = 1; i <= Point2.Qty; i++)
  {
    ut_array_1d_add (coo, Point2.Coo[i], 3, coo2);
    rad2 = Point2.Rad[i];
    status =
      net_tess_opt_init_sset_coo_centre_randpt_test (pTOpt, Point, rad2,
                                                     penetration, coo2,
                                                     val + i);
  }

  (*pdist) = ut_array_1d_sum (val + 1, Point2.Qty);

  ut_free_1d (&val);
  ut_free_1d (&coo2);

  return status;
}

int
net_tess_opt_init_sset_coo_centre_randpt_pick (struct TOPT *pTOpt, struct POINT Point,
                                               int *activedim, gsl_rng * r, double *coo)
{
  int i;

  // do not change the below loop to keep 3 gsl_rng_uniform calls /
  // centre for all dimensions.
  for (i = 0; i < 3; i++)
    coo[i] =
      (*pTOpt).SSet.Size[i][0] + gsl_rng_uniform (r) * ((*pTOpt).SSet.Size[i][1] -
                                                (*pTOpt).SSet.Size[i][0]);

  for (i = Point.Dim; i < 3; i++)
    coo[i] = ut_array_1d_mean ((*pTOpt).SSet.Size[i], 2);

  if (activedim)
    for (i = 0; i < Point.Dim; i++)
      if (!activedim[i])
        coo[i] = ut_array_1d_mean ((*pTOpt).SSet.Size[i], 2);

  return 0;
}

int
net_tess_opt_init_sset_coo_centre_randpt_test (struct TOPT *pTOpt,
                                               struct POINT Point, double rad,
                                               double penetration,
                                               double *coo, double *pdist)
{
  int i, status, l, m, n;
  double dist;
  double *ptcoo = ut_alloc_1d (3);

  status = 0;
  if ((*pTOpt).DomPoly.FaceQty > 0 && neut_poly_point_in ((*pTOpt).DomPoly, coo) == 0)
    status = -2;

  else if (pdist)
  {
    (*pdist) = 0;
    for (i = 1; i <= Point.Qty; i++)
      for (l = -(*pTOpt).SSet.Periodic[0]; l <= (*pTOpt).SSet.Periodic[0]; l++)
      {
        ptcoo[0] =
          Point.Coo[i][0] + l * ((*pTOpt).SSet.Size[0][1] - (*pTOpt).SSet.Size[0][0]);
        for (m = -(*pTOpt).SSet.Periodic[1]; m <= (*pTOpt).SSet.Periodic[1]; m++)
        {
          ptcoo[1] =
            Point.Coo[i][1] + m * ((*pTOpt).SSet.Size[1][1] - (*pTOpt).SSet.Size[1][0]);
          for (n = -(*pTOpt).SSet.Periodic[2]; n <= (*pTOpt).SSet.Periodic[2]; n++)
          {
            ptcoo[2] =
              Point.Coo[i][2] + n * ((*pTOpt).SSet.Size[2][1] -
                                          (*pTOpt).SSet.Size[2][0]);
            dist = ut_space_dist (ptcoo, coo) - Point.Rad[i] - rad;
            (*pdist) = ut_num_min (dist, (*pdist));
          }
        }
      }

    if (*pdist < -penetration * rad)
      status = -1;
  }

  ut_free_1d (&ptcoo);

  return status;
}
