/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_show_.h"

void
nev_show (char **argv, int *pi, struct TESS Tess, struct TESR Tesr,
          struct NODES Nodes, struct MESH *Mesh, struct POINT Point,
          struct PRINT *pPrint)
{
  struct PART Part;
  neut_part_set_zero (&Part);

  // general -----------------------------------------------------------

  if (!strcmp (argv[(*pi)], "-showcsys"))
    nev_show_csys (argv, pi, pPrint);

  if (!strcmp (argv[(*pi)], "-showshadow"))
    nev_show_shadow (argv, pi, pPrint);

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
      nev_show_tess_cell (argv, pi, Tess, pPrint);
    else if (Tesr.Dim > 0)
      nev_show_tesr_cell (argv, pi, Tesr, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showseed"))
  {
    if (Tess.Dim > 0)
      nev_show_tess_seed (argv, pi, Tess, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showver"))
  {
    if (Tess.Dim > 0)
      nev_show_tess_ver (argv, pi, Tess, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showedge"))
  {
    if (Tess.Dim >= 1)
      nev_show_tess_edge (argv, pi, Tess, pPrint);
    else if (Tesr.Dim == 1)
      nev_show_tesr_cell (argv, pi, Tesr, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showface"))
  {
    if (Tess.Dim >= 2)
      nev_show_tess_face (argv, pi, Tess, pPrint);
    else if (Tesr.Dim == 2)
      nev_show_tesr_cell (argv, pi, Tesr, pPrint);
    else
      ut_print_message (2, 0, "Cannot process `%s'.\n", argv[(*pi)]);
  }

  else if (!strcmp (argv[(*pi)], "-showpoly"))
  {
    if (Tess.Dim == 3)
      nev_show_tess_poly (argv, pi, Tess, pPrint);
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

  else if (!strcmp (argv[(*pi)], "-showpoint"))
    nev_show_points (argv, pi, Point, pPrint);

  neut_part_free (Part);

  return;
}
