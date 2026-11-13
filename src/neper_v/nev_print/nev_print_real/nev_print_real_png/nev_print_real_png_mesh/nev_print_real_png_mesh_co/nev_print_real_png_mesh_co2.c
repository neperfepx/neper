/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_real_png_mesh_co_.h"

void
nev_print_real_png_mesh_co_compress (struct PRINT Print, struct MESH MeshCo,
                                     struct DATA NodeData, struct NODES *pN,
                                     struct MESH *pM2D, int **pelt2deltco,
                                     int **pedges, int *pedgeqty)
{
  int i, j, k, elt, qty, nodeqty, vernodeqty, triqty, edgeqty, refedgeqty;
  int tri[3], *eltnodes = NULL, *nodes_old_new = NULL, **seqtri = NULL, **seqedge = NULL;
  double ref, area, *v1 = ut_alloc_1d (3), *v2 = ut_alloc_1d (3), *n = ut_alloc_1d (3);

  nev_print_real_png_mesh_co_seq (MeshCo, &vernodeqty, &seqtri, &triqty, &seqedge,
                                  &edgeqty, &refedgeqty);

  ut_string_string ("tri", &(*pM2D).EltType);
  (*pM2D).Dimension = 2;
  (*pM2D).EltOrder = 1;
  (*pelt2deltco) = ut_alloc_1d_int (1);
  (*pedges) = NULL;
  (*pedgeqty) = 0;

  nodeqty = 0;
  for (i = 1; i <= MeshCo.EltQty; i++)
    nodeqty = ut_num_max (nodeqty, ut_array_1d_int_max (MeshCo.EltNodes[i], vernodeqty));
  nodes_old_new = ut_alloc_1d_int (nodeqty + 1);

  for (elt = 1; elt <= MeshCo.EltQty; elt++)
  {
    if (!Print.showeltco[elt])
      continue;

    eltnodes = MeshCo.EltNodes[elt];

    for (j = 0; j < vernodeqty; j++)
      if (!nodes_old_new[eltnodes[j]])
      {
        neut_nodes_addnode (pN, NodeData.Coo[eltnodes[j]], 0);
        nodes_old_new[eltnodes[j]] = (*pN).NodeQty;
      }

    // reference length (of the edges of the interface facet), for testing
    // degeneracy
    ref = 0;
    for (j = 0; j < refedgeqty; j++)
      ref = ut_num_max (ref, ut_space_dist (NodeData.Coo[eltnodes[seqedge[j][0]]],
                                            NodeData.Coo[eltnodes[seqedge[j][1]]]));

    for (j = 0; j < triqty; j++)
    {
      for (k = 0; k < 3; k++)
        tri[k] = eltnodes[seqtri[j][k]];

      ut_array_1d_sub (NodeData.Coo[tri[0]], NodeData.Coo[tri[1]], 3, v1);
      ut_array_1d_sub (NodeData.Coo[tri[0]], NodeData.Coo[tri[2]], 3, v2);
      ut_vector_vectprod (v1, v2, n);
      area = 0.5 * ut_vector_norm (n);

      if (area <= 1e-6 * ref * ref)
        continue;

      for (k = 0; k < 3; k++)
        tri[k] = nodes_old_new[tri[k]];
      neut_mesh_addelt (pM2D, tri);
      (*pelt2deltco) = ut_realloc_1d_int (*pelt2deltco, (*pM2D).EltQty + 1);
      (*pelt2deltco)[(*pM2D).EltQty] = elt;
    }

    for (j = 0; j < edgeqty; j++)
    {
      int node1 = eltnodes[seqedge[j][0]], node2 = eltnodes[seqedge[j][1]];

      if (ut_space_dist (NodeData.Coo[node1], NodeData.Coo[node2]) <= 1e-6 * ref)
        continue;

      (*pedges) = ut_realloc_1d_int (*pedges, 2 * ((*pedgeqty) + 1));
      (*pedges)[2 * (*pedgeqty)] = ut_num_min (nodes_old_new[node1], nodes_old_new[node2]);
      (*pedges)[2 * (*pedgeqty) + 1] = ut_num_max (nodes_old_new[node1], nodes_old_new[node2]);
      (*pedgeqty)++;
    }
  }

  // removing duplicate edges (shared by neighbor elts)
  if (*pedgeqty > 0)
  {
    qsort (*pedges, *pedgeqty, 2 * sizeof (int), nev_print_real_png_mesh_co_edge_cmp);

    qty = 1;
    for (i = 1; i < *pedgeqty; i++)
      if (nev_print_real_png_mesh_co_edge_cmp ((*pedges) + 2 * i, (*pedges) + 2 * (qty - 1)))
      {
        (*pedges)[2 * qty] = (*pedges)[2 * i];
        (*pedges)[2 * qty + 1] = (*pedges)[2 * i + 1];
        qty++;
      }
    (*pedgeqty) = qty;
  }

  ut_free_2d_int (&seqtri, triqty);
  ut_free_2d_int (&seqedge, edgeqty);
  ut_free_1d_int (&nodes_old_new);
  ut_free_1d (&v1);
  ut_free_1d (&v2);
  ut_free_1d (&n);

  return;
}

void
nev_print_real_png_mesh_co_print_faces (FILE * file, struct PRINT Print, struct NODES N,
                                        struct MESH M2D, int *elt2deltco,
                                        struct DATA MeshDataCo)
{
  int i;
  int **rgb = ut_alloc_2d_int (M2D.EltQty + 1, 3);
  double *trs = ut_alloc_1d (M2D.EltQty + 1);

  for (i = 1; i <= M2D.EltQty; i++)
  {
    ut_array_1d_int_memcpy (MeshDataCo.Col[elt2deltco[i]], 3, rgb[i]);
    if (MeshDataCo.Trs)
      trs[i] = MeshDataCo.Trs[elt2deltco[i]];
  }

  nev_print_real_png_mesh2d (file, N, M2D, NULL, rgb, trs, "elt", Print);

  ut_free_2d_int (&rgb, M2D.EltQty + 1);
  ut_free_1d (&trs);

  return;
}

void
nev_print_real_png_mesh_co_print_edges (FILE * file, struct PRINT Print, struct NODES N,
                                        int *edges, int edgeqty, struct DATA MeshDataCo)
{
  int i;

  if (edgeqty == 0 || MeshDataCo.BRad <= 0)
    return;

  fprintf (file,
           "#declare eltcoedge =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f diffuse %f reflection %f} }\n",
           MeshDataCo.BCol[0] / 255., MeshDataCo.BCol[1] / 255., MeshDataCo.BCol[2] / 255.,
           Print.lightambient, Print.lightdiffuse, Print.lightreflection);

  for (i = 0; i < edgeqty; i++)
    nev_print_real_png_segment_wsph (file, N.NodeCoo[edges[2 * i]], N.NodeCoo[edges[2 * i + 1]],
                                     MeshDataCo.BRad, "eltcoedge");

  return;
}
