/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_elset_.h"

void
nes_pproc_elset (struct IN_S In, struct SIM Sim, struct MESH Mesh)
{
  int i, status, qty;
  int *elset = NULL;

  ut_print_message (0, 2, "Extracting elsets...\n");

  if (strcmp (In.elsetextract, "all") == 0)
  {
    qty = Mesh.ElsetQty;
    elset = ut_alloc_1d_int (qty);
    ut_array_1d_int_set_id (elset, qty);
    ut_array_1d_int_addval (elset, qty, 1, elset);
  }
  else
  {
    char **list = NULL;
    ut_list_break (In.elsetextract, NEUT_SEP_NODEP, &list, &qty);
    elset = ut_alloc_1d_int (qty);

    for (i = 0; i < qty; i++)
    {
      status = sscanf (list[i], "%d", &(elset[i]));
      if (status != 1)
        ut_print_neperbug ();
    }

    ut_free_2d_char (&list, qty);
  }

  ut_sys_mkdir ("%s/elsets", In.simdir);

  for (i = 0; i < qty; i++)
    if (neut_mesh_elset_valid (Mesh, elset[i]))
    {
      ut_print_message (0, 3, "elset %d...\n", elset[i]);

      nes_pproc_elset_init_output (In, elset[i]);

      nes_pproc_elset_extract (In, Sim, Mesh, elset[i]);
    }
    else
      ut_print_message (1, 3, "elset %d... skipping (not valid)\n", elset[i],
                        Mesh.ElsetQty);

  ut_free_1d_int (&elset);

  return;
}
