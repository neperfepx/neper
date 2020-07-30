/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_elt_.h"

void
nes_pproc_elt (struct IN_S In, struct SIM *pSim, struct TESS Tess,
               struct NODES *pNodes, struct MESH *Mesh)

{
  int i, j, qty = 0, resqty, step;
  double *vals = ut_alloc_1d (Mesh[3].EltQty + 1);
  char **res = NULL, *type = NULL;
  char *dir = ut_string_paste (In.simdir, "/results/elements");
  char *filename = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  ut_list_break (In.eltres, NEUT_SEP_NODEP, &res, &resqty);

  for (i = 0; i < resqty; i++)
    if (strcmp (res[i], "inputres") && !neut_sim_res_exist (*pSim, "element", res[i]))
    {
      if (!qty++)
      {
        ut_print_message (0, 2, "Writing results...\n");
        ut_dir_openmessage (dir, "w");
      }

      ut_sys_mkdir ("%s/results/elements/%s", In.simdir, res[i]);

      ut_print_message (0, 4, "%s ", res[i]);
      for (j = 0; j < 49 - (int) strlen (res[i]); j++)
        printf (".");
      printf (" ");

      for (step = 0; step <= (*pSim).StepQty; step++)
      {
        neut_sim_updatenodes (*pSim, step, pNodes);

        if (neut_mesh_exprisvar ("elt3d", res[i]))
        {
          sprintf (filename, "%s/%s/%s.step%d", dir, res[i], res[i], step);
          file = ut_file_open (filename, "W");

          int qty;
          double *tmp = ut_alloc_1d (10);
          for (j = 1; j <= Mesh[3].EltQty; j++)
          {
            neut_mesh_var_val (*pNodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3], Mesh[4],
                               Tess, NULL, NULL, NULL, NULL, 0, "elt3d", j, res[i], &tmp, &qty, &type);
            ut_array_1d_fprintf (file, tmp, qty, "%.12f");
          }
          ut_free_1d (&tmp);

          ut_file_close (file, filename, "W");
        }

        else
        {
          neut_mesh_entity_expr_val (*pNodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3], Mesh[4],
                                     Tess, NULL, NULL, NULL, NULL, "elt3d", res[i], vals, &type);

          sprintf (filename, "%s/%s/%s.step%d", dir, res[i], res[i], step);

          file = ut_file_open (filename, "W");
          for (j = 1; j <= Mesh[3].EltQty; j++)
            fprintf (file, "%.12f\n", vals[j]);
          ut_file_close (file, filename, "W");
        }
      }

      neut_sim_updatenodes (*pSim, 0, pNodes);

      neut_sim_addeltres (pSim, res[i], 1);
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
