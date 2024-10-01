/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tesr_tesrtranslate_.h"

void
net_transform_tesr_tesrtranslate (char *string, struct TESR *pTesr)
{
  int i, algoqty, cellqty, *cells = NULL, *refcells = NULL;
  char **algos = NULL;
  struct TESR Tesr2;

  neut_tesr_set_zero (&Tesr2);

  // pre
  ut_print_message (0, 3, "Initializing...\n");
  net_transform_tesr_tesrtranslate_pre (string, pTesr, &Tesr2, &cells,
                                        &refcells, &cellqty, &algos, &algoqty);

  for (i = 0; i < algoqty; i++)
  {
    if (!strncmp (algos[i], "cent", 3))
    {
      // centres
      ut_print_message (0, 3, "Minimizing distance between centres...\n");
      net_transform_tesr_tesrtranslate_centres (pTesr, cells, refcells, cellqty, Tesr2);
    }

    else if (!strcmp (algos[i], "bound"))
    {
      // boundaries
      ut_print_message (0, 3, "Minimizing distance between boundaries...\n");
      net_transform_tesr_tesrtranslate_bounds (pTesr, cells, refcells, cellqty, Tesr2);
    }
  }

  neut_tesr_free (&Tesr2);
  ut_free_2d_char (&algos, algoqty);
  ut_free_1d_int (&cells);
  ut_free_1d_int (&refcells);

  return;
}
