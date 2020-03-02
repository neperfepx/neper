/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transform_.h"

void
nem_transform_smooth (char *smooth, struct TESS Tess, struct NODES *pNodes,
                      struct MESH *Mesh)
{
  int i, dim, iterqty;
  double factor;
  char *type = ut_alloc_1d_char (strlen (smooth));
  char *message = ut_alloc_1d_char (1000);
  char *prevmessage = ut_alloc_1d_char (1000);

  if (sscanf (smooth, "smooth(%lf,%d,%s", &factor, &iterqty, type) != 3)
    ut_print_message (2, 4, "Failed to parse expression `%s'.\n", smooth);
  type[strlen (type) - 1] = '\0';

  ut_print_message (0, 4, "");
  for (i = 1; i <= iterqty; i++)
  {
    sprintf (message, "Iteration %4d/%d", i, iterqty);
    ut_print_progress (stdout, 4, INT_MAX, message, prevmessage);

    for (dim = 1; dim < neut_mesh_array_dim (Mesh); dim++)
      nem_smoothing_laplacian (Tess, pNodes, Mesh, dim, factor, 1, type);
  }

  ut_free_1d_char (&type);
  ut_free_1d_char (&message);
  ut_free_1d_char (&prevmessage);

  return;
}

void
nem_transform_explode (char *explode, struct NODES *pNodes, struct MESH *Mesh)
{
  int i, j, dim = neut_mesh_array_dim (Mesh);
  double factor, dist;
  double *c = ut_alloc_1d (3), *C = ut_alloc_1d (3);
  double *v = ut_alloc_1d (3);
  int nodeqty, *nodes = NULL;

  neut_mesh_centre (*pNodes, Mesh[dim], C);

  if (sscanf (explode, "explode(%lf", &factor) != 1)
    ut_print_message (2, 4, "Failed to parse expression `%s'.\n", explode);

  for (i = 1; i <= Mesh[dim].ElsetQty; i++)
  {
    neut_mesh_elset_centre (*pNodes, Mesh[dim], i, c);
    ut_array_1d_sub (C, c, 3, v);
    dist = ut_array_1d_norm (v, 3);
    ut_array_1d_scale (v, 3, dist * factor);

    neut_mesh_elset_nodes (Mesh[dim], i, &nodes, &nodeqty);

    for (j = 0; j < nodeqty; j++)
      ut_array_1d_add ((*pNodes).NodeCoo[nodes[j]], v, 3,
                       (*pNodes).NodeCoo[nodes[j]]);
  }

  ut_free_1d (&C);
  ut_free_1d (&c);
  ut_free_1d_int (&nodes);

  return;
}

void
nem_transform_slice (char *slice, struct NODES *pNodes, struct MESH *Mesh)
{
  double *eq = ut_alloc_1d (4);
  struct NODES SNodes;
  struct MESH SMesh;
  int *elt_newold = NULL;
  int **node_newold = NULL;
  double *node_fact = NULL;

  neut_nodes_set_zero (&SNodes);
  neut_mesh_set_zero (&SMesh);

  sscanf (slice, "slice(%lf,%lf,%lf,%lf)", eq, eq + 1, eq + 2, eq + 3);

  neut_mesh3d_slice (*pNodes, Mesh[3], eq, &SNodes, &SMesh, &elt_newold,
                     &node_newold, &node_fact);

  neut_nodes_memcpy (SNodes, pNodes);
  neut_mesh_memcpy (SMesh, Mesh + 2);
  neut_nodes_free (&SNodes);
  neut_mesh_free (&SMesh);
  neut_mesh_free (Mesh + 3);
  neut_mesh_free (Mesh + 1);
  neut_mesh_free (Mesh + 0);

  ut_free_1d (&eq);
  ut_free_1d_int (&elt_newold);
  ut_free_2d_int (&node_newold, SNodes.NodeQty + 1);
  ut_free_1d (&node_fact);

  return;
}
