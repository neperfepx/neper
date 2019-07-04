/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_multiseed_.h"

void
net_tess_opt_init_sset_multiseed (struct IN_T In, int level, struct TOPT TOpt, char *var,
				  double *rad, int *qty)
{
  int i, varqty = 4;
  char **vars = ut_alloc_2d_char (varqty, 10);
  double *vals = ut_alloc_1d (varqty);
  strcpy (vars[0], "avradeq");
  strcpy (vars[1], "avdiameq");
  strcpy (vars[2], "radeq");
  strcpy (vars[3], "diameq");

  if (ut_string_isdigits (In.morphooptimultiseed[level]))
  {
    sscanf (In.morphooptimultiseed[level], "%d", qty);
    ut_array_1d_int_set (qty + 1, TOpt.CellQty, qty[0]);
  }
  else if (ut_string_filename (In.morphooptimultiseed[level]))
    ut_array_1d_int_fscanfn (In.morphooptimultiseed[level],
			     qty + 1, TOpt.CellQty);

  else
  {
    neut_tess_cellavradeq (TOpt.Dom, TOpt.CellQty, vals);
    vals[1] = 2 * vals[0];

    for (i = 1; i <= TOpt.CellQty; i++)
    {
      if (!strcmp (var, "tesr"))
	neut_tesr_expr_val_int_one (TOpt.tartesr, "cell", i,
				    In.morphooptimultiseed[level], qty + i, NULL);

      else
      {
	vals[2] = rad[i];
	vals[3] = 2 * rad[i];
	ut_math_eval_int (In.morphooptimultiseed[level], varqty, vars, vals,
			  qty + i);
      }

      qty[i] = ut_num_max (qty[i], 1);
    }
  }

  /*
  ut_print_message (0, 4, "%d seeds, min/max per cell: %d/%d.\n",
		    ut_array_1d_int_sum (qty + 1, TOpt.CellQty),
		    ut_array_1d_int_min (qty + 1, TOpt.CellQty),
		    ut_array_1d_int_max (qty + 1, TOpt.CellQty));
		    */

  ut_free_1d (vals);
  ut_free_2d_char (vars, varqty);

  return;
}
