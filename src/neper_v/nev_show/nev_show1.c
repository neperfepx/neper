/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_show_.h"

void
nev_show (char **argv, int *pi, struct TESS Tess, struct TESR Tesr,
          struct NODES Nodes, struct MESH *Mesh, struct POINT *Points,
          int PointQty, struct PRINT *pPrint)
{
  int i;
  char *input = NULL;

  // general -----------------------------------------------------------

  if (!strcmp (argv[(*pi)], "-showcsys"))
    nev_show_csys (argv, pi, pPrint);

  else if (!strcmp (argv[(*pi)], "-showscale"))
    nev_show_scale (argv, pi, pPrint);

  else if (!strcmp (argv[(*pi)], "-showtess"))
    nev_show_tess (argv, pi, pPrint);

  else if (!strcmp (argv[(*pi)], "-showtesr"))
    nev_show_tesr (argv, pi, pPrint);

  else if (!strcmp (argv[(*pi)], "-showmesh"))
    nev_show_mesh (argv, pi, pPrint);

  else if (!strcmp (argv[(*pi)], "-showmeshslice"))
    nev_show_mesh_slice (argv, pi, pPrint);

  // tess and tesr specific --------------------------------------------

  else if (!strcmp (argv[(*pi)], "-showcell"))
  {
    if (Tess.Dim > 0)
      nev_show_tess_entity (argv, pi, Tess, "cell", pPrint);
    else if (Tesr.Dim > 0)
      nev_show_tesr_cell (argv, pi, Tesr, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showseed"))
  {
    if (Tess.Dim > 0)
      nev_show_tess_entity (argv, pi, Tess, "seed", pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showcrystal"))
  {
    if (Tess.Dim > 0)
      nev_show_tess_entity (argv, pi, Tess, "crystal", pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showver"))
  {
    if (Tess.Dim > 0)
      nev_show_tess_entity (argv, pi, Tess, "ver", pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showedge"))
  {
    if (Tess.Dim >= 1)
      nev_show_tess_entity (argv, pi, Tess, "edge", pPrint);
    else if (Tesr.Dim > 0)
      nev_show_tesr_edge (argv, pi, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showface"))
  {
    if (Tess.Dim >= 2)
      nev_show_tess_entity (argv, pi, Tess, "face", pPrint);
    else if (Tesr.Dim == 2)
      nev_show_tesr_cell (argv, pi, Tesr, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showpoly"))
  {
    if (Tess.Dim == 3)
      nev_show_tess_entity (argv, pi, Tess, "poly", pPrint);
    else if (Tesr.Dim == 3)
      nev_show_tesr_cell (argv, pi, Tesr, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showvox")
        || !strcmp (argv[(*pi)], "-showvoidvox"))
  {
    if (!neut_tesr_isvoid (Tesr))
      nev_show_tesr_vox (argv[(*pi)], argv, pi, Tesr, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showfaceinter"))
  {
    if (Tess.Dim == 3)
      nev_show_tess_faceinter (argv, pi, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  // mesh --------------------------------------------------------------

  else if (!strncmp (argv[(*pi)], "-showelt", 8))
    nev_show_mesh_elt (argv, pi, Tess, Nodes, Mesh, pPrint);

  else if (!strcmp (argv[(*pi)], "-shownode"))
    nev_show_mesh_nodes (argv, pi, Tess, Nodes, Mesh, pPrint);

  else if (!strncmp (argv[(*pi)], "-showelset", 10))
    nev_show_mesh_elset (argv, pi, Tess, Nodes, Mesh, pPrint);

  // point -------------------------------------------------------------

  else if (!strncmp (argv[(*pi)], "-show", 4))
  {
    if (!(*pPrint).showpoint)
      (*pPrint).showpoint = ut_alloc_1d_pint (PointQty);

    int found = 0;
    ut_string_string (argv[(*pi)] + 5, &input);
    for (i = 0; i < PointQty; i++)
      if (!strcmp (input, Points[i].Name))
      {
        nev_show_points (argv, pi, Points[i], (*pPrint).showpoint + i);
        found = 1;
      }

    if (!found)
      ut_print_exprbug (argv[(*pi)]);
  }

  ut_free_1d_char (&input);

  return;
}
