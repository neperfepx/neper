/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_gmsh_.h"

void
neut_mesh_fscanf_gmshHead (FILE * msh, int *pcontiguous)
{
  int status;
  char string[1000];

  /* Reading of the 2 first strings, which must be "$MeshFormat" and "2". */
  if (fscanf (msh, "%s", string) != 1 || strcmp (string, "$MeshFormat") != 0)
  {
    ut_print_message (2, 0, "Input file is not a msh file!\n");
    ut_print_message (2, 0, "(start word does not match.)\n");
    abort ();
  }

  if (fscanf (msh, "%s", string) != 1
      || (strcmp (string, "2") != 0 && strcmp (string, "2.2") != 0))
  {
    ut_print_message (2, 0, "Bad msh file format version detected!\n");
    abort ();
  }

  /* skipping the 2 next ones. */
  ut_file_skip (msh, 2);

  (*pcontiguous) = 0;

  status = fscanf (msh, "%s", string);

  if (status != 1)
    abort ();

  if (strcmp (string, "$Comments") == 0)
  {
    do
    {
      if (fscanf (msh, "%s", string) != 1)
	abort ();

      if (strcmp (string, "contiguous") == 0)
	(*pcontiguous) = 1;
    }
    while (strcmp (string, "$EndComments") != 0);

    do
    {
      if (fscanf (msh, "%s", string) != 1)
	abort ();
    }
    while (strcmp (string, "$EndMeshFormat") != 0);
  }
  else if (strcmp (string, "$EndMeshFormat") == 0)
    return;
  else
    abort ();

  return;
}

void
ReadNodes (FILE * msh, struct NODES *pNodes, int *node_nbs)
{
  ReadNodesProp (msh, pNodes, node_nbs);

  ReadNodesFoot (msh);

  return;
}
