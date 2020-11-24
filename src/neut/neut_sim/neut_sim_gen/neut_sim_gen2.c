/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_gen_.h"

void
neut_sim_fprintf_results (FILE *file, char *entity, char **res, char **resexpr,
                          int *reswritten, int resqty)
{
  int i;

  fprintf (file, "results_%s", entity);
  for (i = 0; i < resqty; i++)
    if (reswritten[i])
    {
      fprintf (file, " %s", res[i]);
      if (resexpr[i])
        fprintf (file, "%s%s", NEUT_SEP_DEP, resexpr[i]);
    }
  fprintf (file, "\n");

  return;
}

void
neut_sim_verbose_results (char *entity, char **res, int resqty)
{
  int i, nb;

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

  return;
}
