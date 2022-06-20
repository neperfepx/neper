/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_writemesh_.h"

void
nem_writemesh_msh (struct IN_M In, struct TESS Tess, struct NODES Nodes,
                   struct MESH *Mesh, struct NSET *NSet, char *nsetlist,
                   char *fasetlist, char *format)
{
  int i;
  FILE *file = NULL;

  for (i = 3; i >= 0; i--)
  {
    if (Tess.Dim == i
        && ut_list_testelt (In.dimout_msh, NEUT_SEP_NODEP, "3") == 1
        && !Mesh[3].EltType)
      ut_print_message (1, 3, "%dD mesh is void.\n", i);
  }

  char *mode = NULL, *version = NULL;
  if (!strcmp (format, "msh4"))
    ut_string_string ("msh4", &version);
  else
    ut_string_string ("msh", &version);

  if (!strcmp (format, "msh:ascii")
      || !strcmp (format, "msh")
      || !strcmp (format, "msh4"))
    ut_string_string ("ascii", &mode);
  else if (!strcmp (format, "msh:binary"))
    ut_string_string ("binary", &mode);

  file = ut_file_open (!strcmp (version, "msh") ? In.msh : In.msh4, "w");

  neut_mesh_fprintf_msh (file, In.dimout_msh, Tess, Nodes, Mesh[0],
                         Mesh[1], Mesh[2], Mesh[3], Mesh[4],
                         NSet[0], NSet[1], NSet[2], nsetlist,
                         fasetlist, NULL, version, mode);

  ut_file_close (file, !strcmp (version, "msh") ? In.msh : In.msh4, "w");

  ut_free_1d_char (&mode);
  ut_free_1d_char (&version);

  return;
}

void
nem_writemesh_sim (struct IN_M In, struct TESS Tess, struct NODES Nodes,
                   struct MESH *Mesh, struct NSET *NSet, char *nsetlist,
                   char *fasetlist)
{
  int i;
  FILE *file = NULL;
  struct SIM Sim;
  char *simmsh = ut_alloc_1d_char (1000);
  char *simtess = ut_alloc_1d_char (1000);

  neut_sim_set_zero (&Sim);
  neut_mesh_sim (In.sim, Tess, Nodes, Mesh, &Sim);

  sprintf (simmsh, "%s/inputs/simulation.msh", In.sim);
  sprintf (simtess, "%s/inputs/simulation.tess", In.sim);

  for (i = 3; i >= 0; i--)
  {
    if (Tess.Dim == i
        && ut_list_testelt (In.dimout_msh, NEUT_SEP_NODEP, "3") == 1
        && !Mesh[3].EltType)
      ut_print_message (1, 3, "%dD mesh is void.\n", i);
  }

  ut_dir_openmessage (In.sim, "w");

  file = ut_file_open (simtess, "W");
  neut_tess_fprintf (file, Tess);
  ut_file_close (file, simtess, "W");
  ut_string_string ("simulation.tess", &Sim.tess);

  file = ut_file_open (simmsh, "W");
  neut_mesh_fprintf_msh (file, In.dimout_msh, Tess, Nodes, Mesh[0],
                         Mesh[1], Mesh[2], Mesh[3], Mesh[4],
                         NSet[0], NSet[1], NSet[2], nsetlist,
                         fasetlist, NULL, "msh", "ascii");
  ut_file_close (file, simmsh, "W");

  neut_sim_fprintf (In.sim, Sim, "W");

  ut_dir_closemessage (In.sim, "w");

  neut_sim_free (&Sim);

  ut_free_1d_char (&simmsh);
  ut_free_1d_char (&simtess);

  return;
}
