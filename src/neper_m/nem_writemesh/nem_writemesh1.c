/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_writemesh_.h"

void
nem_writemesh (struct IN_M In, struct TESS Tess, struct NODES Nodes,
               struct MESH *Mesh, struct NSET *NSet,
               struct BOUNDARY Bound)
{
  int i, j, dim;
  double size;
  FILE *file = NULL;
  char *nsetlist = NULL;
  char *fasetlist = NULL;
  char **sizestring = ut_alloc_2d_char (4, 10);
  char **formats = NULL;
  int formatqty;

  ut_list_break (In.format, NEUT_SEP_NODEP, &formats, &formatqty);

  dim = neut_mesh_array_dim (Mesh);

  strcpy (sizestring[1], "length");
  strcpy (sizestring[2], "area");
  strcpy (sizestring[3], "volume");

  if (!strcmp (In.nset, "default"))
  {
    if (dim == 3)
      ut_string_string ("faces,edges,vertices", &(In.nset));
    else if (dim == 2)
      ut_string_string ("edges,vertices", &(In.nset));
  }

  neut_nset_expand (NSet[0], NSet[1], NSet[2], In.nset, &nsetlist);
  if (!strcmp (Mesh[dim].EltType, "tri"))
    neut_nset_expand (NSet[0], NSet[1], NSet[2], In.faset, &fasetlist);

  ut_print_message (0, 2, "Preparing mesh...\n");
  for (i = 3; i >= 0; i--)
    if (ut_list_testelt_int (In.dimout_msh, NEUT_SEP_NODEP, i)
        && !Mesh[i].EltElset)
      neut_mesh_init_eltelset (Mesh + i, NULL);

  ut_print_message (0, 2, "Mesh properties:\n");
  ut_print_message (0, 3, "Node number: %8d\n", Nodes.NodeQty);
  ut_print_message (0, 3, "Elt  number: %8d\n", Mesh[dim].EltQty);

  if (dim > 0)
  {
    neut_mesh_size (Nodes, Mesh[dim], &size);
    ut_print_message (0, 3, "Mesh %s: %8.3f\n", sizestring[dim], size);
  }

  if (strlen (In.format) > 0)
    ut_print_message (0, 2, "Writing mesh...\n");

  for (i = 0; i < formatqty; i++)
  {
    if (!strcmp (formats[i], "msh")
        || !strcmp (formats[i], "msh:ascii")
        || !strcmp (formats[i], "msh:binary")
        || !strcmp (formats[i], "msh4"))
      nem_writemesh_msh (In, Tess, Nodes, Mesh, NSet, nsetlist, fasetlist,
                         formats[i]);

    else if (!strcmp (formats[i], "sim"))
      nem_writemesh_sim (In, Tess, Nodes, Mesh, NSet, nsetlist, fasetlist);

    else if (!strcmp (formats[i], "ori"))
    {
      file = ut_file_open (In.ori, "w");
      neut_mesh_fprintf_ori (file, Mesh[dim]);
      ut_file_close (file, In.ori, "w");
    }

    else if (!strcmp (formats[i], "phase"))
    {
      file = ut_file_open (In.phase, "w");
      neut_mesh_fprintf_phase (file, Mesh[dim]);
      ut_file_close (file, In.phase, "w");
    }

    else if (!strcmp (formats[i], "bcs"))
    {
      file = ut_file_open (In.bcs, "w");
      neut_mesh_fprintf_bcs (file, NSet[0], NSet[1], NSet[2], nsetlist);
      ut_file_close (file, In.bcs, "w");
    }

    else if (!strcmp (formats[i], "vtk"))
    {
      for (j = 3; j >= 0; j--)
      {
        if (Tess.Dim == j
            && ut_list_testelt (In.dimout_msh, NEUT_SEP_NODEP, "3") == 1
            && !Mesh[3].EltType)
          ut_print_message (1, 3, "%dD mesh is void.\n", j);
      }

      file = ut_file_open (In.vtk, "w");
      neut_mesh_fprintf_vtk (file, In.dimout, Nodes, Mesh[1], Mesh[2], Mesh[3]);
      ut_file_close (file, In.vtk, "w");
    }

    else if (!strcmp (formats[i], "abq")
        || !strcmp (formats[i], "inp"))
    {
      file = ut_file_open (In.abq, "w");
      neut_mesh_fprintf_inp (file, In.dimout, Tess, Nodes, Mesh[0], Mesh[1],
                             Mesh[2], Mesh[3], Mesh[4], NSet[0], NSet[1],
                             NSet[2], nsetlist, fasetlist, Bound);
      ut_file_close (file, In.abq, "w");
    }

    else if (!strcmp (formats[i], "geof"))
    {
      ut_alloc_1d_int (Mesh[3].EltQty + 1);

      file = ut_file_open (In.geof, "w");
      neut_mesh_fprintf_geof (file, Nodes, Mesh[1], Mesh[2], Mesh[3], Mesh[4],
                              NSet[0], NSet[1], NSet[2], nsetlist,
                              fasetlist, In.dimout, Bound);
      ut_file_close (file, In.geof, "w");
    }

    if (Nodes.Periodic && ut_array_1d_int_sum (Nodes.Periodic, 3) > 0
      && !strcmp (formats[i], "per"))
    {
      file = ut_file_open (In.per, "w");
      if (!strcmp (In.performat, "msh"))
        neut_mesh_fprintf_per (file, Nodes);
      else if (!strcmp (In.performat, "plain"))
        neut_mesh_fprintf_per_plain (file, Nodes);
      else if (!strcmp (In.performat, "geof"))
        neut_mesh_fprintf_per_geof (file, Nodes);
      ut_file_close (file, In.per, "w");
    }
  }

  if (Bound.BoundQty > 0)
  {
    file = ut_file_open (In.intf, "w");
    for (j = 1; j <= Bound.BoundQty; j++)
      if (Bound.BoundDom[j][0] == -1 && Bound.BoundEltQty[j] > 0)
        fprintf (file, "bound%d-cell%d bound%d-cell%d\n", j,
                 Bound.BoundSeed[j][0], j, Bound.BoundSeed[j][1]);
    ut_file_close (file, In.intf, "w");
  }

  ut_free_1d_char (&nsetlist);
  ut_free_1d_char (&fasetlist);
  ut_free_2d_char (&sizestring, 4);
  ut_free_2d_char (&formats, formatqty);

  return;
}
