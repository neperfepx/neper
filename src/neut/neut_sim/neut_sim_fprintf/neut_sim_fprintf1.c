/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_fprintf_.h"

void
neut_sim_fprintf (char *dir, struct SIM Sim, char *mode)
{
  int i, qty, curqty;
  char *filename = ut_string_paste (dir, "/report");
  FILE *file = ut_file_open (filename, mode);

  fprintf (file, "number_of_nodes %d\n", Sim.NodeQty);
  fprintf (file, "number_of_elements %d\n", Sim.EltQty);
  fprintf (file, "number_of_elsets %d\n", Sim.ElsetQty);
  if (Sim.PartQty > 0)
    fprintf (file, "number_of_partitions %d\n", Sim.PartQty);
  fprintf (file, "number_of_steps %d\n", Sim.StepQty);

  qty = ut_array_1d_int_valnb (Sim.StepState + 1, Sim.StepQty, 0);
  curqty = 0;
  if (qty != Sim.StepQty)
  {
    fprintf (file, "printed_steps ");
    for (i = 1; i <= Sim.StepQty; i++)
      if (!Sim.StepState[i])
        fprintf (file, "%d%s", i, ++curqty < qty ? " " : "\n");
  }

  fprintf (file, "orientation_definition %s\n", Sim.OriDes);

  if (Sim.OriSpace)
    fprintf (file, "orientation_space %s\n", Sim.OriSpace);

  neut_sim_fprintf_entities (file, Sim);

  for (i = 0; i < Sim.EntityQty; i++)
    neut_sim_fprintf_results (file, Sim.Entities[i], Sim.EntityRes[i],
                              Sim.EntityResExpr[i], Sim.EntityResQty[i]);

  neut_sim_fprintf_restart (file, Sim);

  ut_file_close (file, filename, mode);
  ut_free_1d_char (&filename);

  return;
}

void
neut_sim_verbose (struct SIM Sim)
{
  int i;

  if (Sim.RestartId || Sim.RestartFiles)
  {
    ut_print_message (0, 3, "Restart id       : %d\n", Sim.RestartId);
    ut_print_message (0, 3, "Restart files    : %s\n", Sim.RestartFiles ? "yes" : "no");
  }
  ut_print_message (0, 3, "Node number      : %d\n", Sim.NodeQty);
  ut_print_message (0, 3, "Element number   : %d\n", Sim.EltQty);
  if (Sim.ElsetQty)
    ut_print_message (0, 3, "Elset number     : %d\n", Sim.ElsetQty);
  ut_print_message (0, 3, "Partition number : %d\n", Sim.PartQty);
  ut_print_message (0, 3, "Step number      : %d\n", Sim.StepQty);

  if (Sim.OriSpace)
    ut_print_message (0, 3, "Orientation space: %s\n", Sim.OriSpace);

  for (i = 0; i < Sim.EntityQty; i++)
    neut_sim_verbose_results (Sim.Entities[i], Sim.EntityRes[i], Sim.EntityResQty[i]);

  return;
}
