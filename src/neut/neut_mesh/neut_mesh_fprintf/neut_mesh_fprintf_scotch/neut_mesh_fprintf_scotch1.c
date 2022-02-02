/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_scotch_.h"

void
neut_mesh_fprintf_scotch (char *body, struct NODES Nodes, struct MESH Mesh)
{
#ifdef HAVE_LIBSCOTCH
  FILE *file = NULL;
  SCOTCH_Mesh SCMesh;
  char *scm, *scg;

  scm = ut_string_addextension (body, ".scm");
  scg = ut_string_addextension (body, ".scg");

  SCOTCH_meshInit (&SCMesh);

  neut_mesh_scotchmesh (Mesh, Nodes.NodeQty, &SCMesh);

  file = ut_file_open (scm, "w");
  SCOTCH_meshSave (&SCMesh, file);
  ut_file_close (file, scm, "w");

  file = ut_file_open (scg, "w");
  neut_nodes_fprintf_scotch (file, Nodes);
  ut_file_close (file, scg, "w");

  ut_free_1d_char (&scm);
  ut_free_1d_char (&scg);

#else
  (void) body;
  (void) Nodes;
  (void) Mesh;
  ut_print_message (2, 0, "Compiled without libscotch.\n");
#endif /* HAVE_LIBSCOTCH */

  return;
}

void
neut_nodes_fprintf_scotch (FILE * out, struct NODES Nodes)
{
  int i, j;

  fprintf (out, "3 %d\n", Nodes.NodeQty);
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (out, "%d", i);
    for (j = 0; j < 3; j++)
      fprintf (out, "  %.12f", Nodes.NodeCoo[i][j]);
    fprintf (out, "\n");
  }

  return;
}
