/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_load_.h"

void
net_tess_opt_init_sset_load_cell (struct TOPT *pTOpt, struct SEEDSET SSet,
				  int cell, int SeedQty, FILE * fp)
{
  (void) (pTOpt);
  (void) (SSet);
  (void) (cell);
  (void) (SeedQty);
  (void) (fp);

  printf ("FIX ME\n");

  /*
     int i, res;

     if (cell < 0 || SeedQty <= 0)
     ut_print_message (2, 0,
     "Failed to load seedset! (cell %d with %d seeds)\n",
     cell, SeedQty);

     (*pTOpt).CellSSeedCoo[cell] = ut_alloc_2d (SeedQty, 3);
     (*pTOpt).CellSSeedWei[cell] = ut_alloc_1d (SeedQty);

     for (i = 0; i < SeedQty; i++)
     {
     res = ut_array_1d_fscanf (fp, (*pTOpt).CellSSeedCoo[cell][i], SSet.Dim);
     res += fscanf (fp, "%lf", (*pTOpt).CellSSeedWei[cell] + i);

     if (res != 2)
     ut_print_message (2, 0, "Failed to load seedset! (cell %d)\n", cell);
     }

     (*pTOpt).CellSCellQty[cell] = SeedQty;
   */

  return;
}
