/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
  int NodeQty, *nodes = NULL;

  neut_mesh_centre (*pNodes, Mesh[dim], C);

  if (sscanf (explode, "explode(%lf", &factor) != 1)
    ut_print_message (2, 4, "Failed to parse expression `%s'.\n", explode);

  for (i = 1; i <= Mesh[dim].ElsetQty; i++)
  {
    neut_mesh_elset_centre (*pNodes, Mesh[dim], i, c);
    ut_array_1d_sub (C, c, 3, v);
    dist = ut_array_1d_norm (v, 3);
    ut_array_1d_scale (v, 3, dist * factor);

    neut_mesh_elset_nodes (Mesh[dim], i, &nodes, &NodeQty);

    for (j = 0; j < NodeQty; j++)
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
  neut_mesh_reset (Mesh + 3);
  neut_mesh_reset (Mesh + 1);
  neut_mesh_reset (Mesh + 0);

  ut_free_1d (&eq);
  ut_free_1d_int (&elt_newold);
  ut_free_2d_int (&node_newold, SNodes.NodeQty + 1);
  ut_free_1d (&node_fact);

  return;
}

void
nem_transform_node (char *node, struct NODES *pNodes)
{
  int nblines, nbwords, colqty, valqty;
  char *fct = NULL, **vars = NULL, **vals = NULL, *filename = ut_alloc_1d_char (1000);

  ut_string_function (node, &fct, &vars, &vals, &valqty);

  if (strcmp (fct, "node"))
    abort ();

  ut_string_string (vals[0], &filename);

  nblines = ut_file_nblines (filename);
  nbwords = ut_file_nbwords (filename);
  colqty = nbwords / nblines;

  if (nblines != (*pNodes).NodeQty)
    ut_print_message (2, 4, "Wrong number of entries in file `%s'.\n", vals[0]);

  ut_array_2d_fnscanf (filename, (*pNodes).NodeCoo + 1, (*pNodes).NodeQty, colqty, "r");

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);
  ut_free_1d_char (&filename);

  return;
}

void
nem_transform_ori (char *ori, struct TESS Tess, struct NODES Nodes, struct MESH *Mesh)
{
  int nblines, valqty, dim = Tess.Dim;
  char *fct = NULL, **vars = NULL, **vals = NULL, *filename = ut_alloc_1d_char (1000);
  struct OL_SET OSet;

  ut_string_function (ori, &fct, &vars, &vals, &valqty);

  if (strcmp (fct, "ori"))
    abort ();

  ut_string_string (ori, &filename);
  filename = ut_realloc_1d_char (filename, strlen (filename) + 2);
  ut_string_fnrs (filename, "ori(", "file(", 1);

  nblines = ut_file_nblines (vals[0]);

  OSet = ol_set_alloc (nblines, Tess.CellCrySym);

  net_ori_file (filename, &OSet);

  if (nblines == Mesh[dim].ElsetQty)
    ut_array_2d_memcpy (OSet.q, Mesh[dim].ElsetQty, 4, Mesh[dim].ElsetOri + 1);

  else if (nblines == Mesh[dim].EltQty)
  {
    if (!Mesh[dim].EltOri)
      Mesh[dim].EltOri = ut_alloc_2d (Mesh[dim].EltQty + 1, 4);
    if (!Mesh[dim].EltOriDes)
      ut_string_string (Mesh[dim].ElsetOriDes, &Mesh[dim].EltOriDes);
    ut_array_2d_memcpy (OSet.q, Mesh[dim].EltQty, 4, Mesh[dim].EltOri + 1);

    neut_mesh_init_elsetori (Nodes, Mesh + 3);
  }

  else
    ut_print_message (2, 4, "Wrong number of entries in file `%s'.\n", vals[0]);

  ol_set_free (&OSet);
  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);
  ut_free_1d_char (&filename);

  return;
}
