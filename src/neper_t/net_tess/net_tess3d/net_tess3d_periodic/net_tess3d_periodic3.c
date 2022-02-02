/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_tess3d_periodic_.h"

void
net_tess3d_periodic_ver_shrinkmaster (struct TESS *pTess)
{
  int i;

  // Making sure that a slave ver cannot be a master ver, i.e. that
  // there is only two hierarchy levels - master or slave.
  for (i = 1; i <= (*pTess).VerQty; i++)
    if ((*pTess).PerVerMaster[i] > 0)
      while ((*pTess).PerVerMaster[(*pTess).PerVerMaster[i]] > 0)
      {
        ut_array_1d_int_add ((*pTess).PerVerShift[i],
                             (*pTess).PerVerShift[(*pTess).PerVerMaster[i]],
                             3, (*pTess).PerVerShift[i]);
        (*pTess).PerVerMaster[i] =
          (*pTess).PerVerMaster[(*pTess).PerVerMaster[i]];
      }

  return;
}

void
net_tess3d_periodic_edge_shrinkmaster (struct TESS *pTess)
{
  int i;

  // Making sure that a slave edge cannot be a master edge, i.e. that
  // there is only two hierarchy levels - master or slave.
  for (i = 1; i <= (*pTess).EdgeQty; i++)
    if ((*pTess).PerEdgeMaster[i] > 0)
      while ((*pTess).PerEdgeMaster[(*pTess).PerEdgeMaster[i]] > 0)
      {
        ut_array_1d_int_add ((*pTess).PerEdgeShift[i],
                             (*pTess).PerEdgeShift[(*pTess).PerEdgeMaster[i]],
                             3, (*pTess).PerEdgeShift[i]);
        (*pTess).PerEdgeMaster[i] =
          (*pTess).PerEdgeMaster[(*pTess).PerEdgeMaster[i]];
      }

  return;
}

void
net_tess3d_periodic_edge_peredgeori (struct TESS *pTess)
{
  int i, edge, master;

  for (i = 1; i <= (*pTess).PerEdgeQty; i++)
  {
    edge = (*pTess).PerEdgeNb[i];
    master = (*pTess).PerEdgeMaster[edge];

    double *v1 = ut_alloc_1d (3);
    double *v2 = ut_alloc_1d (3);

    ut_array_1d_sub ((*pTess).VerCoo[(*pTess).EdgeVerNb[master][1]],
                     (*pTess).VerCoo[(*pTess).EdgeVerNb[master][0]], 3, v1);

    ut_array_1d_sub ((*pTess).VerCoo[(*pTess).EdgeVerNb[edge][1]],
                     (*pTess).VerCoo[(*pTess).EdgeVerNb[edge][0]], 3, v2);

    (*pTess).PerEdgeOri[edge] = ut_num_sgn (ut_array_1d_scalprod (v1, v2, 3));
    if (fabs (ut_vector_angle (v1, v2)) > 1e-3
        && fabs (ut_vector_angle (v1, v2) - 180) > 1e-3)
      ut_print_neperbug ();

    ut_free_1d (&v1);
    ut_free_1d (&v2);
  }

  return;
}
