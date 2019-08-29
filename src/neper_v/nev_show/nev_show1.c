/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
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

  else if (!strcmp (argv[(*pi)], "-showvox"))
  {
    if (Tesr.Dim == 3)
      nev_show_tesr_vox (argv, pi, Tesr, pPrint);
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
    nev_show_mesh_elt (argv, pi, Tess, Nodes, Mesh, Part, pPrint);

  else if (!strcmp (argv[(*pi)], "-shownode"))
    nev_show_mesh_nodes (argv, pi, Tess, Nodes, Mesh, Part, pPrint);

  else if (!strncmp (argv[(*pi)], "-showelset", 10))
    nev_show_mesh_elset (argv, pi, Tess, Nodes, Mesh, Part, pPrint);

  // point -------------------------------------------------------------

  else if (!strcmp (argv[(*pi)], "-showpoint"))
    nev_show_points (argv, pi, Point, pPrint);

  neut_part_free (Part);

  return;
}

void
nev_show_init (struct TESS Tess, struct TESR Tesr,
	       struct NODES Nodes, struct MESH *Mesh,
	       int SQty, struct POINT Point, struct PRINT *pPrint)
{
  int dim = neut_mesh_array_dim (Mesh);
  int i, j, cellqty;
  int *cells = NULL;

  if (SQty > 0)
  {
    if ((*pPrint).showslice == -1)
      (*pPrint).showslice = 1;
    if ((*pPrint).showmesh == -1)
      (*pPrint).showmesh = 0;
    if ((*pPrint).showtess == -1)
      (*pPrint).showtess = 0;
    if ((*pPrint).showtesr == -1)
      (*pPrint).showtesr = 0;
  }

  if (dim >= 0 && (*pPrint).showmesh == -1)
  {
    (*pPrint).showmesh = 1;
    if ((*pPrint).showtess == -1)
      (*pPrint).showtess = 0;
    if ((*pPrint).showtesr == -1)
      (*pPrint).showtesr = 0;
  }

  if (Tess.VerQty > 0 && (*pPrint).showtess == -1)
    (*pPrint).showtess = 1;

  if (Tesr.CellQty > 0 && (*pPrint).showtesr == -1)
    (*pPrint).showtesr = 1;

  if (Point.PointQty > 0)
    if ((*pPrint).showpoint[0] == -1)
    {
      (*pPrint).showpoint =
	ut_realloc_1d_int ((*pPrint).showpoint, Point.PointQty + 1);
      ut_array_1d_int_set ((*pPrint).showpoint + 1, Point.PointQty, 1);
      (*pPrint).showpoint[0] = Point.PointQty;
    }

  if ((*pPrint).showmesh == 1)
  {
    if ((*pPrint).showelt3d[0] == -1)
    {
      (*pPrint).showelt3d =
	ut_realloc_1d_int ((*pPrint).showelt3d, Mesh[3].EltQty + 1);

      (*pPrint).showelt3d[0] = (dim == 3) ? Mesh[3].EltQty : 0;
      ut_array_1d_int_set ((*pPrint).showelt3d + 1, Mesh[3].EltQty, dim == 3);
    }

    if ((*pPrint).showelt2d[0] == -1)
    {
      (*pPrint).showelt2d =
	ut_realloc_1d_int ((*pPrint).showelt2d, Mesh[2].EltQty + 1);

      (*pPrint).showelt2d[0] = (dim == 2) ? Mesh[2].EltQty : 0;
      ut_array_1d_int_set ((*pPrint).showelt2d + 1, Mesh[2].EltQty, dim == 2);
    }

    if ((*pPrint).showelt1d[0] == -1)
    {
      (*pPrint).showelt1d =
	ut_realloc_1d_int ((*pPrint).showelt1d, Mesh[1].EltQty + 1);

      (*pPrint).showelt1d[0] = (dim == 1) ? Mesh[1].EltQty : 0;
      ut_array_1d_int_set ((*pPrint).showelt1d + 1, Mesh[1].EltQty, dim == 1);
    }

    if ((*pPrint).showelt0d[0] == -1)
    {
      (*pPrint).showelt0d =
	ut_realloc_1d_int ((*pPrint).showelt0d, Mesh[0].EltQty + 1);

      (*pPrint).showelt0d[0] = (dim == 0) ? Mesh[0].EltQty : 0;
      ut_array_1d_int_set ((*pPrint).showelt0d + 1, Mesh[0].EltQty, dim == 0);
    }

    if ((*pPrint).shownode[0] == -1)
    {
      (*pPrint).shownode =
	ut_realloc_1d_int ((*pPrint).shownode, Nodes.NodeQty + 1);

      (*pPrint).shownode[0] = 0;
      ut_array_1d_int_set ((*pPrint).shownode + 1, Nodes.NodeQty, 0);
    }
  }

  if ((*pPrint).showtess == 1)
  {
    if (Tess.Dim == 3 && (*pPrint).showpoly[0] == -1)
    {
      (*pPrint).showpoly =
	ut_realloc_1d_int ((*pPrint).showpoly, Tess.PolyQty + 1);
      (*pPrint).showpoly[0] = Tess.PolyQty;
      ut_array_1d_int_set ((*pPrint).showpoly + 1, Tess.PolyQty, 1);
    }

    if ((*pPrint).showface[0] == -1)
    {
      (*pPrint).showface =
	ut_realloc_1d_int ((*pPrint).showface, Tess.FaceQty + 1);
      if (Tess.Dim == 2)
      {
	(*pPrint).showface[0] = Tess.FaceQty;
	ut_array_1d_int_set ((*pPrint).showface + 1, Tess.FaceQty, 1);
      }
      else
      {
	(*pPrint).showface[0] = 0;
	ut_array_1d_int_set ((*pPrint).showface + 1, Tess.FaceQty, 0);
      }
    }

    if ((*pPrint).showedge[0] == -1)
    {
      (*pPrint).showedge =
	ut_realloc_1d_int ((*pPrint).showedge, Tess.EdgeQty + 1);
      ut_array_1d_int_set ((*pPrint).showedge, Tess.EdgeQty + 1, 0);

      for (i = 1; i <= Tess.EdgeQty; i++)
      {
	neut_tess_edge_cells (Tess, i, &cells, &cellqty);
	for (j = 0; j < cellqty; j++)
	  if ((Tess.Dim == 3 && (*pPrint).showpoly[cells[j]] == 1)
	      || (Tess.Dim == 2 && (*pPrint).showface[cells[j]] == 1))
	  {
	    (*pPrint).showedge[i] = 1;
	    break;
	  }
	ut_free_1d_int (cells);
	cells = NULL;
      }
    }

    if ((*pPrint).showver[0] == -1)
    {
      (*pPrint).showver =
	ut_realloc_1d_int ((*pPrint).showver, Tess.VerQty + 1);
      ut_array_1d_int_set ((*pPrint).showver, Tess.VerQty + 1, 0);
    }

    if ((*pPrint).showseed[0] == -1)
    {
      (*pPrint).showseed =
	ut_realloc_1d_int ((*pPrint).showseed, Tess.SeedQty + 1);
      ut_array_1d_int_set ((*pPrint).showseed, Tess.SeedQty + 1, 0);
    }
  }

  else if ((*pPrint).showtesr == 1)
  {
    if (Tesr.Dim == 1)
    {
      (*pPrint).showedge =
	ut_realloc_1d_int ((*pPrint).showedge, Tesr.CellQty + 1);
      ut_array_1d_int_set ((*pPrint).showedge + 1, Tess.EdgeQty, 0);

      if ((*pPrint).showedge[0] == -1)
      {
	(*pPrint).showedge[0] = Tesr.CellQty;
	ut_array_1d_int_set ((*pPrint).showedge + 1, Tesr.CellQty, 1);
      }
    }
    else if (Tesr.Dim == 2)
    {
      (*pPrint).showface =
	ut_realloc_1d_int ((*pPrint).showface, Tesr.CellQty + 1);
      ut_array_1d_int_set ((*pPrint).showface + 1, Tess.FaceQty, 0);

      if ((*pPrint).showface[0] == -1)
      {
	(*pPrint).showface[0] = Tesr.CellQty;
	ut_array_1d_int_set ((*pPrint).showface + 1, Tesr.CellQty, 1);
      }
    }
    else if (Tesr.Dim == 3)
    {
      (*pPrint).showpoly =
	ut_realloc_1d_int ((*pPrint).showpoly, Tesr.CellQty + 1);
      ut_array_1d_int_set ((*pPrint).showpoly + 1, Tess.PolyQty, 0);

      if ((*pPrint).showpoly[0] == -1)
      {
	(*pPrint).showpoly[0] = Tesr.CellQty;
	ut_array_1d_int_set ((*pPrint).showpoly + 1, Tesr.CellQty, 1);
      }
    }
  }

  if ((*pPrint).showshadow == -1)
  {
    if ((*pPrint).showmesh == 1)
      dim = dim;
    else if ((*pPrint).showtess == 1)
      dim = Tess.Dim;
    else if ((*pPrint).showtesr == 1)
      dim = Tesr.Dim;
    else
      dim = 3;

    (*pPrint).showshadow = (dim == 3) ? 1 : 0;
  }

  return;
}
