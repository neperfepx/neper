/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_tree_.h"
#include "neut/neut_oset/neut_oset.hpp"
#include "neut_struct_qcloud.hpp"
extern void neut_odf_comp_tree (struct ODF *pOdf, char *entity, struct QCLOUD *pqcloud, my_kd_tree_t ** pqtree);

void
neut_mesh_fprintf_tree (FILE *file, char *entity, struct NODES Nodes, struct MESH Mesh)
{
  struct ODF Odf;
  struct OL_SET Grid;

  QCLOUD qcloud;
  QCLOUD *pqcloud = &qcloud;
  my_kd_tree_t *qtree = NULL;
  my_kd_tree_t **pqtree = &qtree;
  nanoflann::SearchParams params;

  neut_odf_set_zero (&Odf);

  ut_string_string (Mesh.Domain, &(Odf.Sp.space));

  neut_nodes_memcpy (Nodes, &(Odf.Sp.Nodes));
  neut_mesh_memcpy (Mesh, Odf.Sp.Mesh + 3);

  neut_mesh_init_nodeelts (Odf.Sp.Mesh + 3, Nodes.NodeQty);
  neut_mesh_init_eltweight (Nodes, Odf.Sp.Mesh + 3);
  neut_nodes_init_nodeweight (&(Odf.Sp.Nodes), Odf.Sp.Mesh + 3);

  if (!strncmp (entity, "elt", 3))
    neut_ospace_mesh_olset (&(Odf.Sp), &Grid);
  else if (!strncmp (entity, "node", 4))
    neut_ospace_mesh_olset_allnodes (&(Odf.Sp), &Grid);

  neut_oset_kdtree (&Grid, pqcloud, pqtree);

  (*pqtree)->saveIndex(file);

  // neut_odf_free (&Odf);
  ol_set_free (&Grid);

  return;
}
