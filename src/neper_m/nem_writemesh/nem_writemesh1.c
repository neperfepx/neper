/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_writemesh_.h"

void
nem_writemesh (struct IN_M In, struct TESS Tess,
	       struct NODES Nodes, struct MESH *Mesh,
	       struct NSET *NSet, struct PART Part,
	       struct BOUNDARY Bound)
{
  int i, dim;
  double size;
  FILE *file = NULL;
  char *expandnset = NULL;
  char *expandfaset = NULL;
  char **sizestring = ut_alloc_2d_char (4, 10);

  strcpy (sizestring[1], "length");
  strcpy (sizestring[2], "area");
  strcpy (sizestring[3], "volume");

  neut_nset_expand (NSet[0], NSet[1], NSet[2], In.nset, &expandnset);
  neut_nset_expand (NSet[0], NSet[1], NSet[2], In.faset, &expandfaset);

  ut_print_message (0, 2, "Preparing mesh...\n");
  for (i = 3; i >= 0; i--)
    if (ut_string_inlist_int (In.dimout_msh, NEUT_SEP_NODEP, i)
	&& !Mesh[i].EltElset)
      neut_mesh_init_eltelset (Mesh + i, NULL);

  dim = -1;
  for (i = 3; i >= 0; i--)
    if (Mesh[i].EltQty > 0)
    {
      dim = i;
      break;
    }

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

  if (ut_string_inlist (In.format, NEUT_SEP_NODEP, "msh")
   || ut_string_inlist (In.format, NEUT_SEP_NODEP, "msh:ascii")
   || ut_string_inlist (In.format, NEUT_SEP_NODEP, "msh:binary"))
  {
    for (i = 3; i >= 0; i--)
    {
      if (Tess.Dim == i
	  && ut_string_inlist (In.dimout_msh, NEUT_SEP_NODEP, "3") == 1
	  && !Mesh[3].EltType)
	ut_print_message (1, 3, "%dD mesh is void.\n", i);
    }

    char *format = NULL;
    if (ut_string_inlist (In.format, NEUT_SEP_NODEP, "msh:ascii")
     || ut_string_inlist (In.format, NEUT_SEP_NODEP, "msh"))
      ut_string_string ("ascii", &format);
    else if (ut_string_inlist (In.format, NEUT_SEP_NODEP, "msh:binary"))
      ut_string_string ("binary", &format);

    file = ut_file_open (In.msh, "w");
    neut_mesh_fprintf_gmsh (file, In.dimout_msh, Nodes, Mesh[0], Mesh[1],
			    Mesh[2], Mesh[3], Part, Mesh[4], NULL, format);
    ut_file_close (file, In.msh, "w");
    ut_free_1d_char (format);
  }

  if (ut_string_inlist (In.format, NEUT_SEP_NODEP, "vtk"))
  {
    for (i = 3; i >= 0; i--)
    {
      if (Tess.Dim == i
	  && ut_string_inlist (In.dimout_msh, NEUT_SEP_NODEP, "3") == 1
	  && !Mesh[3].EltType)
	ut_print_message (1, 3, "%dD mesh is void.\n", i);
    }

    file = ut_file_open (In.vtk, "w");
    neut_mesh_fprintf_vtk (file, In.dimout, Nodes, Mesh[1], Mesh[2], Mesh[3],
			   Part);
    ut_file_close (file, In.vtk, "w");
  }

  if (ut_string_inlist (In.format, NEUT_SEP_NODEP, "abq")
      || ut_string_inlist (In.format, NEUT_SEP_NODEP, "inp"))
  {
    file = ut_file_open (In.abq, "w");
    neut_mesh_fprintf_inp (file, In.dimout, Tess, Nodes, Mesh[1],
			   Mesh[2], Mesh[3], Mesh[4], NSet[0], NSet[1], NSet[2],
			   expandnset, expandfaset, Part, Bound);
    ut_file_close (file, In.abq, "w");
  }

  if (ut_string_inlist (In.format, NEUT_SEP_NODEP, "geof"))
  {
    ut_alloc_1d_int (Mesh[3].EltQty + 1);

    file = ut_file_open (In.geof, "w");
    neut_mesh_fprintf_geof (file, Nodes, Mesh[1], Mesh[2],
			    Mesh[3], NSet[0], NSet[1], NSet[2],
			    expandnset, expandfaset, In.dimout, Part, Bound);
    ut_file_close (file, In.geof, "w");
  }

  if (ut_string_inlist (In.format, NEUT_SEP_NODEP, "fepx")
      || ut_string_inlist (In.format, NEUT_SEP_NODEP, "fepx:legacy"))
  {
    char *version = NULL;
    if (ut_string_inlist (In.format, NEUT_SEP_NODEP, "fepx:legacy"))
      ut_string_string ("legacy", &version);

    neut_mesh_fprintf_fepx_name (In.body, Tess, Nodes,
				 Mesh[2], Mesh[3], NSet[0], NSet[1], NSet[2],
				 expandnset, expandfaset, version);

    ut_free_1d_char (version);
  }

  if (Nodes.Periodic && ut_array_1d_int_sum (Nodes.Periodic, 3) > 0)
  {
    file = ut_file_open (In.per, "w");
    if (!strcmp (In.performat, "plain"))
      neut_mesh_fprintf_per_plain (file, Nodes);
    else if (!strcmp (In.performat, "geof"))
      neut_mesh_fprintf_per_geof (file, Nodes);
    ut_file_close (file, In.per, "w");
  }

  if (Bound.BoundQty > 0)
  {
    file = ut_file_open (In.intf, "w");
    for (i = 1; i <= Bound.BoundQty; i++)
      if (Bound.BoundDom[i][0] == -1 && Bound.BoundEltQty[i] > 0)
	fprintf (file, "bound%d-cell%d bound%d-cell%d\n",
	         i, Bound.BoundSeed[i][0], i, Bound.BoundSeed[i][1]);
    ut_file_close (file, In.intf, "w");
  }

  ut_free_1d_char (expandnset);
  ut_free_1d_char (expandfaset);
  ut_free_2d_char (sizestring, 4);

  return;
}
