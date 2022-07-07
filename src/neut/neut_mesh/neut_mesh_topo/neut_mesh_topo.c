/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_topo_.h"

void
neut_mesh_elts_comnodes (struct MESH Mesh, int *elts, int EltQty,
                         int **pnodes, int *pnodeqty)
{
  int i;
  int *tmp1 = NULL;
  int *tmp2 = NULL;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  tmp1 = ut_alloc_1d_int (eltnodeqty);
  tmp2 = ut_alloc_1d_int (eltnodeqty);

  ut_array_1d_int_memcpy (Mesh.EltNodes[elts[0]], eltnodeqty, tmp1);
  (*pnodeqty) = eltnodeqty;

  for (i = 1; i < EltQty; i++)
  {
    ut_array_1d_int_inter (tmp1, *pnodeqty, Mesh.EltNodes[elts[i]],
                           eltnodeqty, tmp2, pnodeqty);
    ut_array_1d_int_memcpy (tmp2, *pnodeqty, tmp1);
  }

  if (pnodes)
  {
    ut_free_1d_int (pnodes);
    (*pnodes) = ut_alloc_1d_int (*pnodeqty);
    ut_array_1d_int_memcpy (tmp1, *pnodeqty, *pnodes);
  }

  ut_free_1d_int (&tmp1);
  ut_free_1d_int (&tmp2);

  return;
}

void
neut_mesh_eltpair_comnodes (struct MESH Mesh, int elt1, int elt2,
                            int **pnodes, int *pnodeqty)
{
  int *elts = ut_alloc_1d_int (2);

  elts[0] = elt1;
  elts[1] = elt2;

  neut_mesh_elts_comnodes (Mesh, elts, 2, pnodes, pnodeqty);

  ut_free_1d_int (&elts);

  return;
}

void
neut_mesh_elts_allnodes (struct MESH Mesh, int *elts, int EltQty,
                         int **pnodes, int *pnodeqty)
{
  int i;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  ut_free_1d_int (pnodes);

  (*pnodeqty) = EltQty * eltnodeqty;
  (*pnodes) = ut_realloc_1d_int (*pnodes, *pnodeqty);

  for (i = 0; i < EltQty; i++)
    ut_array_1d_int_memcpy (Mesh.EltNodes[elts[i]], eltnodeqty,
                            (*pnodes) + i * eltnodeqty);

  ut_array_1d_int_sort_uniq (*pnodes, *pnodeqty, pnodeqty);

  return;
}

void
neut_mesh_elt_nodes_ori (struct MESH Mesh, int elt, int *nodes, int *pori)
{
  int pos1, pos2;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, 1);

  pos1 = ut_array_1d_int_eltpos (Mesh.EltNodes[elt], eltnodeqty, nodes[0]);
  pos2 = ut_array_1d_int_eltpos (Mesh.EltNodes[elt], eltnodeqty, nodes[1]);

  if (ut_array_rotpos (0, eltnodeqty - 1, pos1, 1) == pos2)
    (*pori) = 1;
  else if (ut_array_rotpos (0, eltnodeqty - 1, pos1, -1) == pos2)
    (*pori) = -1;
  else
    (*pori) = 0;

  return;
}

void
neut_mesh_nodes_comelts (struct MESH Mesh, int *nodes, int NodeQty,
                         int **pelts, int *peltqty)
{
  int i;
  int *tmp1 = NULL;
  int *tmp2 = NULL;
  int tmp2qty = -1;

  if (Mesh.NodeElts == NULL)
  {
    ut_print_message (2, 0, "neut_mesh_nodes_comelts requires NodeElts\n");
    ut_print_neperbug ();
  }

  if (NodeQty == 1)
  {
    (*peltqty) = Mesh.NodeElts[nodes[0]][0];
    (*pelts) = ut_alloc_1d_int (*peltqty);
    ut_array_1d_int_memcpy (Mesh.NodeElts[nodes[0]] + 1, *peltqty, *pelts);
    return;
  }

  for (i = 0; i < NodeQty; i++)
    if (nodes[i] > Mesh.NodeQty)
    {
      (*peltqty) = 0;
      return;
    }

  tmp1 = ut_alloc_1d_int (Mesh.NodeElts[nodes[0]][0] + 1);
  tmp2 = ut_alloc_1d_int (Mesh.NodeElts[nodes[0]][0] + 1);

  ut_array_1d_int_memcpy (Mesh.NodeElts[nodes[0]] + 1,
                          Mesh.NodeElts[nodes[0]][0], tmp1);

  (*peltqty) = Mesh.NodeElts[nodes[0]][0];

  for (i = 1; i < NodeQty; i++)
  {
    ut_array_1d_int_inter (tmp1, *peltqty, Mesh.NodeElts[nodes[i]] + 1,
                           Mesh.NodeElts[nodes[i]][0], tmp2, &tmp2qty);

    ut_array_1d_int_memcpy (tmp2, tmp2qty, tmp1);
    (*peltqty) = tmp2qty;

    /*
       printf ("node %d elt: ", nodes[i]);
       ut_array_1d_int_fprintf (stdout, tmp, tmpqty, "%d");
       printf ("> elt: ");
       ut_array_1d_int_fprintf (stdout, elts, *peltqty, "%d");
     */
  }

  (*pelts) = ut_realloc_1d_int (*pelts, tmp2qty);
  ut_array_1d_int_memcpy (tmp2, tmp2qty, *pelts);
  ut_array_1d_int_sort (*pelts, *peltqty);

  ut_free_1d_int (&tmp1);
  ut_free_1d_int (&tmp2);

  return;
}

void
neut_mesh_nodes_onecomelt (struct MESH Mesh, int *nodes, int NodeQty,
                           int *pelt)
{
  int *elts = NULL, EltQty;

  neut_mesh_nodes_comelts (Mesh, nodes, NodeQty, &elts, &EltQty);
  (*pelt) = (EltQty > 0) ? elts[0] : -1;

  ut_free_1d_int (&elts);

  return;
}

void
neut_mesh_nodes_allelts (struct MESH Mesh, int *nodes, int NodeQty,
                         int **pelts, int *peltqty)
{
  int i, j, node;

  (*peltqty) = 0;
  for (i = 0; i < NodeQty; i++)
  {
    node = nodes[i];
    for (j = 1; j <= Mesh.NodeElts[node][0]; j++)
      if (ut_array_1d_int_eltpos (*pelts, *peltqty, Mesh.NodeElts[node][j]) ==
          -1)
      {
        (*pelts) = ut_realloc_1d_int (*pelts, (*peltqty) + 1);
        (*pelts)[(*peltqty)++] = Mesh.NodeElts[node][j];
      }
  }

  ut_array_1d_int_sort ((*pelts), *peltqty);

  return;
}

int
neut_mesh_nodes_comelt (struct MESH Mesh, int *nodes, int NodeQty, int *pelt)
{
  int *tmp = NULL;
  int tmpqty;

  neut_mesh_nodes_comelts (Mesh, nodes, NodeQty, &tmp, &tmpqty);

  if (tmpqty == 1)
    (*pelt) = tmp[0];
  else
    (*pelt) = -1;

  ut_free_1d_int (&tmp);

  return ut_num_min (*pelt, 0);
}

int
neut_mesh_nodes_elsets_comelts (struct MESH Mesh, int *nodes, int NodeQty,
                                int *elsets, int elsetqty, int **pelts,
                                int *peltqty)
{
  int i, j;
  int *tmp = NULL;
  int tmpqty;

  ut_free_1d_int (pelts);

  if (Mesh.EltElset == NULL)
  {
    printf ("neut_mesh_nodes_elsets_comelts needs EltElset\n");
    ut_print_neperbug ();
  }

  neut_mesh_nodes_comelts (Mesh, nodes, NodeQty, &tmp, &tmpqty);

  (*peltqty) = 0;
  for (i = 0; i < tmpqty; i++)
    for (j = 0; j < elsetqty; j++)
      if (ut_array_1d_int_eltpos (elsets, elsetqty, Mesh.EltElset[tmp[i]]) !=
          -1)
      {
        (*pelts) = ut_realloc_1d_int (*pelts, (*peltqty) + 1);
        (*pelts)[(*peltqty)++] = tmp[i];
        break;
      }

  ut_free_1d_int (&tmp);

  return 0;
}

void
neut_mesh_elset_nodes (struct MESH Mesh, int id, int **pnodes, int *pqty)
{
  int i, elt, qty, eltnodeqty;
  int *tmp;

  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  tmp = ut_alloc_1d_int (Mesh.Elsets[id][0] * eltnodeqty);

  qty = 0;
  for (i = 1; i <= Mesh.Elsets[id][0]; i++)
  {
    elt = Mesh.Elsets[id][i];

    ut_array_1d_int_memcpy (Mesh.EltNodes[elt], eltnodeqty, tmp + qty);
    qty += eltnodeqty;
  }

  ut_array_1d_int_sort_uniq (tmp, qty, &qty);

  if (pqty)
    (*pqty) = qty;
  if (pnodes)
  {
    (*pnodes) = ut_alloc_1d_int (qty);
    ut_array_1d_int_memcpy (tmp, qty, *pnodes);
  }

  ut_free_1d_int (&tmp);

  return;
}

int
neut_mesh_elt0d_elts1d (struct MESH Mesh0D, int elt0d, struct MESH Mesh1D,
                        int **pelt1d, int *pelt1dqty)
{
  int node = Mesh0D.EltNodes[elt0d][0];

  if (Mesh1D.EltQty == 0)
  {
    (*pelt1dqty) = 0;
    return 0;
  }

  (*pelt1dqty) = Mesh1D.NodeElts[node][0];
  (*pelt1d) = ut_alloc_1d_int (*pelt1dqty);

  ut_array_1d_int_memcpy (Mesh1D.NodeElts[node] + 1, *pelt1dqty, *pelt1d);

  return *pelt1dqty;
}

int
neut_mesh_elt0d_elsets1d (struct MESH Mesh0D, int elt0d, struct MESH Mesh1D,
                          int **pelset1d, int *pelset1dqty)
{
  int *elt1d = NULL;
  int elt1dqty;

  if (Mesh1D.EltQty == 0)
  {
    (*pelset1dqty) = 0;
    return 0;
  }

  ut_free_1d_int (pelset1d);

  neut_mesh_elt0d_elts1d (Mesh0D, elt0d, Mesh1D, &elt1d, &elt1dqty);

  // is on the domain surface
  if (elt1dqty == 1)
  {
    (*pelset1d) = ut_alloc_1d_int (2);
    (*pelset1d)[0] = Mesh1D.EltElset[elt1d[0]];
    (*pelset1d)[1] = -1;
    (*pelset1dqty) = 2;
  }
  // may or may not be on an interface
  else
    neut_mesh_elts_elsets (Mesh1D, elt1d, elt1dqty, pelset1d, pelset1dqty);

  ut_free_1d_int (&elt1d);

  return *pelset1dqty;
}

int
neut_mesh_elt1d_elts2d (struct MESH Mesh1D, int elt1d, struct MESH Mesh2D,
                        int **pelt2d, int *pelt2dqty)
{
  int NodeQty = neut_elt_nodeqty (Mesh1D.EltType, 1, Mesh1D.EltOrder);

  if (Mesh2D.EltQty == 0)
  {
    (*pelt2dqty) = 0;
    return 0;
  }

  ut_free_1d_int (pelt2d);

  if (Mesh2D.Dimension != 2)
    abort ();

  neut_mesh_nodes_comelts (Mesh2D, Mesh1D.EltNodes[elt1d], NodeQty, pelt2d,
                           pelt2dqty);

  return *pelt2dqty;
}

int
neut_mesh_elt1d_elsets2d (struct MESH Mesh1D, int elt1d, struct MESH Mesh2D,
                          int **pelset2d, int *pelset2dqty)
{
  int *elt2d = NULL;
  int elt2dqty;

  if (Mesh2D.EltQty == 0)
  {
    (*pelset2dqty) = 0;
    return 0;
  }

  ut_free_1d_int (pelset2d);

  neut_mesh_elt1d_elts2d (Mesh1D, elt1d, Mesh2D, &elt2d, &elt2dqty);

  // RQ 18/06/2013: removed Mesh2D.ElsetQty == 1 && condition
  if (elt2dqty == 1)
  {
    (*pelset2d) = ut_alloc_1d_int (2);
    (*pelset2dqty) = 2;
    (*pelset2d)[0] = Mesh2D.EltElset[elt2d[0]];
    (*pelset2d)[1] = -1;
  }
  else
    neut_mesh_elts_elsets (Mesh2D, elt2d, elt2dqty, pelset2d, pelset2dqty);

  ut_free_1d_int (&elt2d);

  return *pelset2dqty;
}

int
neut_mesh_elt1d_elts3d (struct MESH Mesh1D, int elt1d, struct MESH Mesh2D,
                        struct MESH Mesh3D, int **pelt3d, int *pelt3dqty)
{
  int i, j, qty, elt2dqty;
  int *elt2d = NULL;
  int *tmp = NULL;

  if (Mesh3D.EltQty == 0)
  {
    (*pelt3dqty) = 0;
    return 0;
  }

  ut_free_1d_int (pelt3d);

  neut_mesh_elt1d_elts2d (Mesh1D, elt1d, Mesh2D, &elt2d, &elt2dqty);

  (*pelt3dqty) = 0;
  for (i = 0; i < elt2dqty; i++)
  {
    neut_mesh_elt2d_elts3d (Mesh2D, elt2d[i], Mesh3D, &tmp, &qty);
    for (j = 0; j < qty; j++)
      if (ut_array_1d_int_eltpos (*pelt3d, (*pelt3dqty), tmp[j]) == -1)
      {
        (*pelt3d) = ut_realloc_1d_int (*pelt3d, (*pelt3dqty) + 1);
        (*pelt3d)[(*pelt3dqty)++] = tmp[j];
      }
  }

  ut_free_1d_int (&elt2d);
  ut_free_1d_int (&tmp);

  return *pelt3dqty;
}

int
neut_mesh_elt0d_elts3d (struct MESH Mesh0D, int elt0d, struct MESH Mesh1D,
                        struct MESH Mesh2D, struct MESH Mesh3D, int **pelt3d,
                        int *pelt3dqty)
{
  int i, j, qty, elt1dqty;
  int *elt1d = NULL;
  int *tmp = NULL;

  if (Mesh3D.EltQty == 0)
  {
    (*pelt3dqty) = 0;
    return 0;
  }

  neut_mesh_elt0d_elts1d (Mesh0D, elt0d, Mesh1D, &elt1d, &elt1dqty);

  (*pelt3dqty) = 0;
  ut_free_1d_int (pelt3d);

  for (i = 0; i < elt1dqty; i++)
  {
    neut_mesh_elt1d_elts3d (Mesh1D, elt1d[i], Mesh2D, Mesh3D, &tmp, &qty);
    for (j = 0; j < qty; j++)
      if (ut_array_1d_int_eltpos ((*pelt3d), (*pelt3dqty), tmp[j]) == -1)
      {
        (*pelt3d) = ut_realloc_1d_int (*pelt3d, (*pelt3dqty) + 1);
        (*pelt3d)[(*pelt3dqty)++] = tmp[j];
      }
  }

  ut_free_1d_int (&elt1d);
  ut_free_1d_int (&tmp);

  return *pelt3dqty;
}

int
neut_mesh_elt0d_elts2d (struct MESH Mesh0D, int elt0d, struct MESH Mesh1D,
                        struct MESH Mesh2D, int **pelt2d, int *pelt2dqty)
{
  int i, j, qty, elt1dqty;
  int *elt1d = NULL;
  int *tmp = NULL;

  if (Mesh2D.EltQty == 0)
  {
    (*pelt2dqty) = 0;
    return 0;
  }

  neut_mesh_elt0d_elts1d (Mesh0D, elt0d, Mesh1D, &elt1d, &elt1dqty);

  (*pelt2dqty) = 0;
  ut_free_1d_int (pelt2d);

  for (i = 0; i < elt1dqty; i++)
  {
    neut_mesh_elt1d_elts2d (Mesh1D, elt1d[i], Mesh2D, &tmp, &qty);
    for (j = 0; j < qty; j++)
      if (ut_array_1d_int_eltpos ((*pelt2d), (*pelt2dqty), tmp[j]) == -1)
      {
        (*pelt2d) = ut_realloc_1d_int (*pelt2d, (*pelt2dqty) + 1);
        (*pelt2d)[(*pelt2dqty)++] = tmp[j];
      }
  }

  ut_free_1d_int (&elt1d);
  ut_free_1d_int (&tmp);

  return *pelt2dqty;
}

void
neut_mesh_elt_boundmesh (struct NODES Nodes, struct MESH Mesh, int elt,
                         struct MESH *pBMesh)
{
  int i;
  int *othernode = NULL;
  int *tab = Mesh.EltNodes[elt];        // do not free; this is a shortcut
  double *eq = ut_alloc_1d (4);

  (*pBMesh).Dimension = Mesh.Dimension - 1;
  (*pBMesh).EltOrder = Mesh.EltOrder;
  (*pBMesh).EltType =
    ut_realloc_1d_char ((*pBMesh).EltType, strlen (Mesh.EltType) + 1);
  strcpy ((*pBMesh).EltType, Mesh.EltType);

  if (Mesh.Dimension == 1)
  {
    (*pBMesh).EltQty = 2;
    (*pBMesh).ElsetQty = 0;

    if ((*pBMesh).EltNodes == NULL)
      (*pBMesh).EltNodes = ut_alloc_2d_int (3, 1);

    (*pBMesh).EltNodes[1][0] = tab[0];
    (*pBMesh).EltNodes[2][0] = tab[1];
  }

  else if (Mesh.Dimension == 2)
  {
    if (strcmp (Mesh.EltType, "tri") == 0)
      (*pBMesh).EltQty = 3;
    else if (strcmp (Mesh.EltType, "quad") == 0)
      (*pBMesh).EltQty = 4;
    else
      abort ();

    (*pBMesh).ElsetQty = 0;

    if ((*pBMesh).EltNodes == NULL)
    {
      if ((*pBMesh).EltOrder == 1)
        (*pBMesh).EltNodes = ut_alloc_2d_int ((*pBMesh).EltQty + 1, 2);
      else if ((*pBMesh).EltOrder == 2)
        (*pBMesh).EltNodes = ut_alloc_2d_int ((*pBMesh).EltQty + 1, 3);
      else
        abort ();
    }

    if (strcmp (Mesh.EltType, "tri") == 0)
    {
      (*pBMesh).EltNodes[1][0] = tab[0];
      (*pBMesh).EltNodes[1][1] = tab[1];

      (*pBMesh).EltNodes[2][0] = tab[1];
      (*pBMesh).EltNodes[2][1] = tab[2];

      (*pBMesh).EltNodes[3][0] = tab[2];
      (*pBMesh).EltNodes[3][1] = tab[0];

      if (Mesh.EltOrder == 2)
      {
        (*pBMesh).EltNodes[1][2] = tab[3];
        (*pBMesh).EltNodes[2][2] = tab[4];
        (*pBMesh).EltNodes[3][2] = tab[5];
      }
    }
    else if (strcmp (Mesh.EltType, "quad") == 0)
    {
      (*pBMesh).EltNodes[1][0] = tab[0];
      (*pBMesh).EltNodes[1][1] = tab[1];

      (*pBMesh).EltNodes[2][0] = tab[1];
      (*pBMesh).EltNodes[2][1] = tab[2];

      (*pBMesh).EltNodes[3][0] = tab[2];
      (*pBMesh).EltNodes[3][1] = tab[3];

      (*pBMesh).EltNodes[4][0] = tab[3];
      (*pBMesh).EltNodes[4][1] = tab[0];

      if (Mesh.EltOrder == 2)
      {
        (*pBMesh).EltNodes[1][2] = tab[4];
        (*pBMesh).EltNodes[2][2] = tab[5];
        (*pBMesh).EltNodes[3][2] = tab[6];
        (*pBMesh).EltNodes[4][2] = tab[7];
      }
    }
    else
      abort ();
  }

  else if (Mesh.Dimension == 3)
  {
    (*pBMesh).ElsetQty = 0;

    if (strcmp ((*pBMesh).EltType, "tri") == 0)
    {
      (*pBMesh).EltQty = 4;
      othernode = ut_alloc_1d_int ((*pBMesh).EltQty + 1);

      if ((*pBMesh).EltNodes == NULL)
      {
        if ((*pBMesh).EltOrder == 1)
          (*pBMesh).EltNodes = ut_alloc_2d_int ((*pBMesh).EltQty + 1, 3);
        else if ((*pBMesh).EltOrder == 2)
          (*pBMesh).EltNodes = ut_alloc_2d_int ((*pBMesh).EltQty + 1, 6);
        else
          abort ();
      }

      (*pBMesh).EltNodes[1][0] = tab[0];
      (*pBMesh).EltNodes[1][1] = tab[1];
      (*pBMesh).EltNodes[1][2] = tab[2];
      othernode[1] = tab[3];

      (*pBMesh).EltNodes[2][0] = tab[1];
      (*pBMesh).EltNodes[2][1] = tab[2];
      (*pBMesh).EltNodes[2][2] = tab[3];
      othernode[2] = tab[0];

      (*pBMesh).EltNodes[3][0] = tab[2];
      (*pBMesh).EltNodes[3][1] = tab[3];
      (*pBMesh).EltNodes[3][2] = tab[0];
      othernode[3] = tab[1];

      (*pBMesh).EltNodes[4][0] = tab[3];
      (*pBMesh).EltNodes[4][1] = tab[0];
      (*pBMesh).EltNodes[4][2] = tab[1];
      othernode[4] = tab[2];

      if (Mesh.EltOrder == 2)
      {
        (*pBMesh).EltNodes[1][3] = tab[4];
        (*pBMesh).EltNodes[1][4] = tab[5];
        (*pBMesh).EltNodes[1][5] = tab[6];

        (*pBMesh).EltNodes[2][3] = tab[5];
        (*pBMesh).EltNodes[2][4] = tab[8];
        (*pBMesh).EltNodes[2][5] = tab[9];

        (*pBMesh).EltNodes[3][3] = tab[8];
        (*pBMesh).EltNodes[3][4] = tab[7];
        (*pBMesh).EltNodes[3][5] = tab[6];

        (*pBMesh).EltNodes[4][3] = tab[7];
        (*pBMesh).EltNodes[4][4] = tab[4];
        (*pBMesh).EltNodes[4][5] = tab[9];
      }
    }
    else if (strcmp ((*pBMesh).EltType, "quad") == 0)
    {
      (*pBMesh).EltQty = 6;
      othernode = ut_alloc_1d_int ((*pBMesh).EltQty + 1);

      if ((*pBMesh).EltNodes == NULL)
      {
        if ((*pBMesh).EltOrder == 1)
          (*pBMesh).EltNodes = ut_alloc_2d_int ((*pBMesh).EltQty + 1, 4);
        else if ((*pBMesh).EltOrder == 2)
          (*pBMesh).EltNodes = ut_alloc_2d_int ((*pBMesh).EltQty + 1, 8);
        else
          abort ();
      }

      (*pBMesh).EltNodes[1][0] = tab[0];
      (*pBMesh).EltNodes[1][1] = tab[1];
      (*pBMesh).EltNodes[1][2] = tab[2];
      (*pBMesh).EltNodes[1][3] = tab[3];
      othernode[1] = tab[4];

      (*pBMesh).EltNodes[2][0] = tab[4];
      (*pBMesh).EltNodes[2][1] = tab[5];
      (*pBMesh).EltNodes[2][2] = tab[6];
      (*pBMesh).EltNodes[2][3] = tab[7];
      othernode[2] = tab[0];

      (*pBMesh).EltNodes[3][0] = tab[0];
      (*pBMesh).EltNodes[3][1] = tab[1];
      (*pBMesh).EltNodes[3][2] = tab[5];
      (*pBMesh).EltNodes[3][3] = tab[4];
      othernode[3] = tab[2];

      (*pBMesh).EltNodes[4][0] = tab[3];
      (*pBMesh).EltNodes[4][1] = tab[2];
      (*pBMesh).EltNodes[4][2] = tab[6];
      (*pBMesh).EltNodes[4][3] = tab[7];
      othernode[4] = tab[1];

      (*pBMesh).EltNodes[5][0] = tab[1];
      (*pBMesh).EltNodes[5][1] = tab[2];
      (*pBMesh).EltNodes[5][2] = tab[6];
      (*pBMesh).EltNodes[5][3] = tab[5];
      othernode[5] = tab[3];

      (*pBMesh).EltNodes[6][0] = tab[0];
      (*pBMesh).EltNodes[6][1] = tab[3];
      (*pBMesh).EltNodes[6][2] = tab[7];
      (*pBMesh).EltNodes[6][3] = tab[4];
      othernode[6] = tab[1];

      if (Mesh.EltOrder == 2)
      {
        (*pBMesh).EltNodes[1][4] = tab[8];
        (*pBMesh).EltNodes[1][5] = tab[11];
        (*pBMesh).EltNodes[1][6] = tab[13];
        (*pBMesh).EltNodes[1][7] = tab[9];

        (*pBMesh).EltNodes[2][4] = tab[16];
        (*pBMesh).EltNodes[2][5] = tab[18];
        (*pBMesh).EltNodes[2][6] = tab[19];
        (*pBMesh).EltNodes[2][7] = tab[17];

        (*pBMesh).EltNodes[3][4] = tab[8];
        (*pBMesh).EltNodes[3][5] = tab[12];
        (*pBMesh).EltNodes[3][6] = tab[16];
        (*pBMesh).EltNodes[3][7] = tab[10];

        (*pBMesh).EltNodes[4][4] = tab[13];
        (*pBMesh).EltNodes[4][5] = tab[14];
        (*pBMesh).EltNodes[4][6] = tab[19];
        (*pBMesh).EltNodes[4][7] = tab[15];

        (*pBMesh).EltNodes[5][4] = tab[11];
        (*pBMesh).EltNodes[5][5] = tab[14];
        (*pBMesh).EltNodes[5][6] = tab[18];
        (*pBMesh).EltNodes[5][7] = tab[12];

        (*pBMesh).EltNodes[6][4] = tab[9];
        (*pBMesh).EltNodes[6][5] = tab[15];
        (*pBMesh).EltNodes[6][6] = tab[17];
        (*pBMesh).EltNodes[6][7] = tab[10];
      }
    }

    else
      abort ();
  }

  // if 2D mesh and Nodes present, making 2D elements point outwards from the 3D element
  if ((*pBMesh).Dimension == 2 && Nodes.NodeQty > 0)
    for (i = 1; i <= (*pBMesh).EltQty; i++)
    {
      neut_mesh_elt_eq (*pBMesh, Nodes, i, eq);
      if (ut_space_point_plane_side (Nodes.NodeCoo[othernode[i]] - 1, eq) ==
          1)
        neut_mesh_elt_reversenodes (pBMesh, i);
    }

  ut_free_1d (&eq);
  ut_free_1d_int (&othernode);

  return;
}

// Generates the 2D mesh corresponding to a 3D mesh.
// It also retuns the FaceQty value and the FacePoly array, which hold
// the number of 2D elsets and their relations to the 3D elsets.  The
// terms `FaceQty' and `FacePoly' are used in reference to the Tess
// structure, and closely match them, except that FacePoly is indexed O
// (vs 1 in the Tess structure)
void
neut_mesh3d_mesh2d (struct NODES Nodes, struct MESH Mesh3D,
                    struct MESH *pMesh2D, int ***pFacePoly, int *pFaceQty,
                    int verbosity)
{
  int i, j, k, elt3dqty, elset, dir, elset3dqty;
  int *elt3d = NULL;
  struct MESH EltMesh2D;
  int *elset3d = ut_alloc_1d_int (2);
  char *progress = ut_alloc_1d_char (20);
  double **fodeq = NULL;
  int fodqty = 0;

  neut_mesh_set_zero (&EltMesh2D);

  neut_mesh_reset (pMesh2D);
  (*pMesh2D).Dimension = 2;
  (*pMesh2D).EltOrder = Mesh3D.EltOrder;
  ut_string_string (Mesh3D.EltType, &(*pMesh2D).EltType);

  (*pFaceQty) = 0;

  // Step 1/2: finding 2D elements -------------------------------------

  // for every 3D elt, for every of its facets, we look for all
  // its parent 3d elts (can be 1 or 2) and 3D elsets.
  // If more than 1 elset, the facet is an interface element - recording
  // it.

  if (verbosity)
    ut_print_progress (stdout, 0, Mesh3D.EltQty, "[1/2] %3.0f%%", progress);

  for (i = 1; i <= Mesh3D.EltQty; i++)
  {
    neut_mesh_elt_boundmesh (Nodes, Mesh3D, i, &EltMesh2D);

    for (j = 1; j <= EltMesh2D.EltQty; j++)
    {
      neut_mesh_elt2d_elts3d (EltMesh2D, j, Mesh3D, &elt3d, &elt3dqty);
      neut_mesh_elt2d_elsets3d (EltMesh2D, j, Mesh3D, &elset3d, &elset3dqty);

      if (elt3d[0] >= i)        // means the facet has not been test yet
        if (elset3dqty > 1)
          neut_mesh_addelt (pMesh2D, EltMesh2D.EltNodes[j]);
    }

    neut_mesh_free (&EltMesh2D);

    if (verbosity)
      ut_print_progress_nonl (stdout, i, Mesh3D.EltQty, "[1/2] %3.0f%%",
                              progress);
  }

  neut_mesh_init_nodeelts (pMesh2D, 0);

  // Step 2/2: assigning 2D elements to 2D elsets ----------------------

  // The principle is to loop on the 2d elements, and for each:
  // - if it has already been assigned to an elset, skip it.
  // - else, create a new elset and a new face;
  // - look for the connected elements one by one, and for each:
  //   . if it belongs to the same face (i.e. no changing polys),
  //     assigned it to the same elset/face.
  //   . else, do nothing.
  // Set ElsetQty, EltElset, *pFaceQty and *pFacePoly.

  if (verbosity)
    ut_print_progress (stdout, 0, (*pMesh2D).EltQty, "[2/2] %3.0f%%",
                       progress);

  (*pMesh2D).EltElset = ut_alloc_1d_int ((*pMesh2D).EltQty + 1);

  elset = 0;
  for (i = 1; i <= (*pMesh2D).EltQty; i++)
  {
    if ((*pMesh2D).EltElset[i] == 0)
    {
      // adding elset to *pMesh2D and defining face (*pFace...)
      // (topology)

      (*pMesh2D).ElsetQty = ++elset;
      (*pMesh2D).EltElset[i] = elset;

      neut_mesh_elt2d_elsets3d (*pMesh2D, i, Mesh3D, &elset3d, &elset3dqty);

      // if < 0: domain face; finding the good one based on the face
      // equations
      if (elset3d[1] == -1)
      {
        neut_mesh_elt2d_fod (Nodes, *pMesh2D, i, &fodeq, &fodqty,
                             &(elset3d[1]), &dir);

        if (dir == -1)
          neut_mesh_elt_reversenodes (pMesh2D, i);
      }

      (*pFaceQty) = elset;
      (*pFacePoly) =
        ut_realloc_2d_int_addline (*pFacePoly, *pFaceQty, elset3dqty);
      ut_array_1d_int_memcpy (elset3d, elset3dqty,
                              (*pFacePoly)[(*pFaceQty) - 1]);

      // loop on the neighbors

      int testqty, elt2d;
      int *test = NULL;

      neut_mesh_elt_neighelts (*pMesh2D, i, &test, &testqty);

      for (j = 0; j < testqty; j++)
        if ((*pMesh2D).EltElset[test[j]] == 0)
        {
          elt2d = test[j];

          neut_mesh_elt2d_elsets3d (*pMesh2D, elt2d, Mesh3D, &elset3d,
                                    &elset3dqty);

          // domain face
          if (elset3d[1] == -1)
          {
            neut_mesh_elt2d_fod (Nodes, *pMesh2D, elt2d, &fodeq, &fodqty,
                                 &(elset3d[1]), &dir);

            if (dir == -1)
              neut_mesh_elt_reversenodes (pMesh2D, elt2d);
          }

          // the elt has the same topology, therefore same face -
          // recording it and adding non-registered neighbors to the test list.
          if (ut_array_1d_int_equal
              (elset3d, elset3dqty, (*pFacePoly)[elset - 1], 2))
          {
            (*pMesh2D).EltElset[elt2d] = elset;

            int *tmp = NULL;
            int tmpqty;
            neut_mesh_elt_neighelts (*pMesh2D, elt2d, &tmp, &tmpqty);

            // adding neighbors to test to the buffer
            for (k = 0; k < tmpqty; k++)
              if ((*pMesh2D).EltElset[tmp[k]] == 0
                  && ut_array_1d_int_eltpos (test + j, testqty - j,
                                             tmp[k]) == -1)
              {
                if (tmp[k] == 0)
                  ut_print_neperbug ();

                test = ut_realloc_1d_int (test, ++testqty);
                test[testqty - 1] = tmp[k];
              }

            ut_free_1d_int (&tmp);
          }
        }

      ut_free_1d_int (&test);
    }

    if (verbosity)
      ut_print_progress_nonl (stdout, i, (*pMesh2D).EltQty, "[2/2] %3.0f%%",
                              progress);
  }

  if (verbosity)
    ut_print_progress (stdout, (*pMesh2D).EltQty, (*pMesh2D).EltQty,
                       "%3.0f%%", progress);

  neut_mesh_init_elsets (pMesh2D);

  ut_free_2d (&fodeq, fodqty);
  ut_free_1d_int (&elt3d);
  ut_free_1d_int (&elset3d);
  ut_free_1d_char (&progress);

  return;
}

// Generates the 1D mesh corresponding to a 2D mesh.  It also retuns the
// EdgeQty value and the EdgeFaceQty and EdgeFaceNb arrays, which hold
// the number of 1D elsets and their relations to the 2D elsets.  These
// terms are used in reference to the Tess structure, and closely match
// them, except that EdgeFaceQty andEdgeFaceNb are indexed O (vs 1 in
// the Tess structure)
void
neut_mesh2d_mesh1d (struct NODES Nodes, struct MESH Mesh2D,
                    struct MESH *pMesh1D, int ***pEdgeFaceNb,
                    int **pEdgeFaceQty, int *pEdgeQty, int verbosity)
{
  int i, j, k, dir, elt2dqty, elset2dqty, elset;
  struct MESH EltMesh1D;
  int *elt2d = NULL;
  int *elset2d = NULL;
  char *progress = ut_alloc_1d_char (20);
  struct NODES fake;
  double **eodeq = NULL;
  int eodqty = 0;

  int **EdgeFaceNb = NULL;
  int *EdgeFaceQty = NULL;
  int EdgeQty;

  neut_nodes_set_zero (&fake);
  neut_mesh_set_zero (&EltMesh1D);

  if (!Mesh2D.EltElset)
    ut_print_message (2, 0, "neut_mesh2d_mesh1d needs Mesh2D.EltElset.\n");

  neut_mesh_reset (pMesh1D);
  (*pMesh1D).Dimension = 1;
  (*pMesh1D).EltOrder = Mesh2D.EltOrder;
  ut_string_string (Mesh2D.EltType, &(*pMesh1D).EltType);

  // Step 1/2: finding 1D elements -------------------------------------

  // Recording all 1d elts by looping on the edges of all 2d elts.

  if (verbosity)
    ut_print_progress (stdout, 0, Mesh2D.EltQty, "[1/2] %3.0f%%", progress);

  for (i = 1; i <= Mesh2D.EltQty; i++)
  {
    neut_mesh_elt_boundmesh (fake, Mesh2D, i, &EltMesh1D);

    for (j = 1; j <= EltMesh1D.EltQty; j++)
    {
      neut_mesh_elt1d_elts2d (EltMesh1D, j, Mesh2D, &elt2d, &elt2dqty);
      neut_mesh_elt1d_elsets2d (EltMesh1D, j, Mesh2D, &elset2d, &elset2dqty);

      if (elt2d[0] >= i)        // means not recorded yet
        if (elset2dqty > 1)
          neut_mesh_addelt (pMesh1D, EltMesh1D.EltNodes[j]);
    }

    if (verbosity)
      ut_print_progress_nonl (stdout, i, Mesh2D.EltQty, "[1/2] %3.0f%%",
                              progress);
  }

  neut_mesh_init_nodeelts (pMesh1D, 0);

  // Step 2/2: assigning 1D elements to 1D elsets ----------------------

  (*pMesh1D).EltElset = ut_alloc_1d_int ((*pMesh1D).EltQty + 1);

  // The principle is to loop on the 1d elements, and for each:
  // - if it has already been assigned to an edge, skip it.
  // - else, create a new elset and a new edge;
  // - look for the connected elements one by one, and for each:
  //   . if it belongs to the same edge (i.e. no changing faces),
  //     assigned it to the same elset/edge.
  //   . else, do nothing.
  // Set ElsetQty, EltElset, *pEdgeQty, *pEdgeFaceQty and *pEdgeFaceNb

  EdgeQty = 0;

  if (verbosity)
    ut_print_progress (stdout, 0, (*pMesh1D).EltQty, "[2/2] %3.0f%%",
                       progress);

  elset = 0;
  for (i = 1; i <= (*pMesh1D).EltQty; i++)
  {
    if ((*pMesh1D).EltElset[i] == 0)
    {
      // adding elset to *pMesh1D and defining edge (*pEdge...) (topology)

      (*pMesh1D).ElsetQty = ++elset;
      (*pMesh1D).EltElset[i] = elset;

      neut_mesh_elt1d_elsets2d (*pMesh1D, i, Mesh2D, &elset2d, &elset2dqty);

      // if < 0: domain edge; finding the good one based on the edge
      // equations
      if (elset2d[1] == -1)
      {
        neut_mesh_elt1d_eod (Nodes, *pMesh1D, i, &eodeq, &eodqty,
                             &(elset2d[1]), &dir);

        if (dir == -1)
          neut_mesh_elt_reversenodes (pMesh1D, i);
      }

      EdgeQty = elset;
      EdgeFaceQty = ut_realloc_1d_int (EdgeFaceQty, EdgeQty);
      EdgeFaceQty[EdgeQty - 1] = elset2dqty;
      EdgeFaceNb =
        ut_realloc_2d_int_addline (EdgeFaceNb, EdgeQty, elset2dqty);
      ut_array_1d_int_memcpy (elset2d, elset2dqty, EdgeFaceNb[EdgeQty - 1]);

      // loop on neighbors

      int testqty, elt1d;
      int *test = NULL;

      neut_mesh_elt_neighelts (*pMesh1D, i, &test, &testqty);

      for (j = 0; j < testqty; j++)
        if ((*pMesh1D).EltElset[test[j]] == 0)
        {
          elt1d = test[j];

          neut_mesh_elt1d_elsets2d (*pMesh1D, elt1d, Mesh2D, &elset2d,
                                    &elset2dqty);

          // if < 0: domain edge; finding the good one based on the edge
          // equations
          if (elset2d[1] == -1)
          {
            neut_mesh_elt1d_eod (Nodes, *pMesh1D, elt1d, &eodeq, &eodqty,
                                 &(elset2d[1]), &dir);

            if (dir == -1)
              neut_mesh_elt_reversenodes (pMesh1D, elt1d);
          }

          // the elt has the same topology, therefore same edge -
          // recording it and adding non-registered neighbors to the
          // buffer.
          if (ut_array_1d_int_equal
              (elset2d, elset2dqty, EdgeFaceNb[elset - 1],
               EdgeFaceQty[elset - 1]))
          {
            (*pMesh1D).EltElset[elt1d] = elset;

            int *tmp = NULL;
            int tmpqty;
            neut_mesh_elt_neighelts (*pMesh1D, elt1d, &tmp, &tmpqty);
            // If there is 2 neighbors, we are in the body of an edge;
            // recording elt which is not the previous one.
            // If there is more, we are done (end of an edge).
            // printf ("neut_mesh_elt_neighelts returns tmpqty = %d tmp = ", tmpqty);
            // ut_array_1d_int_fprintf (stdout, tmp, tmpqty, "%d");
            if (tmpqty == 2)
              for (k = 0; k < tmpqty; k++)
                if ((*pMesh1D).EltElset[tmp[k]] == 0)
                {
                  if (tmp[k] == 0)
                    abort ();
                  test[j] = tmp[k];
                  j--;
                }
            ut_free_1d_int (&tmp);
          }
        }

      ut_free_1d_int (&test);
    }

    if (verbosity)
      ut_print_progress_nonl (stdout, i, (*pMesh1D).EltQty, "[2/2] %3.0f%%",
                              progress);
  }

  if (verbosity)
    ut_print_progress (stdout, (*pMesh1D).EltQty, (*pMesh1D).EltQty,
                       "%3.0f%%", progress);

  if (pEdgeQty)
    (*pEdgeQty) = EdgeQty;

  if (pEdgeFaceQty)
  {
    (*pEdgeFaceQty) = ut_alloc_1d_int (EdgeQty);

    ut_array_1d_int_memcpy (EdgeFaceQty, EdgeQty, *pEdgeFaceQty);
  }

  if (pEdgeFaceNb)
  {
    (*pEdgeFaceNb) = ut_alloc_1d_pint (EdgeQty);

    for (i = 0; i < EdgeQty; i++)
    {
      (*pEdgeFaceNb)[i] = ut_alloc_1d_int (EdgeFaceQty[i]);
      ut_array_1d_int_memcpy (EdgeFaceNb[i], EdgeFaceQty[i],
                              (*pEdgeFaceNb)[i]);
    }
  }

  ut_free_1d_int (&EdgeFaceQty);
  ut_free_2d_int (&EdgeFaceNb, EdgeQty);

  neut_mesh_init_elsets (pMesh1D);

  neut_mesh_free (&EltMesh1D);
  ut_free_1d_int (&elt2d);
  ut_free_1d_int (&elset2d);
  ut_free_1d_char (&progress);
  neut_nodes_free (&fake);
  ut_free_2d (&eodeq, eodqty);

  return;
}

// Generates the 0D mesh corresponding to a 1D mesh.  It also retuns the
// VerQty value and the VerEdgeQty and VerEdgeNb arrays, which hold the
// number of 0D elsets and their relations to the 1D elsets.  These
// terms are used in reference to the Tess structure, and closely match
// them, except that VerEdgeQty and VerEdgeNb are indexed O (vs 1 in the
// Tess structure)
void
neut_mesh1d_mesh0d (struct MESH Mesh1D, struct MESH *pMesh0D,
                    int ***pVerEdgeNb, int **pVerEdgeQty, int *pVerQty,
                    int verbosity)
{
  int i, j, elt1dqty, elset1dqty;
  struct MESH EltMesh0D;
  int *elt1d = NULL;
  int *elset1d = NULL;
  char *progress = ut_alloc_1d_char (20);
  struct NODES fake;

  neut_nodes_set_zero (&fake);

  neut_mesh_set_zero (&EltMesh0D);

  if (Mesh1D.EltElset == NULL)
  {
    ut_print_message (2, 0, "neut_mesh1d_mesh0d needs Mesh1D.EltElset.\n");
    ut_print_neperbug ();
  }

  (*pVerQty) = 0;

  neut_mesh_reset (pMesh0D);
  (*pMesh0D).Dimension = 0;
  (*pMesh0D).EltOrder = Mesh1D.EltOrder;
  ut_string_string (Mesh1D.EltType, &(*pMesh0D).EltType);

  // for every 1D elt, for every of its vertices, we look for all
  // its parent 1D elts (can be any number).

  if (verbosity)
    ut_print_progress (stdout, 0, Mesh1D.EltQty, "[1/2] %3.0f%%", progress);

  for (i = 1; i <= Mesh1D.EltQty; i++)
  {
    neut_mesh_elt_boundmesh (fake, Mesh1D, i, &EltMesh0D);

    for (j = 1; j <= 2; j++)
    {
      neut_mesh_elt0d_elts1d (EltMesh0D, j, Mesh1D, &elt1d, &elt1dqty);
      /*
         // elt1dqty == 1 means that an edge has a dead end.
         // this is OK for a pure 1D mesh...
         if (elt1dqty == 1)
         ut_print_neperbug ();
       */

      neut_mesh_elt0d_elsets1d (EltMesh0D, j, Mesh1D, &elset1d, &elset1dqty);

      // means not recorded yet
      if (elt1d[0] >= i)
      {
        // the following condition works for both conditions (above)
        if (elset1dqty > 1)
          neut_mesh_addelt (pMesh0D, EltMesh0D.EltNodes[j]);
      }
    }

    if (verbosity)
      ut_print_progress_nonl (stdout, i, Mesh1D.EltQty, "[1/2] %3.0f%%",
                              progress);
  }

  neut_mesh_init_nodeelts (pMesh0D, 0);

  (*pMesh0D).EltElset = ut_alloc_1d_int ((*pMesh0D).EltQty + 1);

  // The principle is to loop on the 0d elements, and for each:
  // - if it has already been assigned to an ver, skip it.
  // - else, create a new elset and a new ver;
  // - look for the connected elements one by one, and for each:
  //   . if it belongs to the same ver (i.e. no changing edges),
  //     assigned it to the same elset/ver.
  //   . else, do nothing.
  // Set ElsetQty, EltElset, *pVerQty, *pVerEdgeQty and *pVerEdgeNb

  (*pVerQty) = (*pMesh0D).EltQty;
  (*pVerEdgeQty) = ut_alloc_1d_int (*pVerQty);

  if (verbosity)
    ut_print_progress (stdout, 0, (*pMesh0D).EltQty, "[2/2] %3.0f%%",
                       progress);

  int eod = 0;
  for (i = 1; i <= (*pMesh0D).EltQty; i++)
  {
    (*pMesh0D).ElsetQty = i;
    (*pMesh0D).EltElset[i] = i;

    neut_mesh_elt0d_elsets1d (*pMesh0D, i, Mesh1D, &elset1d, &elset1dqty);

    // if < 0: domain ver; finding the good one based on the face
    // equations
    if (elset1dqty == 2 && elset1d[1] == -1)
      elset1d[1] = --eod;

    (*pVerEdgeNb) = ut_realloc_2d_int_addline (*pVerEdgeNb, i, elset1dqty);
    ut_array_1d_int_memcpy (elset1d, elset1dqty, (*pVerEdgeNb)[i - 1]);
    (*pVerEdgeQty)[i - 1] = elset1dqty;

    if (verbosity)
      ut_print_progress_nonl (stdout, i, (*pMesh0D).EltQty, "[2/2] %3.0f%%",
                              progress);
  }

  if (verbosity)
    ut_print_progress (stdout, (*pMesh0D).EltQty, (*pMesh0D).EltQty,
                       "%3.0f%%", progress);

  (*pMesh0D).ElsetQty = (*pVerQty);
  (*pMesh0D).Elsets = ut_alloc_2d_int ((*pVerQty) + 1, 1);

  for (i = 1; i <= (*pMesh0D).EltQty; i++)
    neut_mesh_elset_addelt (pMesh0D, (*pMesh0D).EltElset[i], i);

  neut_mesh_free (&EltMesh0D);
  ut_free_1d_int (&elt1d);
  ut_free_1d_int (&elset1d);
  ut_free_1d_char (&progress);
  neut_nodes_free (&fake);

  return;
}

void
neut_mesh_elsets_nodes (struct MESH Mesh, int *elsets, int elsetqty,
                        int **pnodes, int *pnodeqty)
{
  int i, elset, NodeQty;
  int *nodes = NULL;

  (*pnodes) = ut_alloc_1d_int (1);

  (*pnodeqty) = 0;
  for (i = 0; i < elsetqty; i++)
  {
    elset = elsets[i];
    neut_mesh_elset_nodes (Mesh, elset, &nodes, &NodeQty);
    (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty) + NodeQty);
    ut_array_1d_int_memcpy (nodes, NodeQty, (*pnodes) + (*pnodeqty));
    ut_free_1d_int (&nodes);

    (*pnodeqty) += NodeQty;
  }

  ut_array_1d_int_sort_uniq ((*pnodes), (*pnodeqty), &(*pnodeqty));
  (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty));

  return;
}

int
neut_mesh_elt2d_elts3d (struct MESH Mesh2D, int elt2d, struct MESH Mesh3D,
                        int **pelt3d, int *pelt3dqty)
{
  int NodeQty = neut_elt_nodeqty (Mesh2D.EltType, 2, Mesh2D.EltOrder);

  if (Mesh3D.EltQty == 0)
  {
    (*pelt3dqty) = 0;
    return 0;
  }

  neut_mesh_nodes_comelts (Mesh3D, Mesh2D.EltNodes[elt2d], NodeQty, pelt3d,
                           pelt3dqty);

  return (*pelt3dqty >= 0 && *pelt3dqty <= 2) ? *pelt3dqty : -1;
}

int
neut_mesh_elt2d_elsets3d (struct MESH Mesh2D, int elt2d, struct MESH Mesh3D,
                          int **pelset3d, int *pelset3dqty)
{
  int *elt3d = NULL;
  int elt3dqty;

  if (Mesh3D.EltQty == 0)
  {
    (*pelset3dqty) = 0;
    return 0;
  }

  ut_free_1d_int (pelset3d);

  neut_mesh_elt2d_elts3d (Mesh2D, elt2d, Mesh3D, &elt3d, &elt3dqty);

  // is on the domain surface
  if (elt3dqty == 1)
  {
    (*pelset3d) = ut_alloc_1d_int (2);
    (*pelset3dqty) = 2;
    (*pelset3d)[0] = Mesh3D.EltElset[elt3d[0]];
    (*pelset3d)[1] = -1;
  }
  // may or may not be on an interface
  else
    neut_mesh_elts_elsets (Mesh3D, elt3d, elt3dqty, pelset3d, pelset3dqty);

  ut_free_1d_int (&elt3d);

  return *pelset3dqty;
}

void
neut_mesh_elt3d_elts2d (struct MESH Mesh3D, int elt3d, struct MESH Mesh2D,
                        int **pelts2d, int *pelt2dqty)
{
  int i, nb;
  struct MESH Facet;
  int *pos = NULL;
  struct NODES fake;

  if (Mesh2D.EltQty == 0)
  {
    (*pelt2dqty) = 0;
    return;
  }

  pos = ut_alloc_1d_int (4);

  neut_nodes_set_zero (&fake);

  ut_free_1d_int (pelts2d);

  neut_mesh_set_zero (&Facet);

  neut_mesh_elt_boundmesh (fake, Mesh3D, elt3d, &Facet);

  (*pelt2dqty) = 0;

  for (i = 0; i < 4; i++)
  {
    neut_mesh_nodes_comelt (Mesh2D, Facet.EltNodes[i + 1], 3, &nb);
    if (nb > 0)
    {
      (*pelts2d) = ut_realloc_1d_int (*pelts2d, (*pelt2dqty) + 1);
      (*pelts2d)[(*pelt2dqty)++] = nb;
    }
  }

  neut_mesh_free (&Facet);
  ut_free_1d_int (&pos);
  neut_nodes_free (&fake);

  return;
}

void
neut_mesh_elts_elsets (struct MESH Mesh, int *elts, int EltQty, int **pelsets,
                       int *pelsetqty)
{
  int i, elset;

  ut_free_1d_int (pelsets);

  (*pelsetqty) = 0;
  for (i = 0; i < EltQty; i++)
    if (elts[i] >= 1 && elts[i] <= Mesh.EltQty)
    {
      elset = Mesh.EltElset[elts[i]];
      if (ut_array_1d_int_eltpos (*pelsets, *pelsetqty, elset) == -1)
      {
        (*pelsets) = ut_realloc_1d_int (*pelsets, (*pelsetqty) + 1);
        (*pelsets)[(*pelsetqty)++] = elset;
      }
    }

  ut_array_1d_int_sort (*pelsets, *pelsetqty);

  return;
}

void
neut_mesh_facepoly (struct NODES Nodes, struct MESH Mesh2D,
                    struct MESH Mesh3D, int ***pfacepoly)
{
  int i, elt2d, elt3dqty, elset3dqty;
  int *elts3d = NULL;

  (*pfacepoly) = ut_alloc_1d_pint (Mesh2D.ElsetQty + 1);

  int fodqty = 0;
  double **fodeq = NULL;
  int dir;

  // for every face, taking one element (the 1st) and looking for its 2
  // 3D elements. Their elsets are the face polys.
  // in case of only one elt, recording one elset as -7.
  for (i = 1; i <= Mesh2D.ElsetQty; i++)
  {
    elt2d = Mesh2D.Elsets[i][1];

    neut_mesh_elt2d_elts3d (Mesh2D, elt2d, Mesh3D, &elts3d, &elt3dqty);
    if (elt3dqty != 2 && elt3dqty != 1)
      ut_print_neperbug ();
    neut_mesh_elts_elsets (Mesh3D, elts3d, elt3dqty, &((*pfacepoly)[i]),
                           &elset3dqty);

    if (elset3dqty != 2 && elset3dqty != 1)
      ut_print_neperbug ();

    if (elset3dqty == 1)
    {
      (*pfacepoly)[i] = ut_realloc_1d_int ((*pfacepoly)[i], 2);
      (*pfacepoly)[i][1] = (*pfacepoly)[i][0];
      neut_mesh_elt2d_fod (Nodes, Mesh2D, elt2d, &fodeq, &fodqty,
                           &((*pfacepoly)[i][0]), &dir);
    }

    if ((*pfacepoly)[i][0] < 0 && (*pfacepoly)[i][1] > 0)
      ut_array_1d_int_reverseelts ((*pfacepoly)[i], 2);
  }

  ut_free_1d_int (&elts3d);
  ut_free_2d (&fodeq, fodqty);

  return;
}

void
neut_mesh_edgeface (struct MESH Mesh1D, struct MESH Mesh2D,
                    int ***pedgefacenb, int **pedgefaceqty)
{
  int i, elt1d, elt2dqty, elset2dqty;
  int *elsets2d = NULL;
  int *elts2d = NULL;

  (*pedgefacenb) = ut_alloc_1d_pint (Mesh1D.ElsetQty + 1);
  (*pedgefaceqty) = ut_alloc_1d_int (Mesh1D.ElsetQty + 12);

  // for every edge, taking one element (the 1st) and looking for its
  // 2D elements. Their elsets are the edge faces.
  for (i = 1; i <= Mesh1D.ElsetQty; i++)
  {
    elt1d = Mesh1D.Elsets[i][1];

    neut_mesh_elt1d_elts2d (Mesh1D, elt1d, Mesh2D, &elts2d, &elt2dqty);
    if (elt2dqty == 0)
      ut_print_neperbug ();
    neut_mesh_elts_elsets (Mesh2D, elts2d, elt2dqty, &elsets2d, &elset2dqty);
    (*pedgefacenb)[i] = ut_alloc_1d_int (elset2dqty);
    ut_array_1d_int_memcpy (elsets2d, elset2dqty, (*pedgefacenb)[i]);
    (*pedgefaceqty)[i] = elset2dqty;

    if (elset2dqty == 0)
      ut_print_neperbug ();
  }

  ut_free_1d_int (&elts2d);
  ut_free_1d_int (&elsets2d);

  return;
}

void
neut_mesh_veredge (struct MESH Mesh0D, struct MESH Mesh1D, int ***pveredgenb,
                   int **pveredgeqty)
{
  int i, elt0d, elt2dqty, elset1dqty;
  int *elts1d = NULL;
  int *elsets1d = NULL;

  (*pveredgenb) = ut_alloc_1d_pint (Mesh0D.ElsetQty + 1);
  (*pveredgeqty) = ut_alloc_1d_int (Mesh0D.ElsetQty + 12);

  // for every ver, looking for its 1D elements. Their elsets are the
  // ver edges.
  for (i = 1; i <= Mesh0D.ElsetQty; i++)
  {
    elt0d = Mesh0D.Elsets[i][1];

    neut_mesh_elt0d_elts1d (Mesh0D, elt0d, Mesh1D, &elts1d, &elt2dqty);
    if (elt2dqty == 0)
      ut_print_neperbug ();
    neut_mesh_elts_elsets (Mesh1D, elts1d, elt2dqty, &elsets1d, &elset1dqty);
    (*pveredgenb)[i] = ut_alloc_1d_int (elset1dqty);
    ut_array_1d_int_memcpy (elsets1d, elset1dqty, (*pveredgenb)[i]);
    (*pveredgeqty)[i] = elset1dqty;

    if (elset1dqty == 0)
      ut_print_neperbug ();
  }

  ut_free_1d_int (&elts1d);
  ut_free_1d_int (&elsets1d);

  return;
}

int
neut_mesh_elt_neighelts (struct MESH Mesh, int elt, int **pnelts,
                         int *pneltqty)
{
  int i, j, EltQty;
  int *elts = NULL;
  int *nodes = ut_alloc_1d_int (8);
  int boundqty;
  int boundnodeqty;
  int **seq = NULL;

  ut_free_1d_int (pnelts);

  neut_elt_boundlist (Mesh.EltType, Mesh.Dimension, &seq, &boundqty,
                      &boundnodeqty);

  // for each elt boundary, looking for the neighboring elt
  (*pneltqty) = 0;
  for (i = 0; i < boundqty; i++)
  {
    for (j = 0; j < boundnodeqty; j++)
      nodes[j] = Mesh.EltNodes[elt][seq[i][j]];

    neut_mesh_nodes_comelts (Mesh, nodes, boundnodeqty, &elts, &EltQty);

    if ((EltQty == 0) || (EltQty == 1 && elts[0] != elt)
        || (EltQty > 2 && Mesh.Dimension == 3))
      ut_print_neperbug ();

    if (EltQty == 1)
      (*pneltqty) = 0;
    else
      for (j = 0; j < EltQty; j++)
        if (elts[j] != elt)
        {
          (*pnelts) = ut_realloc_1d_int (*pnelts, (*pneltqty) + 1);
          (*pnelts)[(*pneltqty)++] = elts[j];
        }

    ut_free_1d_int (&elts);
  }

  ut_free_1d_int (&nodes);
  ut_free_2d_int (&seq, boundqty);

  return 0;
}

int
neut_mesh_elt_elset_neighelts (struct MESH Mesh, int elt, int elset,
                               int **pnelts, int *pneltqty)
{
  int i;

  if (Mesh.EltElset == NULL)
  {
    printf ("neut_mesh_elt_elset_neighelts needs EltElset\n");
    ut_print_neperbug ();
  }

  neut_mesh_elt_neighelts (Mesh, elt, pnelts, pneltqty);

  for (i = 0; i < (*pneltqty); i++)
    if (Mesh.EltElset[(*pnelts)[i]] != elset)
      (*pnelts)[i] = 0;

  (*pneltqty) -= ut_array_1d_int_rmelt (*pnelts, (*pneltqty), 0, INT_MAX);

  return 0;
}

void
neut_mesh_elt_elset_neighelts_minnodeqty (struct MESH Mesh, int elt,
                                          int elset, int minnodeqty,
                                          int **pnelts, int *pneltqty)
{
  int i, NodeQty;
  int *pair = ut_alloc_1d_int (2);

  neut_mesh_elt_elset_neighelts (Mesh, elt, elset, pnelts, pneltqty);

  pair[0] = elt;
  if (*pneltqty > 0)
    for (i = 0; i < *pneltqty; i++)
    {
      pair[1] = (*pnelts)[i];
      neut_mesh_elts_comnodes (Mesh, pair, 2, NULL, &NodeQty);

      if (NodeQty < minnodeqty)
        (*pnelts)[i] = 0;
    }

  (*pneltqty) -= ut_array_1d_int_rmelt (*pnelts, *pneltqty, 0, INT_MAX);

  ut_free_1d_int (&pair);

  return;
}

void
neut_mesh_elts_boundelts (struct MESH Mesh, int *elts, int EltQty,
                          int **pboundelts, int *pboundeltqty)
{
  int i, j, neigh;
  int *eltsinv = NULL;
  int eltsmax;
  int elt3dfaceqty;
  int **eltneighs = NULL;
  int *eltneighqty = NULL;

  if (Mesh.Dimension != 3)
    ut_print_neperbug ();

  if (strcmp (Mesh.EltType, "tri") == 0)
    elt3dfaceqty = 4;
  else if (strcmp (Mesh.EltType, "quad") == 0)
    elt3dfaceqty = 6;
  else
    abort ();

  eltneighs = ut_alloc_1d_pint (EltQty);
  eltneighqty = ut_alloc_1d_int (EltQty);

  (*pboundelts) = ut_alloc_1d_int (EltQty);

  ut_array_1d_int_inv (elts, EltQty, &eltsinv, &eltsmax);

  // for each elt, recording neighboring elts
  for (i = 0; i < EltQty; i++)
    neut_mesh_elt_neighelts (Mesh, elts[i], &(eltneighs[i]),
                             &(eltneighqty[i]));

  // for each elt, checking if all its neighbors are in the elts array.
  // If not, the elt is a bound elt, and recorded as such.
  // note: if eltneighqty != 4, the elt is systematically a bound elt.

  (*pboundeltqty) = 0;
  for (i = 0; i < EltQty; i++)
  {
    if (eltneighqty[i] != elt3dfaceqty)
      (*pboundelts)[(*pboundeltqty)++] = elts[i];
    else
    {
      for (j = 0; j < elt3dfaceqty; j++)
      {
        neigh = eltneighs[i][j];
        if (neigh > eltsmax)
        {
          (*pboundelts)[(*pboundeltqty)++] = elts[i];
          break;
        }
        else if (eltsinv[neigh] == -1)
        {
          (*pboundelts)[(*pboundeltqty)++] = elts[i];
          break;
        }
      }
    }
  }

  (*pboundelts) = ut_realloc_1d_int ((*pboundelts), *pboundeltqty);

  ut_free_2d_int (&eltneighs, EltQty);
  ut_free_1d_int (&eltneighqty);
  ut_free_1d_int (&eltsinv);

  return;
}

int
neut_mesh_node_dim (struct MESH Mesh0D, struct MESH Mesh1D,
                    struct MESH Mesh2D, struct MESH Mesh3D, int node)
{
  return neut_mesh_node_dim_max (Mesh0D, Mesh1D, Mesh2D, Mesh3D, node);
}

int
neut_mesh_node_dim_max (struct MESH Mesh0D, struct MESH Mesh1D,
                        struct MESH Mesh2D, struct MESH Mesh3D, int node)
{
  int dim = -1;

  if (Mesh0D.EltQty > 0 && !Mesh0D.NodeElts)
    abort ();

  if (Mesh1D.EltQty > 1 && !Mesh1D.NodeElts)
    abort ();

  if (Mesh2D.EltQty > 2 && !Mesh2D.NodeElts)
    abort ();

  if (Mesh3D.EltQty > 3 && !Mesh3D.NodeElts)
    abort ();

  if (node <= Mesh3D.NodeQty && Mesh3D.NodeElts[node][0] > 0)
    dim = 3;
  else if (node <= Mesh2D.NodeQty && Mesh2D.NodeElts[node][0] > 0)
    dim = 2;
  else if (node <= Mesh1D.NodeQty && Mesh1D.NodeElts[node][0] > 0)
    dim = 1;
  else if (node <= Mesh0D.NodeQty && Mesh0D.NodeElts[node][0] > 0)
    dim = 0;

  return dim;
}

int
neut_mesh_node_dim_min (struct MESH Mesh0D, struct MESH Mesh1D,
                        struct MESH Mesh2D, struct MESH Mesh3D, int node)
{
  int dim = -1;

  if (Mesh0D.EltQty > 0 && !Mesh0D.NodeElts)
    abort ();

  if (Mesh1D.EltQty > 1 && !Mesh1D.NodeElts)
    abort ();

  if (Mesh2D.EltQty > 2 && !Mesh2D.NodeElts)
    abort ();

  if (Mesh3D.EltQty > 3 && !Mesh3D.NodeElts)
    abort ();

  if (node <= Mesh0D.NodeQty && Mesh0D.NodeElts[node][0] > 0)
    dim = 0;
  else if (node <= Mesh1D.NodeQty && Mesh1D.NodeElts[node][0] > 0)
    dim = 1;
  else if (node <= Mesh2D.NodeQty && Mesh2D.NodeElts[node][0] > 0)
    dim = 2;
  else if (node <= Mesh3D.NodeQty && Mesh3D.NodeElts[node][0] > 0)
    dim = 3;

  return dim;
}

int
neut_mesh_elt2d_fod (struct NODES Nodes, struct MESH Mesh, int elt,
                     double ***pfodeq, int *pfodqty, int *pfod, int *pdir)
{
  int i, status;
  double *eq = ut_alloc_1d (4);
  int found = 0;

  status = 0;

  neut_mesh_elt_eq (Mesh, Nodes, elt, eq);

  (*pdir) = 1;
  for (i = 0; i < (*pfodqty) && found == 0; i++)
    if (ut_space_planes_equal ((*pfodeq)[i], eq) == 1)
    {
      (*pfod) = -i - 1;
      found = 1;

      if (ut_vector_scalprod (eq + 1, (*pfodeq)[i] + 1) > 0)
        (*pdir) = 1;
      else
        (*pdir) = -1;
    }

  // adding fod
  if (found == 0)
  {
    (*pfodeq) = ut_realloc_2d_addline ((*pfodeq), ++(*pfodqty), 4);
    ut_array_1d_memcpy (eq, 4, (*pfodeq)[(*pfodqty) - 1]);
    (*pfod) = -(*pfodqty);
  }

  ut_free_1d (&eq);

  return status;
}

int
neut_mesh_elt1d_eod (struct NODES Nodes, struct MESH Mesh, int elt,
                     double ***peodeq, int *peodqty, int *peod, int *pdir)
{
  int i, status;
  double *eq = ut_alloc_1d (3);
  int found = 0;

  status = 0;

  neut_mesh_elt_eq (Mesh, Nodes, elt, eq);

  (*pdir) = 1;
  for (i = 0; i < (*peodqty) && found == 0; i++)
    if (ut_space_lines_equal ((*peodeq)[i], eq) == 1)
    {
      (*peod) = -i - 1;
      found = 1;

      if (ut_array_1d_scalprod (eq + 1, (*peodeq)[i] + 1, 2) > 0)
        (*pdir) = 1;
      else
        (*pdir) = -1;
    }

  // adding eod
  if (found == 0)
  {
    (*peodeq) = ut_realloc_2d_addline ((*peodeq), ++(*peodqty), 3);
    ut_array_1d_memcpy (eq, 3, (*peodeq)[(*peodqty) - 1]);
    (*peod) = -(*peodqty);
  }

  ut_free_1d (&eq);

  return status;
}

void
neut_mesh_node_neighnodes (struct MESH Mesh, int node, int **pnodes,
                           int *pnodeqty)
{
  int i, elt, qty;
  int *tmp = NULL;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  if (node > Mesh.NodeQty)
  {
    (*pnodeqty) = 0;
    return;
  }

  tmp = ut_alloc_1d_int (Mesh.NodeElts[node][0] * eltnodeqty);
  qty = 0;
  for (i = 1; i <= Mesh.NodeElts[node][0]; i++)
  {
    elt = Mesh.NodeElts[node][i];

    ut_array_1d_int_memcpy (Mesh.EltNodes[elt], eltnodeqty, tmp + qty);
    qty += eltnodeqty;
  }

  ut_array_1d_int_sort_uniq (tmp, qty, &qty);
  qty -= ut_array_1d_int_rmelt (tmp, qty, node, 1);

  (*pnodes) = ut_alloc_1d_int (qty);
  (*pnodeqty) = qty;
  ut_array_1d_int_memcpy (tmp, qty, *pnodes);

  ut_free_1d_int (&tmp);

  return;
}

int
neut_mesh_node_elset (struct MESH Mesh, int node, int *pelset)
{
  int status, elsetqty, *elsets = NULL;

  neut_mesh_node_elsets (Mesh, node, &elsets, &elsetqty);

  if (elsetqty == 1)
  {
    status = 0;
    (*pelset) = elsets[0];
  }
  else
  {
    status = -1;
    (*pelset) = -1;
  }

  ut_free_1d_int (&elsets);

  return status;
}

void
neut_mesh_node_elsets (struct MESH Mesh, int node, int **pelsets,
                       int *pelsetqty)
{
  int i;

  ut_free_1d_int (pelsets);

  (*pelsets) = ut_alloc_1d_int (Mesh.NodeElts[node][0]);

  for (i = 0; i < Mesh.NodeElts[node][0]; i++)
    (*pelsets)[i] = Mesh.EltElset[Mesh.NodeElts[node][i + 1]];

  ut_array_1d_int_sort_uniq (*pelsets, Mesh.NodeElts[node][0], pelsetqty);

  (*pelsets) = ut_realloc_1d_int (*pelsets, *pelsetqty);

  return;
}

void
neut_mesh_node_elset_elts (struct MESH Mesh, int node, int elset, int **pelts,
                           int *peltqty)
{
  int i, elt;

  (*pelts) = ut_realloc_1d_int (*pelts, Mesh.NodeElts[node][0]);
  (*peltqty) = 0;

  for (i = 0; i < Mesh.NodeElts[node][0]; i++)
  {
    elt = Mesh.NodeElts[node][i + 1];
    if (Mesh.EltElset[elt] == elset)
      (*pelts)[(*peltqty)++] = elt;
  }

  (*pelts) = ut_realloc_1d_int (*pelts, *peltqty);

  return;
}

void
neut_mesh_node_elsets_elsetelts (struct MESH Mesh, int node, int **pelsets,
                                 int *pelsetqty, int ***pelsetelts,
                                 int **pelseteltqty)
{
  int i, elt, elset, pos;

  (*pelsetqty) = 0;
  ut_free_1d_int (pelsets);

  for (i = 0; i < Mesh.NodeElts[node][0]; i++)
  {
    elt = Mesh.NodeElts[node][i + 1];
    elset = Mesh.EltElset[elt];

    pos = ut_array_1d_int_eltpos (*pelsets, *pelsetqty, elset);

    if (pos != -1)
    {
      (*pelseteltqty)[pos]++;
      (*pelsetelts)[pos] =
        ut_realloc_1d_int ((*pelsetelts)[pos], (*pelseteltqty)[pos]);
      (*pelsetelts)[pos][(*pelseteltqty)[pos] - 1] = elt;
    }
    else
    {
      (*pelsetqty)++;
      (*pelsets) = ut_realloc_1d_int (*pelsets, *pelsetqty);
      (*pelsets)[*pelsetqty - 1] = elset;
      (*pelseteltqty) = ut_realloc_1d_int (*pelseteltqty, *pelsetqty);
      (*pelseteltqty)[(*pelsetqty) - 1] = 1;
      (*pelsetelts) = ut_realloc_2d_int_addline (*pelsetelts, *pelsetqty, 1);
      (*pelsetelts)[(*pelsetqty) - 1][0] = elt;
    }
  }

  return;
}

void
neut_mesh_nodes (struct MESH Mesh, int **pnodes, int *pnodeqty)
{
  int i;
  int elsetqty = Mesh.ElsetQty;
  int *elsets = ut_alloc_1d_int (elsetqty);

  for (i = 0; i < elsetqty; i++)
    elsets[i] = i + 1;

  neut_mesh_elsets_nodes (Mesh, elsets, elsetqty, pnodes, pnodeqty);

  ut_free_1d_int (&elsets);

  return;
}

void
neut_mesh_nodemin (struct MESH Mesh, int *pmin)
{
  int eltnodeqty;

  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  (*pmin) = ut_array_2d_int_min (Mesh.EltNodes + 1, Mesh.EltQty, eltnodeqty);

  return;
}

void
neut_mesh_nodemax (struct MESH Mesh, int *pmax)
{
  int eltnodeqty;

  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  (*pmax) = ut_array_2d_int_max (Mesh.EltNodes + 1, Mesh.EltQty, eltnodeqty);

  return;
}

int
neut_mesh_elt2d_elset3d_elt3d (struct MESH Mesh2D, int elt2d,
                               struct MESH Mesh3D, int elset3d, int *pelt3d)
{
  int i, qty, *tmp = NULL;

  neut_mesh_elt2d_elts3d (Mesh2D, elt2d, Mesh3D, &tmp, &qty);

  (*pelt3d) = -1;
  for (i = 0; i < qty; i++)
    if (Mesh3D.EltElset[tmp[i]] == elset3d)
    {
      (*pelt3d) = tmp[i];
      break;
    }

  ut_free_1d_int (&tmp);

  return (*pelt3d > 0) ? 0 : -1;
}

void
neut_mesh_elset2d_elset3d_elts3d (struct MESH Mesh2D, int elset2d,
                                  struct MESH Mesh3D, int elset3d,
                                  int **pelt3ds, int *pelt3dqty)
{
  int i, elt2d, elt3d;

  (*pelt3dqty) = 0;
  for (i = 1; i <= Mesh2D.Elsets[elset2d][0]; i++)
  {
    elt2d = Mesh2D.Elsets[elset2d][i];
    neut_mesh_elt2d_elset3d_elt3d (Mesh2D, elt2d, Mesh3D, elset3d, &elt3d);
    if (elt3d > 0)
      ut_array_1d_int_list_addval (pelt3ds, pelt3dqty, elt3d);
  }

  return;
}

void
neut_mesh_elsets_elts (struct MESH Mesh, int *elsets, int elsetqty,
                       int **pelts, int *peltqty)
{
  int i, elset;

  (*peltqty) = 0;
  ut_free_1d_int (pelts);

  for (i = 0; i < elsetqty; i++)
  {
    elset = elsets[i];
    (*pelts) = ut_realloc_1d_int (*pelts, (*peltqty) + Mesh.Elsets[elset][0]);
    ut_array_1d_int_memcpy (Mesh.Elsets[elset] + 1, Mesh.Elsets[elset][0],
                            (*pelts) + (*peltqty));
    (*peltqty) += Mesh.Elsets[elset][0];
  }

  ut_array_1d_int_sort_uniq (*pelts, *peltqty, peltqty);

  return;
}

int
neut_mesh_elt1d_elsets2d_elts2d (struct MESH Mesh1D, int elt1d,
                                 struct MESH Mesh2D, int *elsets2d,
                                 int elset2dqty, int **pelts2d,
                                 int *pelt2dqty)
{
  int i, qty = 0, *tmp = NULL;

  neut_mesh_elt1d_elts2d (Mesh1D, elt1d, Mesh2D, &tmp, &qty);

  (*pelt2dqty) = 0;
  for (i = 0; i < qty; i++)
    if (ut_array_1d_int_eltpos (elsets2d, elset2dqty, Mesh2D.EltElset[tmp[i]])
        != -1)
      ut_array_1d_int_list_addval (pelts2d, pelt2dqty, tmp[i]);

  ut_free_1d_int (&tmp);

  return 0;
}

int
neut_mesh_elt2d_nodes_ordering (struct MESH Mesh, int elt2d, int *nodes,
                                int NodeQty)
{
  int pos, status = 0;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, 2, 1);

  if (Mesh.Dimension != 2 || NodeQty != 2)
    ut_print_neperbug ();

  pos = ut_array_1d_int_eltpos (Mesh.EltNodes[elt2d], eltnodeqty, nodes[0]);
  if (pos == -1)
    ut_print_neperbug ();

  if (Mesh.EltNodes[elt2d][ut_array_rotpos (0, eltnodeqty - 1, pos, 1)] ==
      nodes[1])
    status = 1;
  else if (Mesh.
           EltNodes[elt2d][ut_array_rotpos (0, eltnodeqty - 1, pos, -1)] ==
           nodes[1])
    status = -1;
  else
    ut_print_neperbug ();

  return status;
}

void
neut_mesh_fasets_bound (struct TESS Tess, struct NODES Nodes,
                        struct MESH Mesh2D, struct MESH Mesh3D,
                        char *fasetlist, struct BOUNDARY *pBound)
{
  int i, fasetqty;
  char **fasets = NULL;

  neut_boundary_set_zero (pBound);

  ut_list_break (fasetlist, NEUT_SEP_NODEP, &fasets, &fasetqty);

  (*pBound).BoundQty = fasetqty;
  (*pBound).BoundEltQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundElts = ut_alloc_2d_pint ((*pBound).BoundQty + 1, 2);
  (*pBound).BoundEltFacets = ut_alloc_2d_pint ((*pBound).BoundQty + 1, 2);
  (*pBound).BoundDom = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  (*pBound).BoundSeed = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  (*pBound).BoundNames = ut_alloc_2d_pchar ((*pBound).BoundQty + 1, 2);

  int EltQty, *elts = NULL, *eltfasets = NULL;
  for (i = 1; i <= fasetqty; i++)
  {
    (*pBound).BoundDom[i][0] = -1;
    neut_mesh_domface_elts3d (Tess, Mesh2D, Mesh3D, Nodes, fasets[i - 1],
                              &elts, &eltfasets, &EltQty);

    (*pBound).BoundEltQty[i] = EltQty;
    (*pBound).BoundElts[i][0] = ut_alloc_1d_int (EltQty + 1);
    (*pBound).BoundEltFacets[i][0] = ut_alloc_1d_int (EltQty + 1);
    ut_array_1d_int_memcpy (elts, EltQty, (*pBound).BoundElts[i][0] + 1);
    ut_array_1d_int_memcpy (eltfasets, EltQty,
                            (*pBound).BoundEltFacets[i][0] + 1);
    ut_string_string (fasets[i - 1], (*pBound).BoundNames[i]);

    ut_free_1d_int (&elts);
    ut_free_1d_int (&eltfasets);
  }

  ut_free_2d_char (&fasets, fasetqty);

  return;
}

void
neut_mesh_domface_elts (struct TESS Tess, struct MESH Mesh2D, char *domface,
                        int **pelts, int *peltqty)
{
  int i, j, domfaceid, id, status;

  status = neut_tess_domface_label_id (Tess, domface, &domfaceid);

  if (status != 0)
    abort ();

  id = 0;
  (*peltqty) = 0;
  for (i = 1; i <= Tess.DomTessFaceQty[domfaceid]; i++)
  {
    (*peltqty) += Mesh2D.Elsets[Tess.DomTessFaceNb[domfaceid][i]][0];
    (*pelts) = ut_realloc_1d_int (*pelts, *peltqty);
    for (j = 1; j <= Mesh2D.Elsets[Tess.DomTessFaceNb[domfaceid][i]][0]; j++)
      (*pelts)[id++] = Mesh2D.Elsets[Tess.DomTessFaceNb[domfaceid][i]][j];
  }

  return;
}

void
neut_mesh_domface_elts3d (struct TESS Tess, struct MESH Mesh2D,
                          struct MESH Mesh3D, struct NODES Nodes, char *faset,
                          int **pelts, int **peltfasets, int *peltqty)
{
  int i, j, elt2d, elt3dqty, id, status, surf, surfori;
  int *elt3d = NULL;
  double *n = ut_alloc_1d (3);
  double *n0 = ut_alloc_1d (3);

  status = neut_tess_domface_label_id (Tess, faset, &id);

  if (status != 0)
    abort ();

  // calculating number of elements
  (*peltqty) = 0;
  for (i = 1; i <= Tess.DomTessFaceQty[id]; i++)
    (*peltqty) += Mesh2D.Elsets[Tess.DomTessFaceNb[id][i]][0];
  (*pelts) = ut_alloc_1d_int (*peltqty);
  (*peltfasets) = ut_alloc_1d_int (*peltqty);
  (*peltqty) = 0;

  ut_array_1d_memcpy (Tess.DomFaceEq[id] + 1, 3, n0);
  ut_array_1d_scale (n0, 3, -1);

  for (i = 1; i <= Tess.DomTessFaceQty[id]; i++)
    for (j = 1; j <= Mesh2D.Elsets[Tess.DomTessFaceNb[id][i]][0]; j++)
    {
      elt2d = Mesh2D.Elsets[Tess.DomTessFaceNb[id][i]][j];

      neut_mesh_elt2d_elts3d (Mesh2D, elt2d, Mesh3D, &elt3d, &elt3dqty);

      if (elt3dqty != 1)
      {
        printf ("Nodes:\n");
        neut_debug_nodes (stdout, Nodes);
        printf ("Mesh2D:\n");
        neut_debug_mesh (stdout, Mesh2D);
        printf ("Mesh3D:\n");
        neut_debug_mesh (stdout, Mesh3D);

        printf ("elt2d = %d\n", elt2d);
        printf ("of nodes ");
        ut_array_1d_int_fprintf (stdout, Mesh2D.EltNodes[elt2d], 3, "%d");

        printf ("elt3dqty != 1 ( = %d)\n", elt3dqty);
        ut_array_1d_int_fprintf (stdout, elt3d, elt3dqty, "%d");
        abort ();
      }

      /*
         neut_mesh_elt_normal (Mesh2D, Nodes, elt2d, n);
         res = ut_vector_scalprod (n0, n);
       */
      neut_elt_nodes_facet (3, Mesh3D.EltNodes[elt3d[0]],
                            Mesh2D.EltNodes[elt2d], &surf, &surfori);

      (*pelts)[(*peltqty)] = elt3d[0];
      (*peltfasets)[(*peltqty)] = surf;
      (*peltqty)++;
    }

  ut_free_1d (&n);
  ut_free_1d (&n0);
  ut_free_1d_int (&elt3d);

  return;
}

int
neut_mesh_elset_elt_insidefacets (struct MESH Mesh1D, struct MESH Mesh2D,
                                  int elset, int elt, int **pinsidefacets,
                                  int *pinsidefacetqty)
{
  int i, j, comelt, status;
  int *nodepair = ut_alloc_1d_int (2);

  if (Mesh2D.EltElset[elt] != elset)
    abort ();

  (*pinsidefacetqty) = 0;

  for (i = 1; i <= 3; i++)
  {
    for (j = 0; j < 2; j++)
      nodepair[j] = Mesh2D.EltNodes[elt][ut_array_rotpos (0, 2, i - 1, j)];

    status = neut_mesh_nodes_comelt (Mesh1D, nodepair, 2, &comelt);
    if (status == -1)
      ut_array_1d_int_list_addval (pinsidefacets, pinsidefacetqty, i);
  }

  ut_free_1d_int (&nodepair);

  return 0;
}

void
neut_mesh_nodeqty (struct MESH Mesh, int *pnodeqty)
{
  int i;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);
  int *nodes = ut_alloc_1d_int (Mesh.EltQty * eltnodeqty);

  (*pnodeqty) = Mesh.EltQty * eltnodeqty;
  for (i = 1; i <= Mesh.EltQty; i++)
    ut_array_1d_int_memcpy (Mesh.EltNodes[i], eltnodeqty,
                            nodes + (i - 1) * eltnodeqty);
  ut_array_1d_int_sort_uniq (nodes, *pnodeqty, pnodeqty);

  ut_free_1d_int (&nodes);

  return;
}

int
neut_mesh_elt1d_isembedded (struct MESH Mesh3D, struct MESH Mesh1D, int elt1d)
{
  int i, j, id, node;
  int elt, EltQty, *elts = NULL;
  int facetnodeqty, *facetnodes = NULL;

  neut_mesh_nodes_comelts (Mesh3D, Mesh1D.EltNodes[elt1d], 2, &elts, &EltQty);

  facetnodeqty = 2 * EltQty;
  facetnodes = ut_alloc_1d_int (facetnodeqty);

  id = 0;
  for (i = 0; i < EltQty; i++)
  {
    elt = elts[i];

    for (j = 0; j < 4; j++)
    {
      node = Mesh3D.EltNodes[elt][j];
      if (ut_array_1d_int_eltpos (Mesh1D.EltNodes[elt1d], 2, node) == -1)
        facetnodes[id++] = node;
    }
  }

  ut_array_1d_int_sort_uniq (facetnodes, facetnodeqty, &facetnodeqty);

  ut_free_1d_int (&elts);
  ut_free_1d_int (&facetnodes);

  return EltQty == facetnodeqty;
}

int
neut_mesh_node_domtype (struct TESS Tess, struct MESH Mesh0D,
                        struct MESH Mesh1D, struct MESH Mesh2D,
                        struct MESH Mesh3D, int node, int *pdomtype)
{
  struct MESH **Mesh = (struct MESH **) calloc (sizeof (struct MESH *), 4);

  Mesh[0] = &Mesh0D;
  Mesh[1] = &Mesh1D;
  Mesh[2] = &Mesh2D;
  Mesh[3] = &Mesh3D;

  int meshdim = neut_mesh_array_dim (*Mesh);
  int dim, elset;

  dim = neut_mesh_node_dim_min (Mesh0D, Mesh1D, Mesh2D, Mesh3D, node);

  if (dim == meshdim)
    (*pdomtype) = -1;

  else
  {
    neut_mesh_node_elset (*(Mesh[dim]), node, &elset);

    if (dim == 2)
      (*pdomtype) = (Tess.FaceDom[elset][0] == 2) ? 2 : -1;
    else if (dim == 1)
      (*pdomtype) = Tess.EdgeDom[elset][0];
    else if (dim == 0)
      (*pdomtype) = Tess.VerDom[elset][0];
  }

  free (Mesh);

  return 0;
}

void
neut_mesh_elt_domface (struct TESS Tess, struct MESH Mesh2D, int elt,
                       int *pdomface)
{
  if (!Mesh2D.EltElset)
    abort ();

  (*pdomface) =
    Tess.FaceDom[Mesh2D.EltElset[elt]][0] ==
    2 ? Tess.FaceDom[Mesh2D.EltElset[elt]][1] : -1;

  return;
}

void
neut_mesh_order1nodes_order2node (struct MESH Mesh, int node1, int node2,
                                  int *pnode)
{
  if (!Mesh.NodeElts)
    ut_print_message (2, 0, "neut_mesh_nodes_comelts requires NodeElts\n");

  int pos, pos1, pos2;
  int *elts = NULL, EltQty, elt;
  int *nodes = ut_alloc_1d_int (2);
  nodes[0] = node1;
  nodes[1] = node2;

  int nodeqty_o1, nodeqty_tot;
  int **fir = NULL, **sec = NULL;

  neut_elt_orderarrays (Mesh.EltType, Mesh.Dimension, &fir, &nodeqty_tot,
                        &sec, &nodeqty_o1);

  neut_mesh_nodes_comelts (Mesh, nodes, 2, &elts, &EltQty);

  if (EltQty == 0)
    (*pnode) = -1;

  else
  {
    elt = elts[0];

    pos1 = ut_array_1d_int_eltpos (Mesh.EltNodes[elt], nodeqty_o1, nodes[0]);
    pos2 = ut_array_1d_int_eltpos (Mesh.EltNodes[elt], nodeqty_o1, nodes[1]);
    pos = sec[pos1][pos2];
    (*pnode) = Mesh.EltNodes[elt][pos];
  }

  ut_free_1d_int (&nodes);
  ut_free_1d_int (&elts);

  return;
}
