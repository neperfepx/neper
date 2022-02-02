/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_fprintf_.h"

void
neut_sim_fprintf_results (FILE *file, char *entity, char **res, char **resexpr,
                          int resqty)
{
  int i;

  fprintf (file, "results_%s", entity);

  for (i = 0; i < resqty; i++)
  {
    fprintf (file, " %s", res[i]);
    if (resexpr[i] && strlen (resexpr[i]))
      fprintf (file, "%s%s", NEUT_SEP_DEP, resexpr[i]);
  }
  fprintf (file, "\n");

  return;
}

void
neut_sim_fprintf_entities (FILE *file, struct SIM Sim)
{
  int i, j;

  fprintf (file, "entities");

  for (i = 0; i < Sim.EntityQty; i++)
    fprintf (file, " %s", Sim.Entities[i]);
  fprintf (file, "\n");

  for (i = 0; i < Sim.EntityQty; i++)
    if (Sim.EntityType[i])
    {
      fprintf (file, "entity_type %s %s", Sim.Entities[i], Sim.EntityType[i]);
      fprintf (file, "\n");
    }

  for (i = 0; i < Sim.EntityQty; i++)
    if (Sim.EntityMemberExpr[i])
    {
      fprintf (file, "entity_expr %s", Sim.Entities[i]);
      for (j = 0; j < Sim.EntityMemberQty[i]; j++)
        fprintf (file, " %s", Sim.EntityMemberExpr[i][j]);
      fprintf (file, "\n");
    }

  return;
}

void
neut_sim_fprintf_restart (FILE *file, struct SIM Sim)
{
  if (Sim.RestartId || Sim.RestartFiles)
    fprintf (file, "restart_id %d\n", Sim.RestartId);

  if (Sim.RestartId || Sim.RestartFiles)
    fprintf (file, "restart_files %d\n", Sim.RestartFiles);

  return;
}

void
neut_sim_verbose_results (char *entity_in, char **res, int resqty)
{
  int i, nb;
  char *entity = ut_alloc_1d_char (strlen (entity_in) + 1);

  entity[0] = toupper (entity_in[0]);
  for (i = 1; i < (int) strlen (entity_in); i++)
    entity[i] = entity_in[i];
  if (entity[strlen (entity) - 1] == 's')
    entity[strlen (entity) - 1] = '\0';

  ut_print_message (0, 3, "%s results", entity);
  for (i = strlen (entity); i < 9; i++)
    printf (" ");
  printf (":");

  nb = 35;
  for (i = 0; i < resqty; i++)
  {
    if (strlen (res[i]) + nb > 72)
    {
      printf ("\n");
      for (nb = 0; nb < 33; nb++)
        printf (" ");
    }

    printf (" %s", res[i]);
    nb += strlen (res[i]) + 1;
  }
  printf ("\n");

  ut_free_1d_char (&entity);

  return;
}
