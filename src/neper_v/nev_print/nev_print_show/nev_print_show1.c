/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_show_.h"

void
nev_print_show (struct IN_V In, struct TESS *pTess, struct TESR Tesr,
          struct NODES Nodes, struct MESH *Mesh, struct POINT *Points,
          int PointQty, struct PRINT *pPrint)
{
  int i, j;

  for (i = 0; i < In.showqty; i++)
  {
    // general -----------------------------------------------------------

    if (!strcmp (In.show[i][0], "csys"))
      ut_string_int (In.show[i][1], &((*pPrint).showcsys));

    else if (!strcmp (In.show[i][0], "scale"))
      ut_string_int (In.show[i][1], &((*pPrint).showscale));

    else if (!strcmp (In.show[i][0], "tess"))
      ut_string_int (In.show[i][1], &((*pPrint).showtess));

    else if (!strcmp (In.show[i][0], "tesr"))
      ut_string_int (In.show[i][1], &((*pPrint).showtesr));

    else if (!strcmp (In.show[i][0], "mesh"))
      ut_string_int (In.show[i][1], &((*pPrint).showmesh));

    else if (!strcmp (In.show[i][0], "meshslice"))
      ut_string_int (In.show[i][1], &((*pPrint).showslice));

    else if (!strcmp (In.show[i][0], "meshsliceelt1d"))
      ut_string_int (In.show[i][1], &((*pPrint).showsliceelt1d));

    // tess and tesr specific --------------------------------------------

    else if (!strcmp (In.show[i][0], "cell"))
    {
      if ((*pTess).Dim > 0)
        nev_print_show_tess_entity ("cell", In.show[i][1], pTess, pPrint);
      else if (Tesr.Dim > 0)
        nev_print_show_tesr_cell (In.show[i][1], Tesr, pPrint);
      else
        ut_print_message (2, 0, "Cannot process `%s'.\n", In.show[i][0]);
    }

    else if (!strcmp (In.show[i][0], "seed"))
    {
      if ((*pTess).Dim > 0)
      nev_print_show_tess_entity ("seed", In.show[i][1], pTess, pPrint);
      else
        ut_print_message (2, 0, "Cannot process `%s'.\n", In.show[i][0]);
    }

    else if (!strcmp (In.show[i][0], "crystal"))
    {
      if ((*pTess).Dim > 0)
        nev_print_show_tess_entity ("crystal", In.show[i][1], pTess, pPrint);
      else
        ut_print_message (2, 0, "Cannot process `%s'.\n", In.show[i][0]);
    }

    else if (!strcmp (In.show[i][0], "ver"))
    {
      if ((*pTess).Dim > 0)
        nev_print_show_tess_entity ("ver", In.show[i][1], pTess, pPrint);
      else
        ut_print_message (2, 0, "Cannot process `%s'.\n", In.show[i][0]);
    }

    else if (!strcmp (In.show[i][0], "edge"))
    {
      if ((*pTess).Dim >= 1)
        nev_print_show_tess_entity ("edge", In.show[i][1], pTess, pPrint);
      else if (Tesr.Dim > 0)
        nev_print_show_tesr_edge (In.show[i][1], pPrint);
      else
        ut_print_message (2, 0, "Cannot process `%s'.\n", In.show[i][0]);
    }

    else if (!strcmp (In.show[i][0], "face"))
    {
      if ((*pTess).Dim >= 2)
        nev_print_show_tess_entity ("face", In.show[i][1], pTess, pPrint);
      else if (Tesr.Dim == 2)
        nev_print_show_tesr_cell (In.show[i][1], Tesr, pPrint);
      else
        ut_print_message (2, 0, "Cannot process `%s'.\n", In.show[i][0]);
    }

    else if (!strcmp (In.show[i][0], "poly"))
    {
      if ((*pTess).Dim == 3)
        nev_print_show_tess_entity ("poly", In.show[i][1], pTess, pPrint);
      else if (Tesr.Dim == 3)
        nev_print_show_tesr_cell (In.show[i][1], Tesr, pPrint);
      else
        ut_print_message (2, 0, "Cannot process `%s'.\n", In.show[i][0]);
    }

    else if (!strcmp (In.show[i][0], "vox")
          || !strcmp (In.show[i][0], "voidvox"))
    {
      if (!neut_tesr_isvoid (Tesr))
        nev_print_show_tesr_vox (In.show[i][0], In.show[i][1], Tesr, pPrint);
      else
        ut_print_message (2, 0, "Cannot process `%s'.\n", In.show[i][0]);
    }

    else if (!strcmp (In.show[i][0], "faceinter"))
    {
      if ((*pTess).Dim == 3)
        ut_string_int (In.show[i][1], &((*pPrint).showfaceinter));
      else
        ut_print_message (2, 0, "Cannot process `%s'.\n", In.show[i][0]);
    }

    // mesh --------------------------------------------------------------

    else if (!strncmp (In.show[i][0], "elt", 3))
      nev_print_show_mesh_elt (In.show[i][0], In.show[i][1], pTess, Nodes, Mesh, pPrint);

    else if (!strcmp (In.show[i][0], "node"))
      nev_print_show_mesh_nodes (In.show[i][1], pTess, Nodes, Mesh, pPrint);

    else if (!strncmp (In.show[i][0], "elset", 5))
      nev_print_show_mesh_elset (In.show[i][0], In.show[i][1], pTess, Nodes, Mesh, pPrint);

    // point -------------------------------------------------------------

    else
    {
      if (!(*pPrint).showpoint)
        (*pPrint).showpoint = ut_alloc_1d_pint (PointQty);

      int found = 0;
      for (j = 0; j < PointQty; j++)
        if (!strcmp (In.show[i][0], Points[j].Name))
        {
          nev_print_show_points (In.show[i][1], Points[j], (*pPrint).showpoint + j);
          found = 1;
        }

      if (!found)
        ut_print_exprbug (In.show[i][0]);
    }
  }

  return;
}
