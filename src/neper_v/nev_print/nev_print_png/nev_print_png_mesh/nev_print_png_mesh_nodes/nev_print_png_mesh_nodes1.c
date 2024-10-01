/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_mesh_nodes_.h"

void
nev_print_png_mesh_nodes (FILE * file, struct PRINT Print, struct NODES Nodes,
                      struct MESH *Mesh, struct DATA NodeData)
{
  int i, node_qty, printnode_qty;
  char *texture = NULL;
  int *hidden = NULL;

  (void) Mesh;

  node_qty = ut_array_1d_int_sum (Print.shownode + 1, Nodes.NodeQty);
  if (node_qty == 0)
    return;

  texture = ut_alloc_1d_char (100);
  hidden = ut_alloc_1d_int (Nodes.NodeQty + 1);

  ut_array_1d_int_set (hidden + 1, Nodes.NodeQty, 0);

  printnode_qty = 0;

  for (i = 1; i <= Nodes.NodeQty; i++)
    if (Print.shownode[i])
    {
      fprintf (file,
               "#declare node%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f diffuse %f reflection %f} }\n",
               i, NodeData.Col[i][0] / 255., NodeData.Col[i][1] / 255.,
               NodeData.Col[i][2] / 255., Print.lightambient, Print.lightdiffuse, Print.lightreflection);

      sprintf (texture, "node%d", i);

      nev_print_png_sphere (file, NodeData.Coo[i], NodeData.Rad[i], texture);
      printnode_qty++;
    }

  ut_print_message (0, 4,
                    "Number of nodes        reduced by %3.0f%% (to %d).\n",
                    100 * (double) hidden[0] / node_qty,
                    node_qty - hidden[0]);

  return;
}
