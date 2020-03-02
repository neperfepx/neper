/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include "ol_gnd_nye.h"

int
ol_gnd_map_nye_region (struct OL_MAP Map, struct OL_CRYS Crys,
                       double thetamin, double thetamax, double **nye,
                       unsigned int *pid)
{
  return ol_gnd_map_nye_region_method (Map, Crys, thetamin, thetamax, nye,
                                       pid, "man2010");
}

int
ol_gnd_map_nye_region_method (struct OL_MAP Map, struct OL_CRYS Crys,
                              double thetamin, double thetamax, double **nye,
                              unsigned int *pid, char *method)
{
  unsigned int i, j;
  double *qmap = NULL;

  Crys = Crys;

  if (!
      ((Map.xsize == 2 && Map.ysize == 2)
       || (Map.xsize == 3 && Map.xsize == 3)))
  {
    printf ("xsize = %d ysize = %d != 3\n", (int) Map.xsize, (int) Map.ysize);
    abort ();
  }

  if (Map.id[0][0] == 0 || Map.id[0][1] == 0 || Map.id[1][0] == 0)
  {
    (*pid) = 0;
    return (*pid);
  }
  else
    (*pid) = 1;

  qmap = ol_q_alloc ();
  ol_g_q (Map.gmap, qmap);

  // methods available for estimating the derivatives:
  // "man2010", after (Man et al, IJP, 2010) - large rotation
  // "demir2009", after (Demir et al, Acta Mater, 2009) - small rotation

  struct OL_MAP MapR = ol_map_alloc (Map.xsize, Map.ysize, Map.stepsize,
                                     Map.crysym);
  ol_map_memcpy (Map, &MapR);

  for (i = 0; i < Map.xsize; i++)
    for (j = 0; j < Map.ysize; j++)
      ol_q_refcsys (MapR.q[i][j], qmap, MapR.q[i][j]);

  // Calculating the disorientation along axes 1 and 2.

  double **wx = ol_g_alloc ();
  double **wy = ol_g_alloc ();

  double *qref = NULL;
  double *qx1 = NULL;
  double *qx2 = NULL;
  double *qy1 = NULL;
  double *qy2 = NULL;

  if (Map.xsize == 2)
  {
    qref = MapR.q[0][0];
    qx1 = MapR.q[0][0];
    qx2 = MapR.q[0][1];
    qy1 = MapR.q[0][0];
    qy2 = MapR.q[1][0];
  }
  else if (Map.xsize == 3)
  {
    qref = MapR.q[1][1];
    qx1 = MapR.q[1][0];
    qx2 = MapR.q[1][2];
    qy1 = MapR.q[0][1];
    qy2 = MapR.q[2][1];
  }
  else
    abort ();

  if (!strcmp (method, "man2010"))
  {
    double **gdx = ol_g_alloc ();
    double **gdy = ol_g_alloc ();
    double *qdx = ol_q_alloc ();
    double *qdy = ol_q_alloc ();
    double thetax, thetay;

    ol_q_q_qdisori_ref (qx1, qx2, Map.crysym, qdx);     // axis 1
    ol_q_q_qdisori_ref (qy1, qy2, Map.crysym, qdy);     // axis 2
    ol_q_theta (qdx, &thetax);
    ol_q_theta (qdy, &thetay);

    if (thetax > thetamax || thetay > thetamax)
      (*pid) = 0;

    if (thetax > thetamin)
    {
      ol_q_g (qdx, gdx);
      ol_g_lng (gdx, wx);
    }
    // else, zero

    if (thetay > thetamin)
    {
      ol_q_g (qdy, gdy);
      ol_g_lng (gdy, wy);
    }
    // else, zero

    ol_q_free (qdx);
    ol_q_free (qdy);
    ol_g_free (gdx);
    ol_g_free (gdy);
  }

  else if (!strcmp (method, "demir2009"))
  {
    int id;
    double **gx1 = ol_g_alloc ();
    double **gx2 = ol_g_alloc ();
    double **gy1 = ol_g_alloc ();
    double **gy2 = ol_g_alloc ();
    double *qtmp = ol_q_alloc ();
    double theta;

    ol_q_g (qx1, gx1);
    ol_q_g (qx2, gx2);
    ol_q_g (qy1, gy1);
    ol_q_g (qy2, gy2);

    id = ol_q_q_qdisori_ref (qx1, qx2, "cubic", qtmp);
    ol_g_crysym (gx2, "cubic", id, gx2);
    ol_q_theta (qtmp, &theta);
    if (theta > thetamax)
      (*pid) = 0;

    ut_array_2d_sub (gx2, gx1, 3, 3, wx);

    id = ol_q_q_qdisori_ref (qy1, qy2, "cubic", qtmp);
    ol_g_crysym (gy2, "cubic", id, gy2);
    ol_q_theta (qtmp, &theta);
    if (theta > thetamax)
      (*pid) = 0;

    if (theta > thetamin)
      ut_array_2d_sub (gy2, gy1, 3, 3, wy);
    // else, zero

    ol_g_free (gx1);
    ol_g_free (gx2);
    ol_g_free (gy1);
    ol_g_free (gy2);
    ol_q_free (qtmp);
  }

  else
    abort ();

  double **wz = ol_g_alloc ();
  ol_g_set_zero (wz);

  if ((*pid) == 1)
  {
    // ii'th terms are zero.
    nye[0][0] = wy[0][2] - wz[0][1];
    nye[0][1] = wy[1][2];
    nye[0][2] = -wz[2][1];

    nye[1][0] = -wx[0][2];
    nye[1][1] = wz[1][0] - wx[1][2];
    nye[1][2] = wz[2][0];

    nye[2][0] = wx[0][1];
    nye[2][1] = -wy[1][0];
    nye[2][2] = wx[2][1] - wy[2][0];

    ut_array_2d_scale (nye, 3, 3, 1. / ((Map.xsize - 1) * Map.stepsize));

    double **gmapinv = ol_g_alloc ();
    double **gref = ol_g_alloc ();
    ol_g_inverse (Map.gmap, gmapinv);
    ol_q_g (qref, gref);
    ol_g_csys (nye, gmapinv, nye);
    ol_g_csys (nye, gref, nye);

    ol_g_free (gmapinv);
    ol_g_free (gref);
  }

  ut_free_2d (&wx, 3);
  ut_free_2d (&wy, 3);

  return (*pid);
}

void
ol_gnd_map_nye_method (struct OL_MAP Map, struct OL_CRYS Crys,
                       double thetamin, double thetamax,
                       struct OL_GNDMAP *pGnd, char *method)
{
  size_t i, j;
  struct OL_MAP Map2;

  Map2 = ol_map_alloc (2, 2, Map.stepsize, Map.crysym);

  for (i = 0; i < Map.xsize - 1; i++)
    for (j = 0; j < Map.ysize - 1; j++)
    {
      ol_map_submap_center (Map, i, j, 2, 2, &Map2);
      ol_gnd_map_nye_region_method (Map2, Crys, thetamin, thetamax,
                                    (*pGnd).nye[i][j], &((*pGnd).id[i][j]),
                                    method);
    }

  ol_map_free (Map2);

  return;
}

void
ol_gnd_map_nye (struct OL_MAP Map, struct OL_CRYS Crys, double thetamin,
                double thetamax, struct OL_GNDMAP *pGnd)
{
  ol_gnd_map_nye_method (Map, Crys, thetamin, thetamax, pGnd, "man2010");

  return;
}
