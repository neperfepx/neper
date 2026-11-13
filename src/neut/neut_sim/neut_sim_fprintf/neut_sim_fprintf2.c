/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_fprintf_.h"

void
neut_sim_verbose_results (char *entity_in, char **res, int resqty)
{
  int i, nb;
  char *entity = ut_alloc_1d_char (strlen (entity_in) + 1);

  entity[0] = toupper (entity_in[0]);
  for (i = 1; i < (int) strlen (entity_in); i++)
    entity[i] = entity_in[i];
  if (strcmp (entity, "Tess") && entity[strlen (entity) - 1] == 's')
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

void
neut_sim_fprintf_entity (struct SIM Sim, int entity, char *mode)
{
  int i;
  char *filename = NULL;
  FILE *file = NULL;

  ut_string_sprintf (&filename, "%s/results/%s/%s.def", Sim.simdir, Sim.Entities[entity],
                     Sim.Entities[entity]);

  file = ut_file_open (filename, mode);

  fprintf (file, "*type\n");
  fprintf (file, " %s\n", Sim.EntityType[entity]);

  if (Sim.EntityMemberExpr[entity])
  {
    fprintf (file, "*member\n");
    for (i = 0; i < Sim.EntityMemberQty[entity]; i++)
      fprintf (file, " %s\n", Sim.EntityMemberExpr[entity][i]);
  }

  ut_file_close (file, filename, mode);

  ut_free_1d_char (&filename);

  return;
}

void
neut_sim_fprintf_res (struct SIM Sim, int entity, int res, char *mode)
{
  int hastype, hasexpr;
  char *entitydir = NULL, *dir = NULL, *filename = NULL, *knowntype = NULL;
  char *name = Sim.EntityRes[entity][res];
  char *expr = Sim.EntityResExpr[entity] ? Sim.EntityResExpr[entity][res] : NULL;
  char *type = Sim.EntityResType[entity] ? Sim.EntityResType[entity][res] : NULL;
  FILE *file = NULL;

  neut_sim_entity_entitydir (Sim.Entities[entity], &entitydir);
  ut_string_sprintf (&dir, "%s/results/%s/%s", Sim.simdir, entitydir, name);

  if (ut_sys_isdir ("%s", dir))
  {
    // the type is not needed if given by the name
    neut_sim_simres_knowntype (name, &knowntype);
    hastype = type && ut_string_strcmp (type, knowntype);
    hasexpr = expr && strlen (expr) && strcmp (expr, name);

    if (hastype || hasexpr)
    {
      ut_string_sprintf (&filename, "%s/%s.def", dir, name);

      file = ut_file_open (filename, mode);

      if (hastype)
        fprintf (file, "*type\n %s\n", type);

      if (hasexpr)
        fprintf (file, "*expr\n %s\n", expr);

      ut_file_close (file, filename, mode);
    }
  }

  ut_free_1d_char (&entitydir);
  ut_free_1d_char (&dir);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&knowntype);

  return;
}
