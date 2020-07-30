/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_elset_.h"

void
nes_pproc_elset_init_output (struct IN_S In, int elset)
{
  int j, varqty;
  char **var = NULL;
  ut_list_break (In.elsetres, NEUT_SEP_NODEP, &var, &varqty);

  ut_sys_mkdir ("%s/elsets/elset%d", In.simdir, elset);

  for (j = 0; j < varqty; j++)
    ut_sys_mkdir ("%s/elsets/elset%d/%s", In.simdir, elset, var[j]);

  ut_free_2d_char (&var, varqty);
  return;
}

void
nes_pproc_elset_extract (struct IN_S In, struct SIM Sim,
                         struct MESH Mesh, int elset)
{
  int j, k, step_beg, varqty;
  char **var = NULL;
  char *entity = ut_alloc_1d_char (10);

  FILE *stepid_main = NULL;
  FILE *stepid_elset = NULL;

  int EltQty, NodeQty, lineqty;
  int *elts = NULL, *nodes = NULL, *lines = NULL;

  char *stepid_main_pref = ut_alloc_1d_char (1000);
  char *stepid_main_name = ut_alloc_1d_char (1000);
  char *stepid_elset_pref = ut_alloc_1d_char (1000);
  char *stepid_elset_name = ut_alloc_1d_char (1000);

  ut_list_break (In.elsetres, NEUT_SEP_NODEP, &var, &varqty);

  elts = Mesh.Elsets[elset] + 1;
  EltQty = Mesh.Elsets[elset][0];
  neut_mesh_elset_nodes (Mesh, elset, &nodes, &NodeQty);

  for (j = 0; j < varqty; j++)
    if (ut_list_testelt (In.elsetres, NEUT_SEP_NODEP, var[j]))
    {
      sprintf (stepid_main_pref, "%s/data/%s/%s-step",
               In.simdir, var[j], var[j]);
      sprintf (stepid_elset_pref, "%s/elsets/elset%d/%s/%s-step",
               In.simdir, elset, var[j], var[j]);

      neut_sim_var_entity (var[j], entity);
      lines = (!strcmp (entity, "elt")) ? elts : nodes;
      lineqty = (!strcmp (entity, "elt")) ? EltQty : NodeQty;

      neut_sim_var_startincr (var[j], &step_beg);
      if (step_beg < 0)
        ut_print_neperbug ();

      for (k = step_beg; k <= Sim.StepQty; k++)
      {
        sprintf (stepid_main_name, "%s%d", stepid_main_pref, k);
        sprintf (stepid_elset_name, "%s%d", stepid_elset_pref, k);
        stepid_elset = ut_file_open (stepid_elset_name, "W");

        if (ut_file_exist (stepid_main_name))
        {
          stepid_main = ut_file_open (stepid_main_name, "R");
          ut_file_cp_lines (stepid_main, stepid_elset, lines, lineqty);
          ut_file_close (stepid_main, stepid_main_name, "R");
        }
        else
          nes_pproc_elset_compute (elset, var[j], k, In.simdir,
                                   stepid_elset);

        ut_file_close (stepid_elset, stepid_elset_name, "W");
      }
    }

  ut_free_1d_int (&nodes);

  ut_free_2d_char (&var, varqty);
  ut_free_1d_char (&entity);

  return;
}
