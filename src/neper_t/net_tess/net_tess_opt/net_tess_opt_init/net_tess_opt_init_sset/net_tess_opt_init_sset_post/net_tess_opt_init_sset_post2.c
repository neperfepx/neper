/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_post_.h"

void
net_tess_opt_init_sset_post_per (int *periodic, int level, struct TESS Dom,
                                 int poly, struct SEEDSET *pSSet)
{
  int i, j, k, l;
  int master;
  int *shift = NULL;
  double **bbox = NULL;

  if (level > 0)
  {
    (*pSSet).Nall = (*pSSet).N;
    return;
  }

  shift = ut_alloc_1d_int (3);
  bbox = ut_alloc_2d (3, 2);

  if (poly != 1)
    ut_print_neperbug ();
  neut_tess_bbox (Dom, bbox);

  if (ut_array_1d_int_sum (periodic, 3) > 0)
    ut_string_string ("periodic", &(*pSSet).Type);
  else
    ut_string_string ("standard", &(*pSSet).Type);

  (*pSSet).Periodic = ut_alloc_1d_int (3);
  ut_array_1d_int_memcpy (periodic, 3, (*pSSet).Periodic);

  (*pSSet).PeriodicDist = ut_alloc_1d (3);
  for (i = 0; i < 3; i++)
    if ((*pSSet).Periodic[i])
      (*pSSet).PeriodicDist[i] = bbox[i][1] - bbox[i][0];

  (*pSSet).Nall = (*pSSet).N;
  (*pSSet).PerSeedMaster = ut_alloc_1d_int ((*pSSet).Nall + 1);
  (*pSSet).PerSeedShift = ut_alloc_2d_int ((*pSSet).Nall + 1, 3);

  for (k = -(*pSSet).Periodic[2]; k <= (*pSSet).Periodic[2]; k++)
    for (j = -(*pSSet).Periodic[1]; j <= (*pSSet).Periodic[1]; j++)
      for (i = -(*pSSet).Periodic[0]; i <= (*pSSet).Periodic[0]; i++)
      {
        if (k == 0 && j == 0 && i == 0)
          continue;

        ut_array_1d_int_set_3 (shift, i, j, k);

        for (l = 1; l <= (*pSSet).N; l++)
        {
          master = l;
          neut_seedset_slave_add (pSSet, master, shift);
        }
      }

  neut_seedset_seedcootoseedcoo0 (pSSet);
  neut_seedset_init_seedslave (pSSet);

  ut_free_1d_int (&shift);
  ut_free_2d (&bbox, 3);

  return;
}

void
net_tess_opt_init_sset_post_log (struct IN_T In, int level,
                                 struct TOPT *pTOpt)
{
  int i, j, seed;
  FILE *fp = NULL;
  char *filename = NULL;

  if (ut_list_testelt (In.optilogvar[level], NEUT_SEP_NODEP, "sset0"))
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
      (In.optilogvar[level], NEUT_SEP_NODEP, "cellscell"))
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
      (In.optilogvar[level], NEUT_SEP_NODEP, "scellcell"))
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
