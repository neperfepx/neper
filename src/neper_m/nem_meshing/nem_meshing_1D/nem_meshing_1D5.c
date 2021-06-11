/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_1D_.h"

void
nem_mesh_1d_unitsegment (double cl1, double cl2, double clt, double pl,
                         int *pqty, double **pcoo, double **pcl)
{
  int i, id, n1p, n2p, add;
  double I;
  double lp, Dl, l1p, l2p, n1p_cl, n2p_cl, cltp, d1, d2;
  double plp;

  // Checking input data
  if (pl == 1)
    pl = 1 + 1e-6;

  if (cl1 > clt || cl2 > clt || cl2 < cl1 || pl <= 1 || cl1 <= 0 || cl2 <= 0)
  {
    printf ("cl1 = %f cl2 = %f clt = %f pl = %f\n", cl1, cl2, clt, pl);
    ut_print_neperbug ();
  }

  // I is the intersection point between the cl progressions from both
  // extremities of the segment.

  I = cl1_cl2_pl_I (cl1, cl2, pl);

  // I > 1 means that pl is too low to reach cl2 at the second
  // extremity, by progression from cl1. pl has to be increased.
  if (I > 1)
  {
    plp = cl2 - cl1 + 1;
    (*pqty) = cl1_pl_x_i (cl1, plp, 1.) + 1;
    lp = cl1_pl_n_l (cl1, plp, *pqty);

    (*pcoo) = ut_alloc_1d ((*pqty));
    for (i = 1; i <= *pqty; i++)
      (*pcoo)[i - 1] = cl1_pl_i_x (cl1, plp, i) / lp;
  }

  // Processing with pl
  else
  {
    // Calculating the number of nodes and the lengths of the 2
    // progression segments.
    n1p = cl1_cl_pl_n (cl1, clt, pl);
    n2p = cl1_cl_pl_n (cl2, clt, pl);
    l1p = cl1_pl_n_l (cl1, pl, n1p);
    l2p = cl1_pl_n_l (cl2, pl, n2p);

    // If the progression segments intersect each other, there will be
    // only 2 segments
    if (l1p + l2p > 1)
    {
      n1p = cl1_pl_x_i (cl1, pl, I);    // number of nodes before I
      n2p = cl1_pl_x_i (cl2, pl, 1 - I);        // number of nodes before I
      l1p = cl1_pl_n_l (cl1, pl, n1p);  // length before I
      l2p = cl1_pl_n_l (cl2, pl, n2p);  // length before I

      n1p_cl = cl1_pl_i_cl (cl1, pl, n1p);      // cl at last point
      n2p_cl = cl1_pl_i_cl (cl2, pl, n2p);      // cl at last point
      cltp = ut_num_min (n1p_cl, n2p_cl);       // min of the two

      // Distance between the two progression segments
      Dl = 1 - l1p - l2p;
      if (Dl < 0)
      {
        printf ("Dl < 0\n");
        ut_print_neperbug ();
      }

      // If Dl < cltp, adding a segment of length cltp then scaling the
      // node coo to get a full segment of length 1
      if (Dl < cltp)
      {
        lp = l1p + l2p + cltp;

        id = 0;
        (*pqty) = n1p + n2p;
        (*pcoo) = ut_alloc_1d ((*pqty));
        for (i = 1; i <= n1p; i++)
          (*pcoo)[id++] = cl1_pl_i_x (cl1, pl, i);
        for (i = n2p; i >= 1; i--)
          (*pcoo)[id++] = lp - cl1_pl_i_x (cl2, pl, i);
        ut_array_1d_scale ((*pcoo), (*pqty), 1 / lp);
      }

      // otherwise, adding a node by extending the 2 progression
      // segments then scaling the node coo to get a full segment of
      // length 1
      else
      {
        // taking 1 more node for each progression segment; both last
        // nodes will be only one in the final mesh
        l1p = cl1_pl_n_l (cl1, pl, n1p + 1);
        l2p = cl1_pl_n_l (cl2, pl, n2p + 1);
        lp = l1p + l2p;

        id = 0;
        (*pqty) = n1p + n2p + 1;
        (*pcoo) = ut_alloc_1d ((*pqty));
        for (i = 1; i <= n1p + 1; i++)  // + 1 for the 1 more node
          (*pcoo)[id++] = cl1_pl_i_x (cl1, pl, i);
        for (i = n2p; i >= 1; i--)
          (*pcoo)[id++] = lp - cl1_pl_i_x (cl2, pl, i);
        ut_array_1d_scale ((*pcoo), (*pqty), 1 / lp);
      }
    }

    // if the two progression segments do not intersect, computing the
    // number of nodes to add between them. Adding segments of length
    // clt between the parts and scaling all coos as before.
    else
    {
      // distance between the progression segments
      Dl = 1 - l1p - l2p;

      // number of nodes to add, allowing a tolerancy of 0.05
      add = ceil (ut_num_max (Dl / clt, 0.051) - 0.05) - 1;

      lp = l1p + l2p + (add + 1) * clt;

      // lp < 1 may happen due to the -0.05 tolerancy above. In this
      // case, increasing clt a little for the elements between the
      // progression parts. Not doing so would result in element lengths
      // higher than cl1 and cl2 at both extremities, which is not
      // allowed.
      if (lp < 1)
      {
        cltp = (1 - l1p - l2p) / (add + 1);
        lp = 1;
      }
      else
        cltp = clt;

      // Computing coos

      id = 0;
      (*pqty) = n1p + n2p + add;
      (*pcoo) = ut_alloc_1d ((*pqty));
      for (i = 1; i <= n1p; i++)
        (*pcoo)[id++] = cl1_pl_i_x (cl1, pl, i);
      for (i = 1; i <= add; i++)
      {
        (*pcoo)[id] = (*pcoo)[id - 1] + cltp;
        id++;
      }
      for (i = n2p; i >= 1; i--)
        (*pcoo)[id++] = lp - cl1_pl_i_x (cl2, pl, i);
      ut_array_1d_scale ((*pcoo), (*pqty), 1 / lp);
    }
  }

  // Computing characteristic lengths from the segment lengths

  (*pcl) = ut_alloc_1d (*pqty);

  // The characteristic length at a node is the max of the lengths of
  // the two elements of the node, with a change to account for the
  // elements that are between the progression parts, but have length <
  // clt.
  (*pcl)[0] = cl1;
  for (i = 1; i < (*pqty) - 1; i++)
  {
    d1 = (*pcoo)[i] - (*pcoo)[i - 1];
    d2 = (*pcoo)[i + 1] - (*pcoo)[i];

    (*pcl)[i] = ut_num_max (d1, d2);
    if ((*pcl)[i] < clt && d1 > clt / pl && d2 > clt / pl)
      (*pcl)[i] = clt;
  }
  (*pcl)[(*pqty) - 1] = cl2;

  (*pcl)[0] = ut_num_max ((*pcl)[0], (*pcoo)[1]);
  (*pcl)[(*pqty) - 1] =
    ut_num_max ((*pcl)[(*pqty) - 1],
                (*pcoo)[(*pqty) - 1] - (*pcoo)[(*pqty) - 2]);

  (*pqty) -= 2;

  return;
}

void
nem_meshing_1d_edge_proj_fixcl (struct MESH M, struct NODES *pN)
{
  int i;
  double *length = ut_alloc_1d (M.EltQty + 1);

  for (i = 1; i <= M.EltQty; i++)
    neut_mesh_elt_length (*pN, M, i, length + i);

  for (i = 2; i < (*pN).NodeQty; i++)
  {
    (*pN).NodeCl[i] = ut_num_max ((*pN).NodeCl[i], length[i - 1]);
    (*pN).NodeCl[i] = ut_num_max ((*pN).NodeCl[i], length[i]);
  }

  ut_free_1d (&length);

  return;
}
