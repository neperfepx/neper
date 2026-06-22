/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_.h"

void
net_tess_opt_init_bounds_inputvals (struct TOPT *pTOpt, char **dofs, int dofqty, char *bound, double **bound_input)
{
  int i, j, boundqty, dofid, partqty;
  char **bounds = NULL;
  char **parts = NULL;

  ut_list_break (bound, NEUT_SEP_NODEP, &bounds, &boundqty);

  for (i = 0; i < boundqty; i++)
  {
    ut_list_break (bounds[i], NEUT_SEP_DEP, &parts, &partqty);
    if (partqty != 2)
      ut_print_exprbug (bound);

    dofid = -1;
    for (j = 0; j < dofqty; j++)
      if (!strcmp (parts[0], dofs[j]))
      {
        dofid = j;
        break;
      }

    if (dofid != -1)
      ut_array_1d_fnscanf_wcard (parts[1], bound_input[dofid], (*pTOpt).seedoptiqty, (char *) "numeral", (char *) "r");

    ut_free_2d_char (&parts, partqty);

    if (!strcmp (dofs[dofid], "rt"))
      ut_array_1d_scale (bound_input[dofid], (*pTOpt).seedoptiqty, M_PI / 180);
  }

  ut_free_2d_char (&bounds, boundqty);
  ut_free_2d_char (&parts, partqty);

  return;
}
