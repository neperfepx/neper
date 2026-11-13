/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

// The nodes are duplicated once per group of seeds, groups[i][0] being
// the number of seeds and groups[i][1...] the seeds (the first seed being
// recorded as DupNodeSeed).  The seeds of the nodes are recorded in
// nodeseeds.
void
nem_interface_duplicate (int **groups, int groupqty, int *nodes, int nodeqty,
                         struct NODES *pNodes, struct MESH *Mesh, int ***pnodeseeds)
{
  int i, j, k, l, node, newnode, eltnodeqty, seed;
  int EltQty, *elts = NULL;
  int dim = neut_mesh_array_dim (Mesh);

  eltnodeqty =
    neut_elt_nodeqty (Mesh[dim].EltType, Mesh[dim].Dimension,
                      Mesh[dim].EltOrder);

  if (!(*pNodes).DupNodeSeed)
    (*pNodes).DupNodeSeed = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  if (!(*pNodes).DupNodeMaster)
    (*pNodes).DupNodeMaster = ut_alloc_1d_int ((*pNodes).NodeQty + 1);

  for (j = 0; j < nodeqty; j++)
  {
    node = nodes[j];

    // attaching master dup node to the first group.
    // slave dup nodes will be attached to other groups.
    (*pNodes).DupNodeSeed[node] = groups[0][1];
    (*pnodeseeds)[node] = ut_alloc_1d_int (groups[0][0] + 1);
    ut_array_1d_int_memcpy (groups[0], groups[0][0] + 1, (*pnodeseeds)[node]);

    // Starting from 1: the first group is skipped.
    for (i = 1; i < groupqty; i++)
    {
      newnode =
        neut_nodes_addnode (pNodes, (*pNodes).NodeCoo[node],
                            (*pNodes).NodeCl ? (*pNodes).NodeCl[node] : -1);

      (*pNodes).DupNodeQty++;
      (*pNodes).DupNodeNb =
        ut_realloc_1d_int ((*pNodes).DupNodeNb, (*pNodes).DupNodeQty + 1);
      (*pNodes).DupNodeNb[(*pNodes).DupNodeQty] = newnode;

      (*pNodes).DupNodeMaster =
        ut_realloc_1d_int ((*pNodes).DupNodeMaster, (*pNodes).NodeQty + 1);
      (*pNodes).DupNodeMaster[(*pNodes).NodeQty] = node;

      (*pNodes).DupNodeSeed =
        ut_realloc_1d_int ((*pNodes).DupNodeSeed, (*pNodes).NodeQty + 1);
      (*pNodes).DupNodeSeed[(*pNodes).NodeQty] = groups[i][1];

      (*pnodeseeds) = ut_realloc_1d_pint (*pnodeseeds, (*pNodes).NodeQty + 1);
      (*pnodeseeds)[newnode] = ut_alloc_1d_int (groups[i][0] + 1);
      ut_array_1d_int_memcpy (groups[i], groups[i][0] + 1, (*pnodeseeds)[newnode]);

      // Updating node numbers in the elements of the upper-dimension mesh
      // (there are none for a periodic (slave) seed, i.e. a seed > CellQty)
      for (l = 1; l <= groups[i][0]; l++)
      {
        seed = groups[i][l];

        neut_mesh_node_elset_elts (Mesh[dim], node, seed, &elts, &EltQty);

        for (k = 0; k < EltQty; k++)
          ut_array_1d_int_findnreplace (Mesh[dim].EltNodes[elts[k]], eltnodeqty,
                                        node, newnode);
      }
    }
  }

  ut_free_1d_int (&elts);

  return;
}

// The nodes of an entity (vertex, edge or face) are duplicated for its
// cells and, in the case of a periodic tessellation, for the periodic
// (slave) seeds of the master interfaces it belongs to.  The copies of
// the latter carry the slave side of the master interfaces (for
// cohesive elements) and are tied by periodicity to the nodes of the
// slave interfaces.  The order of the cells is preserved, and periodic
// seeds come last.
void
nem_interface_dup_seeds (struct TESS Tess, int *inters, int interqty,
                         int dim, int id, int **pseeds, int *pseedqty)
{
  int i, j, inter, entinterqty, *entinters = NULL;
  int seedqty, *seeds = NULL;
  int perseedqty = 0, *perseeds = NULL;

  ut_free_1d_int (pseeds);

  if (dim == 0)
    neut_tess_ver_seeds_positive (Tess, id, pseeds, pseedqty);
  else if (dim == 1)
    neut_tess_edge_seeds_positive (Tess, id, pseeds, pseedqty);
  else
    neut_tess_face_seeds_positive (Tess, id, pseeds, pseedqty);

  for (i = 0; i < *pseedqty; i++)
    if ((*pseeds)[i] > Tess.CellQty)
    {
      (*pseedqty) -= ut_array_1d_int_rmelt (*pseeds, *pseedqty, (*pseeds)[i], 1);
      i--;
    }

  if (dim == 0)
    neut_tess_ver_inters (Tess, id, &entinters, &entinterqty);
  else if (dim == Tess.Dim - 1)
  {
    entinterqty = 1;
    entinters = ut_alloc_1d_int (1);
    entinters[0] = id;
  }
  else
    neut_tess_edge_faces (Tess, id, &entinters, &entinterqty);

  for (i = 0; i < entinterqty; i++)
  {
    inter = entinters[i];

    if (ut_array_1d_int_eltpos (inters, interqty, inter) == -1
        || neut_tess_inter_isperslave (Tess, inter))
      continue;

    neut_tess_inter_seeds_positive (Tess, inter, &seeds, &seedqty);
    for (j = 0; j < seedqty; j++)
      if (seeds[j] > Tess.CellQty)
        ut_array_1d_int_list_addval (&perseeds, &perseedqty, seeds[j]);
    ut_free_1d_int (&seeds);
  }

  ut_array_1d_int_sort (perseeds, perseedqty);

  (*pseeds) = ut_realloc_1d_int (*pseeds, *pseedqty + perseedqty);
  ut_array_1d_int_memcpy (perseeds, perseedqty, *pseeds + *pseedqty);
  (*pseedqty) += perseedqty;

  ut_free_1d_int (&entinters);
  ut_free_1d_int (&perseeds);

  return;
}

// The seeds of an entity (vertex or edge) are grouped when they are
// joined by continuous interfaces around the entity, so that the
// corresponding cells share the nodes of the entity.  All seeds of the
// entity are considered for joining, and the groups are those of seeds
// (in their order), completed by the other seeds they include.
void
nem_interface_dup_seedgroups (struct TESS Tess, int *inters, int interqty, int dim,
                              int id, int *seeds, int seedqty, int ***pgroups,
                              int *pgroupqty)
{
  int i, j, r1, r2, allseedqty, *allseeds = NULL, *roots = NULL, *grouproots = NULL;

  (*pgroupqty) = 0;
  (*pgroups) = NULL;

  if (dim < Tess.Dim - 1)
  {
    if (dim == 0)
      neut_tess_ver_seeds_positive (Tess, id, &allseeds, &allseedqty);
    else
      neut_tess_edge_seeds_positive (Tess, id, &allseeds, &allseedqty);
  }
  else
  {
    allseedqty = seedqty;
    allseeds = ut_alloc_1d_int (seedqty);
    ut_array_1d_int_memcpy (seeds, seedqty, allseeds);
  }

  roots = ut_alloc_1d_int (allseedqty);
  for (i = 0; i < allseedqty; i++)
    roots[i] = i;

  // (an interface is between 2 seeds only)
  if (dim < Tess.Dim - 1)
    for (i = 0; i < allseedqty; i++)
      for (j = i + 1; j < allseedqty; j++)
        if (nem_interface_dup_seedpair_continuous (Tess, inters, interqty, dim, id,
                                                   allseeds[i], allseeds[j]))
        {
          for (r1 = i; roots[r1] != r1; r1 = roots[r1]);
          for (r2 = j; roots[r2] != r2; r2 = roots[r2]);
          roots[ut_num_max_int (r1, r2)] = ut_num_min_int (r1, r2);
        }

  grouproots = ut_alloc_1d_int (seedqty);
  for (i = 0; i < seedqty; i++)
  {
    r1 = ut_array_1d_int_eltpos (allseeds, allseedqty, seeds[i]);
    if (r1 == -1)
      ut_print_neperbug ();
    for (; roots[r1] != r1; r1 = roots[r1]);

    j = ut_array_1d_int_eltpos (grouproots, *pgroupqty, r1);
    if (j == -1)
    {
      j = (*pgroupqty)++;
      grouproots[j] = r1;
      (*pgroups) = ut_realloc_2d_int_addline (*pgroups, *pgroupqty, 1);
    }

    (*pgroups)[j][0]++;
    (*pgroups)[j] = ut_realloc_1d_int ((*pgroups)[j], (*pgroups)[j][0] + 1);
    (*pgroups)[j][(*pgroups)[j][0]] = seeds[i];
  }

  // adding the other seeds of the groups (periodic seeds joined to the
  // cells by continuous interfaces), which are needed for periodicity
  for (i = 0; i < allseedqty; i++)
    if (ut_array_1d_int_eltpos (seeds, seedqty, allseeds[i]) == -1)
    {
      for (r1 = i; roots[r1] != r1; r1 = roots[r1]);

      j = ut_array_1d_int_eltpos (grouproots, *pgroupqty, r1);
      if (j != -1)
      {
        (*pgroups)[j][0]++;
        (*pgroups)[j] = ut_realloc_1d_int ((*pgroups)[j], (*pgroups)[j][0] + 1);
        (*pgroups)[j][(*pgroups)[j][0]] = allseeds[i];
      }
    }

  ut_free_1d_int (&allseeds);
  ut_free_1d_int (&roots);
  ut_free_1d_int (&grouproots);

  return;
}

// copy of a node for a seed, i.e. the copy whose seeds include the seed
// (-1 if none), or the node itself if it has no copies
void
nem_interface_dup_node_seed_copy (struct NODES Nodes, int **nodeseeds, int node, int seed,
                                  int *pcopy)
{
  int i, copy;

  if (Nodes.DupNodeMaster && Nodes.DupNodeMaster[node] > 0)
    node = Nodes.DupNodeMaster[node];

  if (!nodeseeds[node] || !Nodes.DupNodeSlaveQty[node])
  {
    (*pcopy) = node;
    return;
  }

  (*pcopy) = -1;
  for (i = 0; i <= Nodes.DupNodeSlaveQty[node]; i++)
  {
    copy = (i == 0) ? node : Nodes.DupNodeSlaveNb[node][i];
    if (ut_array_1d_int_eltpos (nodeseeds[copy] + 1, nodeseeds[copy][0], seed) != -1)
    {
      (*pcopy) = copy;
      break;
    }
  }

  return;
}

// element of a seed (elset) that contains nodes, -1 if none.  The nodes
// can be in the elements of several seeds where an interface is closed
// (where it meets continuous interfaces only).
void
nem_interface_dup_nodes_seed_elt (struct MESH Mesh, int *nodes, int nodeqty, int seed,
                                  int *pelt)
{
  int i, eltqty, *elts = NULL;

  neut_mesh_nodes_comelts (Mesh, nodes, nodeqty, &elts, &eltqty);

  (*pelt) = -1;
  for (i = 0; i < eltqty; i++)
    if (Mesh.EltElset[elts[i]] == seed)
    {
      (*pelt) = elts[i];
      break;
    }

  ut_free_1d_int (&elts);

  return;
}
