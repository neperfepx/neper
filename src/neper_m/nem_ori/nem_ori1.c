/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_ori_.h"

void
nem_ori (struct TESS Tess, struct MESH *Mesh)
{
  int i, j, dim = neut_mesh_array_dim (Mesh);

  if (Tess.CellQty != Mesh[dim].ElsetQty)
    ut_print_neperbug ();

  if (Tess.CellOri)
  {
    ut_string_string (Tess.CellOriDes, &(Mesh[dim].ElsetOriDes));
    Mesh[dim].ElsetOri = ut_alloc_2d (Mesh[dim].ElsetQty + 1, 4);
    ut_array_2d_memcpy (Tess.CellOri + 1, Tess.CellQty, 4, Mesh[dim].ElsetOri + 1);
  }

  if (Tess.CellOriDistrib)
  {
    Mesh[dim].EltOri = ut_alloc_2d (Mesh[dim].EltQty + 1, 4);

    struct OL_SET OSet;

    for (i = 1; i <= Mesh[dim].ElsetQty; i++)
    {
      OSet = ol_set_alloc (Mesh[dim].Elsets[i][0], Tess.CellCrySym);

      ol_set_misoridistrib (Tess.CellOriDistrib[i], i, &OSet);

      ut_string_string (Mesh[dim].ElsetOriDes, &Mesh[dim].EltOriDes);
      for (j = 1; j <= Mesh[dim].Elsets[i][0]; j++)
        ol_q_q_q_ref (Tess.CellOri[i], OSet.q[j - 1], Mesh[dim].EltOri[Mesh[dim].Elsets[i][j]]);

      ol_set_free (OSet);
    }
  }

  return;
}
