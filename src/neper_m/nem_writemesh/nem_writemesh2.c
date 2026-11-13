/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
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

  char *mode = NULL;

  if (!strcmp (format, "msh:ascii") || !strcmp (format, "msh"))
    ut_string_string ("ascii", &mode);
  else if (!strcmp (format, "msh:binary"))
    ut_string_string ("binary", &mode);

  file = ut_file_open (In.msh, "w");

  neut_mesh_fprintf_msh (file, In.dimout_msh, Tess, Nodes, Mesh[0],
                         Mesh[1], Mesh[2], Mesh[3], Mesh[4], Mesh[5],
                         NSet[0], NSet[1], NSet[2], nsetlist,
                         fasetlist, NULL, "msh", mode);

  ut_file_close (file, In.msh, "w");

  ut_free_1d_char (&mode);

  return;
}

void
nem_writemesh_sim (struct IN_M In, struct TESS Tess, struct NODES Nodes,
                   struct MESH *Mesh, struct NSET *NSet, char *nsetlist,
                   char *fasetlist)
{
  int i;
  FILE *file = NULL;
  char *body = NULL, *simmsh = NULL, *simtess = NULL;

  ut_file_removepath (In.body, &body);
  ut_string_sprintf (&simmsh, "%s/inputs/%s.msh", In.sim, body);
  ut_string_sprintf (&simtess, "%s/inputs/%s.tess", In.sim, body);

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

  file = ut_file_open (simmsh, "W");
  neut_mesh_fprintf_msh (file, In.dimout_msh, Tess, Nodes, Mesh[0],
                         Mesh[1], Mesh[2], Mesh[3], Mesh[4], Mesh[5],
                         NSet[0], NSet[1], NSet[2], nsetlist,
                         fasetlist, NULL, "msh", "ascii");
  ut_file_close (file, simmsh, "W");

  ut_dir_closemessage (In.sim, "w");

  ut_free_1d_char (&body);
  ut_free_1d_char (&simmsh);
  ut_free_1d_char (&simtess);

  return;
}

// Expanding the list of fasets: "faces" stands for all fasets of the faset
// mesh (which also applies to a mesh loaded with its fasets), the other
// names are expanded as for nsets (if no faset mesh).  A faset is listed
// once.
void
nem_writemesh_fasetlist (struct IN_M In, struct MESH MeshFasets,
                         struct NSET *NSet, char **pfasetlist)
{
  int i, j, qty;
  char **list = NULL, *fasetlist = NULL;

  if (MeshFasets.ElsetQty == 0)
    neut_nset_expand (NSet[0], NSet[1], NSet[2], In.faset, &fasetlist);
  else
    ut_string_string (In.faset, &fasetlist);

  ut_list_break (fasetlist, NEUT_SEP_NODEP, &list, &qty);

  ut_free_1d_char (pfasetlist);

  for (i = 0; i < qty; i++)
    if (MeshFasets.ElsetQty > 0 && !strcmp (list[i], "faces"))
    {
      // (empty fasets, e.g. the faces of a periodic domain, are skipped)
      for (j = 1; j <= MeshFasets.ElsetQty; j++)
        if (MeshFasets.Elsets[j][0] > 0)
          nem_writemesh_fasetlist_add (pfasetlist, MeshFasets.ElsetLabels[j]);
    }
    else
      nem_writemesh_fasetlist_add (pfasetlist, list[i]);

  if (!(*pfasetlist))
    (*pfasetlist) = ut_alloc_1d_char (1);

  ut_free_2d_char (&list, qty);
  ut_free_1d_char (&fasetlist);

  return;
}

// Adding a faset to a list, if not already in it
void
nem_writemesh_fasetlist_add (char **pfasetlist, char *faset)
{
  if (!(*pfasetlist)
      || !ut_list_testelt (*pfasetlist, NEUT_SEP_NODEP, faset))
    ut_list_addelt (pfasetlist, NEUT_SEP_NODEP, faset);

  return;
}
