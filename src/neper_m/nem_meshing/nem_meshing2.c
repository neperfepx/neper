/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_.h"

void
nem_meshing_post (struct TESS Tess, struct MESH *Mesh)
{
  int i, j;
  char *entity = NULL;

  ut_string_string (Tess.DomType, &(Mesh[Tess.Dim].Domain));

  // meshes of lower dimensions
  for (i = 0; i < Tess.Dim; i++)
    if (!neut_mesh_isvoid (Mesh[i]))
    {
      neut_tess_dim_entity (i, &entity);

      if (!Mesh[i].ElsetLabels)
        Mesh[i].ElsetLabels = ut_alloc_2d_char (Mesh[i].ElsetQty + 1, 20);
      for (j = 1; j <= Mesh[i].ElsetQty; j++)
        sprintf (Mesh[i].ElsetLabels[j], "%s%d", entity, j);
    }

  // mesh of upper dimension
  if (!neut_mesh_isvoid (Mesh[Tess.Dim]))
  {
    if (!Mesh[Tess.Dim].ElsetLabels)
      Mesh[Tess.Dim].ElsetLabels = ut_alloc_2d_char (Mesh[Tess.Dim].ElsetQty + 1, 20);

    neut_tess_dim_entity (Tess.Dim, &entity);

    for (i = 1; i <= Mesh[Tess.Dim].ElsetQty; i++)
      sprintf (Mesh[Tess.Dim].ElsetLabels[i], "%s%d", entity,
               Mesh[Tess.Dim].ElsetId ? Mesh[Tess.Dim].ElsetId[i] : i);
  }

  if (Tess.CellGroup && !neut_mesh_isvoid (Mesh[Tess.Dim]))
    neut_mesh_set_elsetgroup (Tess.CellGroup, Mesh);

  ut_free_1d_char (&entity);

  return;
}
