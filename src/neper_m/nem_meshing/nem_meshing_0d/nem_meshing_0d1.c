/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_0d_.h"

void
nem_meshing_0d (struct TESS Tess, struct MESHPARA MeshPara,
                struct NODES *pNodes, struct MESH *Mesh)
{
  int i, master;
  struct NODES N;
  struct MESH M;
  char *message = ut_alloc_1d_char (8);

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);

  ut_array_1d_int_memcpy (Tess.Periodic, 3, (*pNodes).Periodic);

  Mesh[0] = neut_mesh_alloc (0, "tri", 1, 0, 0);

  ut_print_message (0, 2, "0D meshing... ");

  if (MeshPara.dim >= 0)
  {
    ut_print_progress (stdout, 0, Tess.VerQty, "%3.0f%%", message);

    for (i = 1; i <= Tess.VerQty; i++)
    {
      if (strcmp (Tess.Type, "periodic") || !Tess.PerVerMaster[i])
      {
        nem_meshing_0d_ver (Tess, i, MeshPara, &N, &M);
        nem_meshing_0d_addvermesh (N, M, 0, NULL, pNodes, Mesh);
      }
      else
      {
        master = Tess.PerVerMaster[i];
        neut_mesh_elset_mesh (*pNodes, Mesh[0], master, &N, &M, NULL);
        neut_nodes_shift (&N, Tess.PerVerShift[i][0] * Tess.PeriodicDist[0],
                          Tess.PerVerShift[i][1] * Tess.PeriodicDist[1],
                          Tess.PerVerShift[i][2] * Tess.PeriodicDist[2]);
        nem_meshing_0d_addvermesh (N, M, master, Tess.PerVerShift[i], pNodes,
                                   Mesh);
      }

      ut_print_progress_nonl (stdout, i, Tess.VerQty, "%3.0f%%", message);
    }
  }

  printf ("\n");

  neut_mesh_init_nodeelts (Mesh + 0, (*pNodes).NodeQty);
  neut_mesh_init_eltelset (Mesh + 0, NULL);
  if (!strcmp (Tess.Type, "periodic"))
    neut_nodes_init_nodeslave (pNodes);

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  ut_free_1d_char (&message);

  return;
}
