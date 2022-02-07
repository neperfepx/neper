/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
