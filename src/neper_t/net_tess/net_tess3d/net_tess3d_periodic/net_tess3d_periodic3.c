/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
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

void
net_tess3d_periodic_rodrigues_pair (double **coo, int qty, char *crysym, int **ppermaster)
{
  int i, j, master, slave;
  double disori;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  (*ppermaster) = ut_alloc_1d_int (qty + 1);

  ut_array_1d_int_set (*ppermaster, qty + 1, -1);

  // searching slaves based on disorientation
  for (i = 1; i <= qty; i++)
    // if not secondary yet, finding secondary
    if ((*ppermaster)[i] == -1)
    {
      master = i;
      ol_R_q (coo[master], q1);

      slave = -1;

      for (j = i + 1; j <= qty; j++)
      {
        ol_R_q (coo[j], q2);

        ol_q_q_disori_rad (q1, q2, crysym, &disori);

        if (disori < sqrt (OL_EPS) && (*ppermaster)[j] == -1)
        {
          slave = j;

          (*ppermaster)[master] = 0;
          (*ppermaster)[slave] = master;
        }
      }

      if (slave == -1)
        abort ();
    }

  ol_q_free (q1);
  ol_q_free (q2);

  return;
}

void
net_tess3d_periodic_edge_rodrigues_peredgeori (struct TESS *pTess)
{
  int i;
  double disori1, disori2;
  double *q1 = ol_q_alloc ();
  double *q2 = ol_q_alloc ();
  char *crysym = NULL;

  ut_string_functionargument ((*pTess).DomType, &crysym);

  (*pTess).PerEdgeOri = ut_alloc_1d_int ((*pTess).EdgeQty + 1);

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    if ((*pTess).PerEdgeMaster[i] > 0)
    {
      // we cannot rely on the actual directions, since equivalent edges aren't necessarily parallel.
      // testing the equivalency of the first vertex instead
      ol_R_q ((*pTess).VerCoo[(*pTess).EdgeVerNb[i][0]], q1);
      ol_R_q ((*pTess).VerCoo[(*pTess).EdgeVerNb[(*pTess).PerEdgeMaster[i]][0]], q2);
      ol_q_q_disori (q1, q2, crysym, &disori1);
      ol_R_q ((*pTess).VerCoo[(*pTess).EdgeVerNb[(*pTess).PerEdgeMaster[i]][1]], q2);
      ol_q_q_disori (q1, q2, crysym, &disori2);

      if (disori1 > OL_EPS && disori2 > OL_EPS)
        abort ();

      (*pTess).PerEdgeOri[i] = (disori1 < OL_EPS) ? 1 : -1;
    }

  ol_q_free (q1);
  ol_q_free (q2);
  ut_free_1d_char (&crysym);

  return;
}

void
net_tess3d_periodic_face_rodrigues_perfaceori (struct TESS *pTess)
{
  int i;

  (*pTess).PerFaceOri = ut_alloc_1d_int ((*pTess).FaceQty + 1);
  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).PerFaceMaster[i] > 0)
    {
      (*pTess).PerFaceOri[i]
        = ut_num_d2ri (ut_vector_scalprod ((*pTess).FaceEq[(*pTess).PerFaceMaster[i]] + 1, (*pTess).FaceEq[i] + 1));
    }

  return;
}
