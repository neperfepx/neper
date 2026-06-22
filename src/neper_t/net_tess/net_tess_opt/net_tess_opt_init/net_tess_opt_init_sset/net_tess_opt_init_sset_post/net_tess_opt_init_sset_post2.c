/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_post_.h"

void
net_tess_opt_init_sset_post_log (struct IN_T In, int level,
                                 struct TOPT *pTOpt)
{
  int i, j, seed;
  FILE *fp = NULL;
  char *filename = NULL;

  if (ut_list_testelt (In.optilogvar[0][level], NEUT_SEP_NODEP, "sset0"))
  {
    filename = ut_alloc_1d_char (strlen (In.body) + 10);
    sprintf (filename, "%s.sset0", In.body);
    fp = ut_file_open (filename, "W");

    fprintf (fp, "%d\n", (*pTOpt).CellQty);

    for (i = 1; i <= (*pTOpt).CellQty; i++)
    {
      fprintf (fp, "%d %d\n", i, (*pTOpt).CellSCellQty[i]);

      for (j = 0; j < (*pTOpt).CellSCellQty[i]; j++)
      {
        seed = (*pTOpt).CellSCellList[i][j];

        ut_array_1d_fprintf_nonl (fp, ((*pTOpt).SSet).SeedCoo[seed],
                                  ((*pTOpt).SSet).Dim, "%.12lf");
        fprintf (fp, " %.12lf\n", ((*pTOpt).SSet).SeedWeight[seed]);
      }
    }
    ut_file_close (fp, filename, "W");

    ut_print_message (0, 4, "Wrote initial sset to `%s'.\n", filename);

    ut_free_1d_char (&filename);
  }

  if (ut_list_testelt
      (In.optilogvar[0][level], NEUT_SEP_NODEP, "cellscell"))
  {
    filename = ut_alloc_1d_char (strlen (In.body) + 20);
    sprintf (filename, "%s.cellscell", In.body);
    fp = ut_file_open (filename, "W");

    for (i = 1; i <= (*pTOpt).CellQty; i++)
    {
      fprintf (fp, "%d %d\n", i, (*pTOpt).CellSCellQty[i]);

      for (j = 0; j < (*pTOpt).CellSCellQty[i]; j++)
        fprintf (fp, "%d ", (*pTOpt).CellSCellList[i][j]);

      fprintf (fp, "\n");
    }

    ut_file_close (fp, filename, "W");

    ut_print_message (0, 4, "Wrote cellscell to `%s'.\n", filename);

    ut_free_1d_char (&filename);
  }

  if (ut_list_testelt
      (In.optilogvar[0][level], NEUT_SEP_NODEP, "scellcell"))
  {
    filename = ut_alloc_1d_char (strlen (In.body) + 20);
    sprintf (filename, "%s.scellcell", In.body);
    fp = ut_file_open (filename, "W");

    for (i = 1; i <= ((*pTOpt).SSet).N; i++)
      fprintf (fp, "%d\n", (*pTOpt).SCellCell[i]);

    ut_file_close (fp, filename, "W");

    ut_print_message (0, 4, "Wrote scellcell to `%s'.\n", filename);

    ut_free_1d_char (&filename);
  }

  return;
}
