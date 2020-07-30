/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_node_.h"

void
nes_pproc_node (struct IN_S In, struct SIM *pSim, struct TESS Tess,
               struct NODES *pNodes, struct MESH *Mesh)

{
  int i, j, qty = 0, resqty, step;
  double *vals = ut_alloc_1d ((*pNodes).NodeQty + 1);
  char **res = NULL, *type = NULL;
  char *dir = ut_string_paste (In.simdir, "/results/nodes");
  char *filename = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  ut_list_break (In.noderes, NEUT_SEP_NODEP, &res, &resqty);

  for (i = 0; i < resqty; i++)
    if (strcmp (res[i], "inputres") && !neut_sim_res_exist (*pSim, "node", res[i]))
    {
      if (!qty++)
      {
        ut_print_message (0, 2, "Writing results...\n");
        ut_dir_openmessage (dir, "w");
      }

      ut_sys_mkdir ("%s/results/nodes/%s", In.simdir, res[i]);

      ut_print_message (0, 4, "%s ", res[i]);
      for (j = 0; j < 49 - (int) strlen (res[i]); j++)
        printf (".");
      printf (" ");

      for (step = 0; step <= (*pSim).StepQty; step++)
      {
        neut_sim_updatenodes (*pSim, step, pNodes);

        neut_mesh_entity_expr_val (*pNodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3], Mesh[4],
                                   Tess, NULL, NULL, NULL, NULL, "node", res[i], vals, &type);

        sprintf (filename, "%s/%s/%s.step%d", dir, res[i], res[i], step);
        file = ut_file_open (filename, "W");
        for (j = 1; j <= (*pNodes).NodeQty; j++)
          fprintf (file, "%.12f\n", vals[j]);
        ut_file_close (file, filename, "W");
      }
      neut_sim_updatenodes (*pSim, 0, pNodes);

      neut_sim_addnoderes (pSim, res[i], 1);
      neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

      printf ("100%%\n");
    }

  if (qty)
    ut_dir_closemessage (dir, "w");

  ut_free_2d_char (&res, resqty);
  ut_free_1d_char (&type);
  ut_free_1d (&vals);
  ut_free_1d_char (&dir);
  ut_free_1d_char (&filename);

  return;
}
