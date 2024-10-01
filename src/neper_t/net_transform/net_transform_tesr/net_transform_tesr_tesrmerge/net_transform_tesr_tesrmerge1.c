/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tesr_tesrmerge_.h"

void
net_transform_tesr_tesrmerge (char *string, struct TESR *pTesr)
{
  int i, CellQty, *CellIds = NULL, **CellStates = NULL;
  struct TESR **pTesrs = calloc (2, sizeof (struct TESR*));
  struct TESR *Tesrs = calloc (2, sizeof (struct TESR));
  char *strategy = NULL;

  for (i = 0; i < 2; i++)
  {
    neut_tesr_set_zero (Tesrs + i);
    (pTesrs)[i] = Tesrs + i;
  }

  ut_print_message (0, 3, "Initializing...\n");
  net_transform_tesr_tesrmerge_init (string, pTesr, pTesrs, &CellIds,
                                     &CellStates, &CellQty, &strategy);

  if (!strcmp (strategy, "direct"))
  {
    ut_print_message (0, 3, "Merging (direct strategy)...\n");
    net_transform_tesr_tesrmerge_direct (Tesrs, CellIds, CellStates, CellQty, pTesr);
  }
  else if (!strcmp (strategy, "smart"))
  {
    ut_print_message (0, 3, "Merging (smart strategy)...\n");
    net_transform_tesr_tesrmerge_smart (Tesrs, CellIds, CellStates, CellQty, pTesr);
  }
  else
    ut_print_message (2, 3, "Unknown strategy `%s'.\n", strategy);

  for (i = 0; i < 2; i++)
    neut_tesr_free (Tesrs + i);
  free (pTesrs);

  ut_free_1d_int (&CellIds);
  ut_free_2d_int (&CellStates, CellQty);
  ut_free_1d_char (&strategy);

  return;
}
