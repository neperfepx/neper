/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_fprintf_.h"

void
neut_sim_fprintf (char *dir, struct SIM Sim, char *mode)
{
  int i, j, qty;
  char *oldfilename = ut_string_paste (dir, "/report");
  char *filename = ut_string_paste (dir, "/.sim");
  FILE *file = ut_file_open (filename, mode);

  if (ut_file_exist (oldfilename))
    remove (oldfilename);

  fprintf (file, "***sim\n");
  fprintf (file, " **format\n");
  fprintf (file, "   1.0\n");
  fprintf (file, " **input\n");
  if (Sim.tess)
    fprintf (file, "  *tess\n   %s\n", Sim.tess);
  if (Sim.tesr)
    fprintf (file, "  *tesr\n   %s\n", Sim.tesr);
  if (Sim.msh)
    fprintf (file, "  *msh\n   %s\n", Sim.msh);
  if (Sim.bcs)
    fprintf (file, "  *bcs\n   %s\n", Sim.msh);
  if (Sim.ori)
    fprintf (file, "  *ori\n   %s\n", Sim.msh);
  if (Sim.phase)
    fprintf (file, "  *phase\n   %s\n", Sim.msh);
  if (Sim.config)
    fprintf (file, "  *config\n   %s\n", Sim.config);

  fprintf (file, " **general\n");
  fprintf (file, "   %d %d %d %d %d\n",
                 Sim.CellQty, Sim.NodeQty, Sim.EltQty, Sim.ElsetQty,
                 Sim.PartQty);
  if (Sim.OriDes)
  {
    fprintf (file, "  *orides\n");
    fprintf (file, "   %s\n", Sim.OriDes);
  }

  for (i = 0; i < Sim.EntityQty; i++)
  {
    fprintf (file, " **entity %s\n", Sim.Entities[i]);

    if (!neut_sim_entity_internal (Sim.Entities[i]))
    {
      fprintf (file, "  *type\n");
      fprintf (file, "   %s\n", Sim.EntityType[i]);

      if (Sim.EntityMemberQty[i])
      {
        fprintf (file, "  *member\n");
        fprintf (file, "   %d", Sim.EntityMemberQty[i]);
        if (Sim.EntityMemberExpr[i])
        {
          for (j = 0; j < Sim.EntityMemberQty[i]; j++)
            fprintf (file, " %s", Sim.EntityMemberExpr[i][j]);
        }
        fprintf (file, "\n");
      }
    }

    if (Sim.EntityResQty[i])
    {
      fprintf (file, "  *result\n");
      fprintf (file, "   %d\n  ", Sim.EntityResQty[i]);
      if (Sim.EntityResQty[i])
        for (j = 0; j < Sim.EntityResQty[i]; j++)
        {
          fprintf (file, " %s", Sim.EntityRes[i][j]);
          if (Sim.EntityResExpr[i][j] && strlen (Sim.EntityResExpr[i][j]))
            fprintf (file, "%s%s", NEUT_SEP_DEP, Sim.EntityResExpr[i][j]);
        }
      fprintf (file, "\n");
    }
  }

  if (Sim.OriSpace)
  {
    fprintf (file, " **orispace\n");
    fprintf (file, "   %s\n", Sim.OriSpace);
  }

  if (Sim.StepQty > 0)
  {
    fprintf (file, " **step\n");
    fprintf (file, "   %d\n", Sim.StepQty);

    qty = ut_array_1d_int_valnb (Sim.StepState + 1, Sim.StepQty, 0);
    if (qty)
    {
      fprintf (file, "  *printed\n");
      fprintf (file, "   %d\n", Sim.StepQty - qty);
      fprintf (file, "  ");
      for (i = 1; i <= Sim.StepQty; i++)
        if (!Sim.StepState[i])
          fprintf (file, " %d", i);
      fprintf (file, "\n");
    }
  }

  if (Sim.RestartId || Sim.RestartFiles)
  {
    fprintf (file, " **restart\n");
    fprintf (file, "   %d %d\n", Sim.RestartId, Sim.RestartFiles);
  }

  fprintf (file, "***end\n");

  ut_file_close (file, filename, mode);
  ut_free_1d_char (&oldfilename);
  ut_free_1d_char (&filename);

  return;
}

void
neut_sim_verbose (struct SIM Sim)
{
  int i;

  ut_print_message (0, 3, "Input files:");
  if (Sim.tess)
    printf (" tess");
  if (Sim.tesr)
    printf (" tesr");
  if (Sim.msh)
    printf (" msh");
  if (Sim.bcs)
    printf (" bcs");
  if (Sim.ori)
    printf (" ori");
  if (Sim.phase)
    printf (" phase");
  if (Sim.config)
    printf (" config");
  printf ("\n");

  if (Sim.RestartId || Sim.RestartFiles)
  {
    ut_print_message (0, 3, "Restart id       : %d\n", Sim.RestartId);
    ut_print_message (0, 3, "Restart files    : %s\n", Sim.RestartFiles ? "yes" : "no");
  }

  if (Sim.CellQty)
    ut_print_message (0, 3, "Cell number      : %d\n", Sim.CellQty);

  if (Sim.NodeQty)
    ut_print_message (0, 3, "Node number      : %d\n", Sim.NodeQty);

  if (Sim.EltQty)
    ut_print_message (0, 3, "Element number   : %d\n", Sim.EltQty);

  if (Sim.ElsetQty)
    ut_print_message (0, 3, "Elset number     : %d\n", Sim.ElsetQty);

  if (Sim.PartQty)
    ut_print_message (0, 3, "Partition number : %d\n", Sim.PartQty);

  ut_print_message (0, 3, "Step number      : %d\n", Sim.StepQty);

  if (Sim.OriSpace)
    ut_print_message (0, 3, "Orientation space: %s\n", Sim.OriSpace);

  for (i = 0; i < Sim.EntityQty; i++)
    neut_sim_verbose_results (Sim.Entities[i], Sim.EntityRes[i], Sim.EntityResQty[i]);

  return;
}
