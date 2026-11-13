/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

void
nem_interface_dup_pre (struct TESS Tess, int *intertypes, int **pvers, int *pverqty,
                       int **pedges, int *pedgeqty, int **pfaces, int *pfaceqty)
{
  int i;

  (*pfaceqty) = 0;
  (*pedgeqty) = 0;

  if (Tess.Dim == 3)
  {
    for (i = 1; i <= Tess.FaceQty; i++)
      if (intertypes[i] != NEM_INTERFACE_CONTINUOUS)
        ut_array_1d_int_list_addval_nocheck (pfaces, pfaceqty, i);

    neut_tess_faces_edges (Tess, *pfaces, *pfaceqty, pedges, pedgeqty);
    neut_tess_faces_vers (Tess, *pfaces, *pfaceqty, pvers, pverqty);
  }
  else
  {
    for (i = 1; i <= Tess.EdgeQty; i++)
      if (intertypes[i] != NEM_INTERFACE_CONTINUOUS)
        ut_array_1d_int_list_addval_nocheck (pedges, pedgeqty, i);

    neut_tess_edges_vers (Tess, *pedges, *pedgeqty, pvers, pverqty);
  }

  return;
}

// The seeds the nodes are duplicated for are given by
// nem_interface_dup_seeds, and the nodes are duplicated once per group of
// seeds joined by continuous interfaces (nem_interface_dup_seedgroups).
// By using "interfaces", we can deal with 2D and 3D.
void
nem_interface_dup_ver (struct TESS Tess, int *vers, int verqty, int *inters,
                       int interqty, struct NODES *pNodes, struct MESH *Mesh, int ***pnodeseeds)
{
  int i, ver;
  int seedqty, *seeds = NULL, groupqty, **groups = NULL;
  int nodeqty, *nodes = NULL;

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];

    nem_interface_dup_seeds (Tess, inters, interqty, 0, ver, &seeds, &seedqty);
    nem_interface_dup_seedgroups (Tess, inters, interqty, 0, ver, seeds, seedqty, &groups,
                                  &groupqty);

    neut_mesh_elset_nodes (Mesh[0], ver, &nodes, &nodeqty);
    nem_interface_duplicate (groups, groupqty, nodes, nodeqty, pNodes, Mesh, pnodeseeds);
    ut_free_2d_int (&groups, groupqty);
  }

  ut_free_1d_int (&nodes);
  ut_free_1d_int (&seeds);

  return;
}

void
nem_interface_dup_edge (struct TESS Tess, int *edges, int edgeqty, int *inters,
                        int interqty, struct NODES *pNodes, struct MESH *Mesh, int ***pnodeseeds)
{
  int i, edge;
  int seedqty, *seeds = NULL, groupqty, **groups = NULL;
  int NodeQty, *nodes = NULL;

  for (i = 0; i < edgeqty; i++)
  {
    edge = edges[i];

    nem_interface_dup_seeds (Tess, inters, interqty, 1, edge, &seeds, &seedqty);
    nem_interface_dup_seedgroups (Tess, inters, interqty, 1, edge, seeds, seedqty, &groups,
                                  &groupqty);

    neut_mesh_elset_bodynodes_1d (Tess, Mesh[0], Mesh[1], edge, &nodes, &NodeQty);
    nem_interface_duplicate (groups, groupqty, nodes, NodeQty, pNodes, Mesh, pnodeseeds);
    ut_free_2d_int (&groups, groupqty);
  }

  ut_free_1d_int (&nodes);
  ut_free_1d_int (&seeds);

  return;
}

void
nem_interface_dup_face (struct TESS Tess, int *faces, int faceqty, int *inters,
                        int interqty, struct NODES *pNodes, struct MESH *Mesh, int ***pnodeseeds)
{
  int i, face;
  int seedqty, *seeds = NULL, groupqty, **groups = NULL;
  int NodeQty, *nodes = NULL;

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    nem_interface_dup_seeds (Tess, inters, interqty, 2, face, &seeds, &seedqty);
    nem_interface_dup_seedgroups (Tess, inters, interqty, 2, face, seeds, seedqty, &groups,
                                  &groupqty);

    neut_mesh_elset_bodynodes_2d (Tess, Mesh[1], Mesh[2], face, &nodes, &NodeQty);
    nem_interface_duplicate (groups, groupqty, nodes, NodeQty, pNodes, Mesh, pnodeseeds);
    ut_free_2d_int (&groups, groupqty);
  }

  ut_free_1d_int (&nodes);
  ut_free_1d_int (&seeds);

  return;
}

void
nem_interface_dup_boundelts_3d (struct TESS Tess, int *faces, int faceqty,
                                struct NODES Nodes, int **nodeseeds, struct MESH *Mesh,
                                struct BOUNDARY *pBound)
{
  int i, j, k, l, elt2d, elt3d, surf, surfori, face;
  int *nodes2d = ut_alloc_1d_int (3);
  int NodeQty, *nodes = NULL;

  (*pBound).BoundQty = Tess.FaceQty;
  (*pBound).BoundDom = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  ut_array_2d_int_memcpy (Tess.FaceDom + 1, (*pBound).BoundQty, 2,
                          (*pBound).BoundDom + 1);
  (*pBound).BoundSeed = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  ut_array_2d_int_memcpy (Tess.FacePoly + 1, (*pBound).BoundQty, 2,
                          (*pBound).BoundSeed + 1);
  (*pBound).BoundEltQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundElts = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);
  (*pBound).BoundEltFacets = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);
  (*pBound).BoundNodeQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundNodes = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    if (Tess.FaceDom[face][0] == -1 && !neut_tess_face_isperslave (Tess, face))
    {
      ut_array_1d_int_sort ((*pBound).BoundSeed[face], 2);

      (*pBound).BoundEltQty[face] = Mesh[2].Elsets[face][0];
      (*pBound).BoundElts[face] =
        ut_alloc_2d_int (2, (*pBound).BoundEltQty[face] + 1);
      (*pBound).BoundEltFacets[face] =
        ut_alloc_2d_int (2, (*pBound).BoundEltQty[face] + 1);

      // the if statement below has the effect of skipping the periodic
      // (slave) seed of a periodic interface, which has no elements
      for (j = 0; j < 2; j++)
        if ((*pBound).BoundSeed[face][j] <= Tess.CellQty)
          for (k = 1; k <= (*pBound).BoundEltQty[face]; k++)
          {
            elt2d = Mesh[2].Elsets[face][k];

            for (l = 0; l < 3; l++)
            {
              nem_interface_dup_node_seed_copy (Nodes, nodeseeds, Mesh[2].EltNodes[elt2d][l],
                                                (*pBound).BoundSeed[face][j],
                                                nodes2d + l);
              if (nodes2d[l] == -1)
                nodes2d[l] = Mesh[2].EltNodes[elt2d][l];
            }

            nem_interface_dup_nodes_seed_elt (Mesh[3], nodes2d, 3,
                                              (*pBound).BoundSeed[face][j], &elt3d);
            if (elt3d <= 0)
              abort ();
            (*pBound).BoundElts[face][j][k] = elt3d;

            neut_elt_nodes_facet (Mesh[3].EltType, 3, Mesh[3].EltNodes[elt3d], nodes2d,
                                  &surf, &surfori);
            (*pBound).BoundEltFacets[face][j][k] = surf;
          }

      // initializing BoundNodes and BoundNodeQty
      neut_boundary_bound_nodes (Mesh[3], *pBound, face, 0, &nodes, &NodeQty);

      if (NodeQty != 3 * (*pBound).BoundEltQty[face])
        abort ();

      (*pBound).BoundNodeQty[face] = NodeQty;
      (*pBound).BoundNodes[face] = ut_alloc_2d_int (2, NodeQty + 1);

      ut_array_1d_int_memcpy (nodes, NodeQty,
                              (*pBound).BoundNodes[face][0] + 1);

      for (j = 1; j <= (*pBound).BoundNodeQty[face]; j++)
        nem_interface_dup_node_seed_copy (Nodes, nodeseeds, (*pBound).BoundNodes[face][0][j],
                                          (*pBound).BoundSeed[face][1],
                                          (*pBound).BoundNodes[face][1] + j);

      ut_free_1d_int (&nodes);
    }
  }

  (*pBound).BoundNames = ut_alloc_3d_char ((*pBound).BoundQty + 1, 2, 100);
  for (i = 1; i <= (*pBound).BoundQty; i++)
    for (j = 0; j < 2; j++)
      sprintf ((*pBound).BoundNames[i][j], "bound%d-cell%d", i,
               (*pBound).BoundSeed[i][j]);

  ut_free_1d_int (&nodes2d);

  return;
}

void
nem_interface_dup_boundelts_2d (struct TESS Tess, int *edges, int edgeqty,
                                struct NODES Nodes, int **nodeseeds,
                                struct MESH *Mesh, struct BOUNDARY *pBound)
{
  int i, j, k, l, elt1d, elt2d, surf, surfori;
  int *nodes1d = ut_alloc_1d_int (3);
  int NodeQty, *nodes = NULL;

  (*pBound).BoundQty = Tess.EdgeQty;
  (*pBound).BoundDom = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  ut_array_2d_int_memcpy (Tess.EdgeDom + 1, (*pBound).BoundQty, 2,
                          (*pBound).BoundDom + 1);
  (*pBound).BoundSeed = ut_alloc_2d_int ((*pBound).BoundQty + 1, 2);
  for (i = 1; i <= (*pBound).BoundQty; i++)
  {
    ut_array_1d_int_memcpy (Tess.EdgeFaceNb[i], Tess.EdgeFaceQty[i],
                            (*pBound).BoundSeed[i]);
    if (Tess.EdgeFaceQty[i] == 1)
      (*pBound).BoundSeed[i][1] = -1;
  }
  (*pBound).BoundEltQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundElts = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);
  (*pBound).BoundEltFacets = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);
  (*pBound).BoundNodeQty = ut_alloc_1d_int ((*pBound).BoundQty + 1);
  (*pBound).BoundNodes = ut_alloc_1d_ppint ((*pBound).BoundQty + 1);

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeDom[i][0] == -1 && !neut_tess_edge_isperslave (Tess, i)
        && ut_array_1d_int_eltpos (edges, edgeqty, i) != -1)
    {
      ut_array_1d_int_sort ((*pBound).BoundSeed[i], 2);

      (*pBound).BoundEltQty[i] = Mesh[1].Elsets[i][0];
      (*pBound).BoundElts[i] =
        ut_alloc_2d_int (2, (*pBound).BoundEltQty[i] + 1);
      (*pBound).BoundEltFacets[i] =
        ut_alloc_2d_int (2, (*pBound).BoundEltQty[i] + 1);

      // the if statement below has the effect of skipping the slave
      // edges of a periodic tessellation
      for (j = 0; j < 2; j++)
        if ((*pBound).BoundSeed[i][j] <= Tess.CellQty)
          for (k = 1; k <= (*pBound).BoundEltQty[i]; k++)
          {
            elt1d = Mesh[1].Elsets[i][k];

            for (l = 0; l < 2; l++)
            {
              nem_interface_dup_node_seed_copy (Nodes, nodeseeds, Mesh[1].EltNodes[elt1d][l],
                                                (*pBound).BoundSeed[i][j],
                                                nodes1d + l);
              if (nodes1d[l] == -1)
                nodes1d[l] = Mesh[1].EltNodes[elt1d][l];
            }

            nem_interface_dup_nodes_seed_elt (Mesh[2], nodes1d, 2, (*pBound).BoundSeed[i][j],
                                              &elt2d);
            if (elt2d <= 0)
              abort ();
            (*pBound).BoundElts[i][j][k] = elt2d;

            neut_elt_nodes_facet (Mesh[2].EltType, 2, Mesh[2].EltNodes[elt2d], nodes1d,
                                  &surf, &surfori);
            (*pBound).BoundEltFacets[i][j][k] = surf;
          }

      neut_boundary_bound_nodes (Mesh[2], *pBound, i, 0, &nodes, &NodeQty);
      if (NodeQty != (*pBound).BoundEltQty[i] + 1)
        abort ();

      (*pBound).BoundNodeQty[i] = NodeQty;
      (*pBound).BoundNodes[i] =
        ut_alloc_2d_int (2, (*pBound).BoundEltQty[i] + 2);

      ut_array_1d_int_memcpy (nodes, NodeQty, (*pBound).BoundNodes[i][0] + 1);

      // (the 2 nodes can be the same at the end of an interface that meets
      // continuous interfaces only)
      for (j = 1; j <= (*pBound).BoundEltQty[i] + 1; j++)
        nem_interface_dup_node_seed_copy (Nodes, nodeseeds, (*pBound).BoundNodes[i][0][j],
                                          (*pBound).BoundSeed[i][1],
                                          (*pBound).BoundNodes[i][1] + j);

      ut_free_1d_int (&nodes);
    }

  (*pBound).BoundNames = ut_alloc_3d_char ((*pBound).BoundQty + 1, 2, 100);
  for (i = 1; i <= (*pBound).BoundQty; i++)
    for (j = 0; j < 2; j++)
      sprintf ((*pBound).BoundNames[i][j], "bound%d-cell%d", i,
               (*pBound).BoundSeed[i][j]);

  ut_free_1d_int (&nodes1d);

  return;
}

// making sure that all nodes of 1D elts belongs to the same (master,
// or lower-index) cell the edge belongs to.
void
nem_interface_dup_renumber_1d (struct TESS Tess, struct NODES Nodes, int **nodeseeds,
                               struct MESH *Mesh)
{
  int i, j, k, node, dupnode, elt, eltnodeqty;
  int cellqty, *cells = NULL, mastercell;

  eltnodeqty =
    neut_elt_nodeqty (Mesh[1].EltType, Mesh[1].Dimension, Mesh[1].EltOrder);

  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    neut_tess_edge_cells (Tess, i, &cells, &cellqty);
    mastercell = cells[0];

    for (j = 1; j <= Mesh[1].Elsets[i][0]; j++)
    {
      elt = Mesh[1].Elsets[i][j];

      for (k = 0; k < eltnodeqty; k++)
      {
        node = Mesh[1].EltNodes[elt][k];
        nem_interface_dup_node_seed_copy (Nodes, nodeseeds, node, mastercell, &dupnode);
        if (dupnode != -1)
          Mesh[1].EltNodes[elt][k] = dupnode;
      }
    }
  }

  return;
}

// making sure that all nodes of 2D elts belongs to the same (master,
// or lower-index) cell the face belongs to.
void
nem_interface_dup_renumber_2d (struct TESS Tess, struct NODES Nodes, int **nodeseeds,
                               struct MESH *Mesh)
{
  int i, j, k, node, dupnode, elt, eltnodeqty, mastercell;

  eltnodeqty =
    neut_elt_nodeqty (Mesh[2].EltType, Mesh[2].Dimension, Mesh[2].EltOrder);

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    // (not neut_tess_face_cells, which returns the master cell of the
    // periodic seed of a periodic face)
    mastercell = INT_MAX;
    for (j = 0; j < 2; j++)
      if (Tess.FacePoly[i][j] > 0 && Tess.FacePoly[i][j] <= Tess.CellQty)
        mastercell = ut_num_min (mastercell, Tess.FacePoly[i][j]);

    for (j = 1; j <= Mesh[2].Elsets[i][0]; j++)
    {
      elt = Mesh[2].Elsets[i][j];

      for (k = 0; k < eltnodeqty; k++)
      {
        node = Mesh[2].EltNodes[elt][k];
        nem_interface_dup_node_seed_copy (Nodes, nodeseeds, node, mastercell, &dupnode);
        if (dupnode != -1)
          Mesh[2].EltNodes[elt][k] = dupnode;
      }
    }
  }

  return;
}

// After duplication, a periodic master node and its slaves are split
// into copies, each for a group of seeds.  A copy at a position of
// periodicity shift t for a seed of master cell c and periodicity shift
// ts represents the material point of cell c at shift t - ts.  The copies
// that represent a same material point (for one of their seeds) are
// tied by periodicity to the copy at the master position, or to the
// first copy if there is none there.
void
nem_interface_dup_per (struct TESS Tess, struct NODES *pNodes, int **nodeseeds)
{
  int i, j, k, l, m, node, seed, master, root, mnodeqty, *mnodes = NULL;
  int posqty, *pos = NULL;
  int copyqty, *copies = NULL, **copyshifts = NULL, *copyroots = NULL;
  int keyqty, *keycopies = NULL, **keys = NULL;

  if ((*pNodes).PerNodeQty == 0)
    return;

  neut_nodes_permasters (*pNodes, &mnodes, &mnodeqty);

  for (i = 0; i < mnodeqty; i++)
  {
    master = mnodes[i];

    // master node not on a (discontinuous) interface
    if (!nodeseeds[master])
      continue;

    posqty = 1 + (*pNodes).PerNodeSlaveQty[master];
    pos = ut_alloc_1d_int (posqty);
    pos[0] = master;
    ut_array_1d_int_memcpy ((*pNodes).PerNodeSlaveNb[master] + 1, posqty - 1, pos + 1);

    copyqty = 0;
    keyqty = 0;
    for (j = 0; j < posqty; j++)
      for (k = 0; k <= (*pNodes).DupNodeSlaveQty[pos[j]]; k++)
      {
        node = (k == 0) ? pos[j] : (*pNodes).DupNodeSlaveNb[pos[j]][k];

        copies = ut_realloc_1d_int (copies, copyqty + 1);
        copyshifts = ut_realloc_2d_int_addline (copyshifts, copyqty + 1, 3);
        copies[copyqty] = node;
        if (j > 0)
          ut_array_1d_int_memcpy ((*pNodes).PerNodeShift[pos[j]], 3, copyshifts[copyqty]);

        for (l = 1; nodeseeds[node] && l <= nodeseeds[node][0]; l++)
        {
          seed = nodeseeds[node][l];

          keycopies = ut_realloc_1d_int (keycopies, keyqty + 1);
          keys = ut_realloc_2d_int_addline (keys, keyqty + 1, 4);
          keycopies[keyqty] = copyqty;
          neut_tess_seed_masterseed (Tess, seed, keys[keyqty]);
          ut_array_1d_int_memcpy (copyshifts[copyqty], 3, keys[keyqty] + 1);
          if (seed > Tess.CellQty)
            ut_array_1d_int_sub (Tess.PerSeedShift[seed], keys[keyqty] + 1, 3,
                                 keys[keyqty] + 1);
          keyqty++;
        }

        copyqty++;
      }

    // union of the copies of the same keys (roots are the first copies)
    copyroots = ut_alloc_1d_int (copyqty);
    for (j = 0; j < copyqty; j++)
      copyroots[j] = j;

    for (j = 0; j < keyqty; j++)
      for (l = 0; l < j; l++)
        if (ut_array_1d_int_equal (keys[l], 4, keys[j], 4))
        {
          int r1 = keycopies[l], r2 = keycopies[j];
          while (copyroots[r1] != r1)
            r1 = copyroots[r1];
          while (copyroots[r2] != r2)
            r2 = copyroots[r2];
          copyroots[ut_num_max_int (r1, r2)] = ut_num_min_int (r1, r2);
        }

    for (j = 0; j < copyqty; j++)
    {
      for (root = j; copyroots[root] != root; root = copyroots[root]);

      if (root == j)
      {
        (*pNodes).PerNodeMaster[copies[j]] = 0;
        ut_array_1d_int_zero ((*pNodes).PerNodeShift[copies[j]], 3);
      }
      else
      {
        // two copies of a material point cannot be at the same position
        for (m = 0; m < j; m++)
          if (ut_array_1d_int_equal (copyshifts[m], 3, copyshifts[j], 3))
          {
            int root2;
            for (root2 = m; copyroots[root2] != root2; root2 = copyroots[root2]);
            if (root2 == root)
              ut_print_neperbug ();
          }

        (*pNodes).PerNodeMaster[copies[j]] = copies[root];
        ut_array_1d_int_sub (copyshifts[root], copyshifts[j], 3,
                             (*pNodes).PerNodeShift[copies[j]]);
      }
    }

    ut_free_1d_int (&pos);
    ut_free_1d_int (&copies);
    ut_free_1d_int (&copyroots);
    ut_free_1d_int (&keycopies);
    ut_free_2d_int (&copyshifts, copyqty);
    ut_free_2d_int (&keys, keyqty);
  }

  (*pNodes).PerNodeQty = 0;
  (*pNodes).PerNodeNb = ut_realloc_1d_int ((*pNodes).PerNodeNb, (*pNodes).NodeQty + 1);
  for (i = 1; i <= (*pNodes).NodeQty; i++)
    if ((*pNodes).PerNodeMaster[i])
      (*pNodes).PerNodeNb[++(*pNodes).PerNodeQty] = i;
  (*pNodes).PerNodeNb = ut_realloc_1d_int ((*pNodes).PerNodeNb, (*pNodes).PerNodeQty + 1);

  neut_nodes_init_nodeslave (pNodes);

  ut_free_1d_int (&mnodes);

  return;
}

// Recording the seeds of the nodes of the entities that have not been
// duplicated (all seeds of the entity, which are joined by continuous
// interfaces), as needed for periodicity
void
nem_interface_dup_nodeseeds_complete (struct TESS Tess, struct MESH *Mesh,
                                      int **nodeseeds)
{
  int i, j, d, id, entqty, seedqty, *seeds = NULL, nodeqty, *nodes = NULL;

  for (d = 0; d < Tess.Dim; d++)
  {
    entqty = (d == 0) ? Tess.VerQty : (d == 1) ? Tess.EdgeQty : Tess.FaceQty;

    for (id = 1; id <= entqty; id++)
    {
      if (d == 0)
        neut_mesh_elset_nodes (Mesh[0], id, &nodes, &nodeqty);
      else if (d == 1)
        neut_mesh_elset_bodynodes_1d (Tess, Mesh[0], Mesh[1], id, &nodes, &nodeqty);
      else
        neut_mesh_elset_bodynodes_2d (Tess, Mesh[1], Mesh[2], id, &nodes, &nodeqty);

      seeds = NULL;
      for (j = 0; j < nodeqty; j++)
        if (!nodeseeds[nodes[j]])
        {
          if (!seeds)
          {
            if (d == 0)
              neut_tess_ver_seeds_positive (Tess, id, &seeds, &seedqty);
            else if (d == 1)
              neut_tess_edge_seeds_positive (Tess, id, &seeds, &seedqty);
            else
              neut_tess_face_seeds_positive (Tess, id, &seeds, &seedqty);
          }

          nodeseeds[nodes[j]] = ut_alloc_1d_int (seedqty + 1);
          nodeseeds[nodes[j]][0] = seedqty;
          for (i = 0; i < seedqty; i++)
            nodeseeds[nodes[j]][i + 1] = seeds[i];
        }

      ut_free_1d_int (&seeds);
      ut_free_1d_int (&nodes);
    }
  }

  return;
}
