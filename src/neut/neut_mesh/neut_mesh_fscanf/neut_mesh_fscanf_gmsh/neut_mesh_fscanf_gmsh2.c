/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_gmsh_.h"

void
neut_mesh_fscanf_gmshHead (FILE * msh, int *pcontiguous, char **pmode)
{
  int status, type;
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
    ut_print_message (2, 0, "Bad msh file format version detected!\n");

  /* skipping the 2 next ones. */
  status = fscanf (msh, "%d", &type);
  if (status != 1)
    abort ();

  if (type == 0)
    ut_string_string ("ascii", pmode);
  else if (type == 1)
    ut_string_string ("binary", pmode);
  else
    abort ();

  ut_file_skip (msh, 1);

  // trashing 1 value
  if (!strcmp (*pmode, "binary"))
  {
    status = fread (&type, sizeof (int), 1, msh);
    if (status != 1)
      abort ();
    status = fscanf (msh, "%s", string);
    if (status != 1)
      abort ();
  }

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
  else if (strcmp (string, "$EndMeshFormat"))
    abort ();

  ut_file_nextstring (msh, string);
  if (!strcmp (string, "$PhysicalNames"))
    do
      status = fscanf (msh, "%s", string);
    while (status == 1 && strcmp (string, "$EndPhysicalNames"));

  return;
}

void
ReadNodes (FILE * msh, char *mode, struct NODES *pNodes, int *node_nbs)
{
  ReadNodesProp (msh, mode, pNodes, node_nbs);

  ReadNodesFoot (msh, mode);

  return;
}
