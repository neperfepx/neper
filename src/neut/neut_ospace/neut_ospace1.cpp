/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_ospace_.h"
#include "neut/neut_oset/neut_oset.hpp"

void
neut_ospace_set_zero (struct OSPACE *pOSpace)
{
  int i;

  (*pOSpace).fr = NULL;
  (*pOSpace).space = NULL;
  (*pOSpace).spaceunit = NULL;
  (*pOSpace).crysym = NULL;

  ut_string_string ("rodrigues(fr20)", &(*pOSpace).fr);

  neut_tess_set_zero (&((*pOSpace).Tess));
  neut_nodes_set_zero (&((*pOSpace).Nodes));
  (*pOSpace).Mesh = (struct MESH*) calloc (4, sizeof (struct MESH));
  for (i = 0; i < 4; i++)
    neut_mesh_set_zero ((*pOSpace).Mesh + i);

  (*pOSpace).pqcloud = NULL;
  (*pOSpace).pqtree = NULL;
  (*pOSpace).pqcloudn = NULL;
  (*pOSpace).pqtreen = NULL;

  ut_fct_set_zero (&((*pOSpace).hfct));
  ol_homochoric_thetafct (&((*pOSpace).hfct));

  ut_fct_set_zero (&((*pOSpace).thetafct));
  ol_spreadfct (&((*pOSpace).thetafct));

  return;
}

void
neut_ospace_fnscanf (char *crysym, char *space, char *fr, struct OSPACE *pOSpace, char *mode)
{
  QCLOUD *pqcloud;
  my_kd_tree_t **pqtree = NULL;

  if (strcmp (space, "rodrigues"))
    ut_print_message (2, 2, "Unknown space `%s'.\n", space);

  neut_ospace_set_zero (pOSpace);
  ut_string_string (crysym, &(*pOSpace).crysym);
  ut_string_string (space, &(*pOSpace).space);
  ut_string_string (fr, &(*pOSpace).fr);

  // keeping track of the prerecorded pqcloud and pqtree
  pqcloud = (QCLOUD *) (*pOSpace).pqcloud;
  pqtree = (my_kd_tree_t **) (*pOSpace).pqtree;

  if (pqcloud)
    (*pOSpace).pqcloud = pqcloud;
  if (pqtree)
    (*pOSpace).pqtree = pqtree;

  neut_ospace_fnscanf_tess (crysym, space, pOSpace, mode);
  neut_ospace_fnscanf_mesh (crysym, space, fr, pOSpace, mode);

  neut_mesh_init_eltweight ((*pOSpace).Nodes, (*pOSpace).Mesh + 3);
  neut_mesh_init_eltori ((*pOSpace).Nodes, (*pOSpace).Mesh + 3);
  neut_nodes_init_nodeweight (&(*pOSpace).Nodes, (*pOSpace).Mesh + 3);
  neut_nodes_init_nodeori (&(*pOSpace).Nodes);

  return;
}

void
neut_ospace_mesh_olset (struct OSPACE *pOSp, struct OL_SET *pOSet)
{
  double *coo = ut_alloc_1d (3);

  if (!(*pOSp).Mesh[3].EltWeight)
    neut_mesh_init_eltweight ((*pOSp).Nodes, (*pOSp).Mesh + 3);
  if (!(*pOSp).Mesh[3].EltOri)
    neut_mesh_init_eltori ((*pOSp).Nodes, (*pOSp).Mesh + 3);

  (*pOSet) = ol_set_alloc ((*pOSp).Mesh[3].EltQty, (*pOSp).crysym);

  ut_array_1d_memcpy ((*pOSp).Mesh[3].EltWeight + 1, (*pOSp).Mesh[3].EltQty, (*pOSet).weight);
  ut_array_2d_memcpy ((*pOSp).Mesh[3].EltOri + 1, (*pOSp).Mesh[3].EltQty, 4, (*pOSet).q);

  ut_free_1d (&coo);

  return;
}

void
neut_ospace_mesh_olset_allnodes (struct OSPACE *pOSp, struct OL_SET *pOSet)
{
  double *coo = ut_alloc_1d (3);

  if (!(*pOSp).Nodes.NodeWeight)
    neut_nodes_init_nodeweight (&(*pOSp).Nodes, (*pOSp).Mesh + 3);
  if (!(*pOSp).Nodes.NodeOri)
    neut_nodes_init_nodeori (&(*pOSp).Nodes);

  (*pOSet) = ol_set_alloc ((*pOSp).Nodes.NodeQty, (*pOSp).crysym);

  ut_array_1d_memcpy ((*pOSp).Nodes.NodeWeight + 1, (*pOSp).Nodes.NodeQty, (*pOSet).weight);
  ut_array_2d_memcpy ((*pOSp).Nodes.NodeOri + 1, (*pOSp).Nodes.NodeQty, 4, (*pOSet).q);

  ut_free_1d (&coo);

  return;
}

void
neut_orispace_free (struct OSPACE *pOSpace)
{
  int i;

  ut_free_1d_char (&(*pOSpace).fr);
  ut_free_1d_char (&(*pOSpace).space);
  ut_free_1d_char (&(*pOSpace).spaceunit);
  ut_free_1d_char (&(*pOSpace).crysym);

  neut_tess_free (&(*pOSpace).Tess);
  neut_nodes_free (&(*pOSpace).Nodes);
  for (i = 0; i <= 3; i++)
    neut_mesh_free ((*pOSpace).Mesh + i);

  ut_fct_free (&((*pOSpace).hfct));
  ut_fct_free (&((*pOSpace).thetafct));

  return;
}
