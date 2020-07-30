/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_cleaning_.h"

void
nem_cleaning_singnodedup (struct MESH *Mesh, struct NODES *pNodes,
                          int ***pFoDNodes)
{
  int i, j, x, y, z, l, m, n, l2, m2, n2, o, elt, ininodeqty, status;
  int ***array = ut_alloc_3d_int (2, 2, 2);
  int ***arrayid = ut_alloc_3d_int (2, 2, 2);
  int eltnodeqty = neut_elt_nodeqty (Mesh[3].EltType, Mesh[3].Dimension,
                                     Mesh[3].EltOrder);
  double **EltCoo = ut_alloc_2d (Mesh[3].EltQty + 1, 3);

  for (i = 1; i <= Mesh[3].EltQty; i++)
    neut_mesh_elt_centre (*pNodes, Mesh[3], i, EltCoo[i]);

  ininodeqty = (*pNodes).NodeQty;

  char *message = ut_alloc_1d_char (1000);
  ut_print_progress (stdout, 0, ininodeqty, "%3.0f%%", message);

  for (i = 1; i <= ininodeqty; i++)
  {
    if (Mesh[3].NodeElts[i][0] >= 2 && Mesh[3].NodeElts[i][0] <= 4)
    {
      status = 0;
      ut_array_3d_int_zero (array, 2, 2, 2);
      ut_array_3d_int_zero (arrayid, 2, 2, 2);

      for (j = 1; j <= Mesh[3].NodeElts[i][0]; j++)
      {
        elt = Mesh[3].NodeElts[i][j];
        x = (EltCoo[elt][0] < (*pNodes).NodeCoo[i][0]) ? 0 : 1;
        y = (EltCoo[elt][1] < (*pNodes).NodeCoo[i][1]) ? 0 : 1;
        z = (EltCoo[elt][2] < (*pNodes).NodeCoo[i][2]) ? 0 : 1;
        array[x][y][z] = 1;
        arrayid[x][y][z] = elt;
      }

      for (l = 0; l < 2; l++)
        for (m = 0; m < 2; m++)
          for (n = 0; n < 2; n++)
            if (array[l][m][n] == 1)
            {
              // status = 0 means first time we check the node.
              // The element should keep it anyway.
              if (status == 0)
                status = 1;
              else
              {
                if (status == 0)
                  status = 1;

                l2 = (l == 0) ? 1 : 0;
                m2 = (m == 0) ? 1 : 0;
                n2 = (n == 0) ? 1 : 0;

                elt = arrayid[l][m][n];

                // should the elt have his own node?
                // is its sharing a face with another elt?
                if (array[l2][m][n] == 0 && array[l][m2][n] == 0
                    && array[l][m][n2] == 0)
                {
                  neut_nodes_addnode (pNodes, (*pNodes).NodeCoo[i], 0);
                  ut_array_1d_int_findnreplace (Mesh[3].EltNodes[elt],
                                                eltnodeqty, i,
                                                (*pNodes).NodeQty);
                  if (pFoDNodes != NULL)
                    for (o = 1; o <= 6; o++)
                    {
                      if (ut_array_1d_int_eltpos
                          ((*pFoDNodes)[o] + 1, (*pFoDNodes)[o][0], i) != -1)
                      {
                        (*pFoDNodes)[o][0]++;
                        (*pFoDNodes)[o] =
                          ut_realloc_1d_int ((*pFoDNodes)[o],
                                             (*pFoDNodes)[o][0] + 1);
                        (*pFoDNodes)[o][(*pFoDNodes)[o][0]] =
                          (*pNodes).NodeQty;
                      }
                    }
                }
              }
            }
    }
    ut_print_progress (stdout, i, ininodeqty, "%3.0f%%", message);
  }

  ut_free_1d_char (&message);

  int add = (*pNodes).NodeQty - ininodeqty;
  ut_print_message (0, 3, "%d %s added.\n", add,
                    (add <= 1) ? "node" : "nodes");

  ut_free_2d_int (&Mesh[3].NodeElts, ininodeqty);
  Mesh[3].NodeElts = NULL;
  neut_mesh_init_nodeelts (Mesh + 3, (*pNodes).NodeQty);

  ut_free_3d_int (&array, 2, 2);
  ut_free_3d_int (&arrayid, 2, 2);
  ut_free_2d (&EltCoo, Mesh[3].EltQty + 1);

  return;
}

void
nem_cleaning_dupnodemerge (struct NODES *pNodes, struct MESH *Mesh,
                           double eps)
{
  int i, j;

  int *slave_master = ut_alloc_1d_int ((*pNodes).NodeQty + 1);

  // Changing slaves to masters
  for (i = 1; i <= (*pNodes).NodeQty; i++)
    slave_master[i] = i;

  char *message = ut_alloc_1d_char (1000);
  ut_print_progress (stdout, 0, 1, "%3.0f%%", message);

  int testqty = 0;
  int tottestqty = ((*pNodes).NodeQty * ((*pNodes).NodeQty - 1)) / 2;

  for (i = 1; i < (*pNodes).NodeQty; i++)
  {
    for (j = i + 1; j <= (*pNodes).NodeQty; j++)
      if (neut_nodes_dist_pair (*pNodes, i, j) < eps)
        slave_master[j] = slave_master[i];

    testqty += (*pNodes).NodeQty - i;

    ut_print_progress (stdout, testqty, tottestqty, "%3.0f%%", message);
  }

  for (i = 0; i <= 3; i++)
    neut_mesh_switch (Mesh + i, slave_master, NULL, NULL);

  // Removing useless nodes
  int *node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  int NodeQty = 0;

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    node_nbs[i] = (slave_master[i] == i) ? ++NodeQty : -1;

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    if (node_nbs[i] != -1)
      ut_array_1d_memcpy ((*pNodes).NodeCoo[i], 3,
                          (*pNodes).NodeCoo[node_nbs[i]]);

  for (i = 0; i <= 3; i++)
    neut_mesh_switch (Mesh + i, node_nbs, NULL, NULL);

  ut_print_message (0, 3, "%d %s removed.\n", (*pNodes).NodeQty - NodeQty,
                    ((*pNodes).NodeQty - NodeQty <= 1) ? "node" : "nodes");

  (*pNodes).NodeQty = NodeQty;

  ut_free_1d_char (&message);
  ut_free_1d_int (&slave_master);
  ut_free_1d_int (&node_nbs);

  return;
}
