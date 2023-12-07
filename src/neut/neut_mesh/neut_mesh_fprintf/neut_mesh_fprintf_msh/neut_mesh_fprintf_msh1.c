/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_msh_.h"

void
neut_mesh_fprintf_msh (FILE * file, char *dim, struct TESS Tess,
                        struct NODES Nodes, struct MESH Mesh0D,
                        struct MESH Mesh1D, struct MESH Mesh2D,
                        struct MESH Mesh3D, struct MESH MeshCo,
                        struct NSET NSet0D, struct NSET NSet1D, struct NSET NSet2D,
                        char *nsetlist, char *fasetlist, char *numbering, char *version,
                        char *mode)
{
  int fasetqty, *fasetids = NULL;
  char **fasets = NULL;
  int *shift = ut_alloc_1d_int (5);

  neut_mesh_fprintf_msh_pre (Tess, fasetlist, &fasets, &fasetids, &fasetqty);

  neut_mesh_fprintf_msh_header (file, mode, version);

  if (Mesh3D.Domain)
    neut_mesh_fprintf_msh_domain (file, Mesh3D.Domain);

  if (!strcmp (version, "msh4"))
    neut_mesh_fprintf_msh_physical (file, Mesh0D, Mesh1D, Mesh2D, Mesh3D,
                                     MeshCo, fasets, fasetids, fasetqty, dim);

  if (!strcmp (version, "msh") || !strcmp (version, "msh2") || version[0] == '2')
  {
    neut_mesh_fprintf_msh_nodes (file, mode, Nodes);

    neut_mesh_fprintf_msh_elts (file, mode, Tess, Mesh0D, Mesh1D, Mesh2D,
                                 Mesh3D, MeshCo, fasets, fasetids, fasetqty,
                                 dim, numbering, shift);
  }

  else if (!strcmp (version, "msh4") || version[0] == '4')
  {
    neut_mesh_fprintf_msh_entities (file, mode, Tess, Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D);

    if (!ut_string_strcmp (Mesh2D.EltType, "quad")
     || !ut_string_strcmp (Mesh3D.EltType, "quad"))
      ut_print_message (2, 3, "`-format msh4` not available with `-elt quad|hex`.\n");

    neut_mesh_fprintf_msh_nodes_v4 (file, mode, Tess, Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D);

    neut_mesh_fprintf_msh_elts_v4 (file, mode, Tess, Mesh0D, Mesh1D, Mesh2D,
                                    Mesh3D, MeshCo, fasets, fasetids, fasetqty,
                                    dim, numbering, shift);
  }

  if (Nodes.PerNodeQty && !strcmp (mode, "ascii"))
    neut_mesh_fprintf_msh_periodicity (file, Nodes);

  if (((Tess.Dim == 2 && ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
    || (Tess.Dim == 3 && ut_list_testelt (dim, NEUT_SEP_NODEP, "3")))
    && nsetlist)
    neut_mesh_fprintf_msh_nsets (file, NSet0D, NSet1D, NSet2D, nsetlist);

  if (((Tess.Dim == 2 && ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
    || (Tess.Dim == 3 && ut_list_testelt (dim, NEUT_SEP_NODEP, "3")))
    && fasetlist && strlen (fasetlist))
    neut_mesh_fprintf_msh_fasets (file, Tess, Mesh2D, Mesh3D, shift, fasetlist);

  if (((Tess.Dim == 2 && ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
    || (Tess.Dim == 3 && ut_list_testelt (dim, NEUT_SEP_NODEP, "3")))
    && Nodes.PartQty > 0)
    neut_mesh_fprintf_msh_nodeparts (file, Nodes);

  if (strcmp (version, "msh4"))
    neut_mesh_fprintf_msh_physical (file, Mesh0D, Mesh1D, Mesh2D, Mesh3D,
                                     MeshCo, fasets, fasetids, fasetqty, dim);

  if ((Tess.Dim == 2 && ut_list_testelt (dim, NEUT_SEP_NODEP, "2")
       && (Mesh2D.ElsetOri || Mesh2D.EltOri))
   || (Tess.Dim == 3 && ut_list_testelt (dim, NEUT_SEP_NODEP, "3")
       && (Mesh3D.ElsetOri || Mesh3D.EltOri)))
    neut_mesh_fprintf_msh_orientations (file, Tess.Dim == 2 ? Mesh2D : Mesh3D);

  if ((Tess.Dim == 2 && ut_list_testelt (dim, NEUT_SEP_NODEP, "2") && Mesh2D.ElsetGroup)
   || (Tess.Dim == 3 && ut_list_testelt (dim, NEUT_SEP_NODEP, "3") && Mesh3D.ElsetGroup))
    neut_mesh_fprintf_msh_elsetgroups (file, Tess.Dim == 2 ? Mesh2D : Mesh3D);

  ut_free_1d_int (&fasetids);
  ut_free_2d_char (&fasets, fasetqty);
  ut_free_1d_int (&shift);

  return;
}
