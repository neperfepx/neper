/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_.h"

void
nem_meshing_gen (struct TESS Tess, struct NODES *pNodes)
{
  if (Tess.Periodic)
  {
    (*pNodes).Periodic = ut_alloc_1d_int (3);
    ut_array_1d_int_memcpy (Tess.Periodic, 3, (*pNodes).Periodic);
    (*pNodes).PeriodicDist = ut_alloc_1d (3);
    ut_array_1d_memcpy (Tess.PeriodicDist, 3, (*pNodes).PeriodicDist);
  }

  return;
}

void
nem_meshing_post (struct TESS Tess, struct MESH *Mesh)
{
  int i, j;
  char *entity = NULL;

  // meshes of lower dimensions
  for (i = 0; i < Tess.Dim; i++)
  {
    neut_tess_dim_entity (i, &entity);

    Mesh[i].ElsetLabels = ut_alloc_2d_char (Mesh[i].ElsetQty + 1, 20);
    for (j = 1; j <= Mesh[i].ElsetQty; j++)
      sprintf (Mesh[i].ElsetLabels[j], "%s%d", entity, j);
  }

  // mesh of upper dimension
  Mesh[Tess.Dim].ElsetLabels = ut_alloc_2d_char (Tess.CellQty + 1, 20);

  neut_tess_dim_entity (Tess.Dim, &entity);

  for (i = 1; i <= Tess.CellQty; i++)
    sprintf (Mesh[Tess.Dim].ElsetLabels[i], "%s%d", entity,
             Mesh[Tess.Dim].ElsetId ? Mesh[Tess.Dim].ElsetId[i] : i);

  ut_free_1d_char (&entity);

  return;
}
