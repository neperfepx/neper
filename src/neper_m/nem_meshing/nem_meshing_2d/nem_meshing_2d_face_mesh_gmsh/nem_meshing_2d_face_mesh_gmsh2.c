/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2d_face_mesh_gmsh_.h"

void
nem_meshing_2d_face_mesh_gmsh_boundary (struct TESS Tess, struct NODES Nodes,
                                        struct MESH *Mesh, int face,
                                        int **pbnodes, double ***pbnodecoos,
                                        int *pbnodeqty)
{
  int i;

  neut_mesh_face_boundnodes (Mesh[1], Tess, face, pbnodes, pbnodeqty);

  if (pbnodecoos)
  {
    (*pbnodecoos) = ut_alloc_2d (*pbnodeqty, 3);
    for (i = 0; i < *pbnodeqty; i++)
      ut_array_1d_memcpy (Nodes.NodeCoo[(*pbnodes)[i]], 3, (*pbnodecoos)[i]);
  }

  return;
}

void
nem_meshing_2d_face_mesh_gmsh_proj (struct TESS Tess, struct NODES Nodes,
                                    int face, struct MESHPARA MeshPara,
                                    int *bnodes, double ***pbnodecoos, int bnodeqty,
                                    int *pfacttype, double *factparms)
{
  int i;

  // (no mesh size factor, unless stated otherwise)
  (*pfacttype) = NEM_GMSH_SIZEFACT_NONE;
  ut_array_1d_zero (factparms, NEM_GMSH_SIZEFACT_PARMQTY);

  if (!strcmp (MeshPara.face_op[face], "noproj"))
    return;

  else if (!strcmp (MeshPara.face_op[face], "meshproj")
           || !strcmp (MeshPara.face_op[face], "planeproj"))
    for (i = 0; i < bnodeqty; i++)
      ut_space_point_dir_plane_proj ((*pbnodecoos)[i],
                                     MeshPara.face_eq[face] + 1,
                                     MeshPara.face_eq[face],
                                     (*pbnodecoos)[i]);

  // stereographic projection (conformal) onto the tangent plane at the
  // middle of the face, which scales the lengths by a factor (see
  // nem_meshing_2d_face_mesh_gmsh_sph_stereo)
  else if (!strcmp (MeshPara.face_op[face], "sphereproj"))
  {
    (*pfacttype) = NEM_GMSH_SIZEFACT_STEREO;
    nem_meshing_2d_face_mesh_gmsh_sph_stereo (Tess, face, Nodes, bnodes, bnodeqty,
                                              *pbnodecoos, factparms);
  }

  // unrolling the face onto its tangent plane (at the middle of the face),
  // which preserves the lengths
  else if (!strcmp (MeshPara.face_op[face], "cylinderproj"))
    nem_meshing_2d_face_mesh_gmsh_cyl_unroll (Tess, face, Nodes, bnodes, bnodeqty,
                                              *pbnodecoos);

  // conformal unrolling of the face onto a plane, which scales the lengths by
  // a factor (see nem_meshing_2d_face_mesh_gmsh_tor_unroll)
  else if (!strcmp (MeshPara.face_op[face], "torusproj"))
  {
    (*pfacttype) = NEM_GMSH_SIZEFACT_TORUS;
    nem_meshing_2d_face_mesh_gmsh_tor_unroll (Tess, face, Nodes, bnodes, bnodeqty,
                                              *pbnodecoos, factparms);
  }

  else
    abort ();

  return;
}

void
nem_meshing_2d_face_mesh_gmsh_backproj (struct TESS Tess, struct NODES RNodes,
                                        struct MESH *RMesh, int face,
                                        struct MESHPARA MeshPara,
                                        struct NODES Nodes, int *bnodes,
                                        int *lbnodes, int bnodeqty,
                                        struct NODES *pN, struct MESH M)
{
  int i, domface = (Tess.Dim == 3) ? Tess.FaceDom[face][1] : -1;

  // Below, we proceed in different ways based on the type of the face

  if (!strcmp (MeshPara.face_op[face], "noproj"))
    return;

  else if (!strcmp (MeshPara.face_op[face], "meshproj"))
    neut_nodes_proj_alongontomesh (pN, MeshPara.face_eq[face] + 1, RNodes,
                                   RMesh[2], face);

  else if (!strcmp (MeshPara.face_op[face], "planeproj"))
  {
    // back-projecting
    struct NODES Nint;
    struct MESH Mint;

    neut_nodes_set_zero (&Nint);
    neut_mesh_set_zero (&Mint);

    neut_tess_face_interpolmesh (Tess, face, &Nint, &Mint);
    neut_nodes_proj_alongontomesh (pN, MeshPara.face_eq[face] + 1, Nint, Mint,
                                   1);

    neut_nodes_free (&Nint);
    neut_mesh_free (&Mint);

    if ((*pN).NodeQty > 0)
      nem_meshing_2d_face_mesh_gmsh_backproj_fixboundary (Nodes, bnodes,
                                                          lbnodes, bnodeqty,
                                                          pN);

    // if the face belongs to a curved domface, projecting onto it
    if (neut_tess_face_iscurved (Tess, face))
      for (i = 1; i <= (*pN).NodeQty; i++)
        neut_primparms_point_proj (Tess.DomFaceType[domface],
                                   Tess.DomFaceParms[domface],
                                   (*pN).NodeCoo[i], (*pN).NodeCoo[i]);

    // if the face has curved edges (edges that belong to a curved domedge)
    // laplacian smoothing it
    else if (neut_tess_face_hascurvededge (Tess, face))
      neut_mesh_2d_laplaciansmooth (pN, M, lbnodes, bnodeqty);

    // in other cases (face not flat), we do nothing
  }

  // inverse stereographic projection
  else if (!strcmp (MeshPara.face_op[face], "sphereproj"))
    nem_meshing_2d_face_mesh_gmsh_sph_unstereo (Tess, face, Nodes, bnodes, bnodeqty, pN);

  // rolling the face back (inverse of the unrolling)
  else if (!strcmp (MeshPara.face_op[face], "cylinderproj"))
    for (i = 1; i <= (*pN).NodeQty; i++)
      nem_meshing_2d_face_mesh_gmsh_cyl_roll (Tess, face, Nodes, bnodes, bnodeqty,
                                              (*pN).NodeCoo[i], (*pN).NodeCoo[i]);

  // rolling the face back (inverse of the unrolling)
  else if (!strcmp (MeshPara.face_op[face], "torusproj"))
    nem_meshing_2d_face_mesh_gmsh_tor_roll (Tess, face, Nodes, bnodes, bnodeqty, pN);

  else
    abort ();

  // should not be necessary, but just in case
  if ((*pN).NodeQty > 0)
    nem_meshing_2d_face_mesh_gmsh_backproj_fixboundary (Nodes, bnodes,
                                                        lbnodes, bnodeqty,
                                                        pN);

  return;
}

// Finding the boundary nodes of a face mesh (in the projection plane), from
// their coordinates
void
nem_meshing_2d_face_mesh_gmsh_bnodes (struct NODES N, struct MESH *pM,
                                      double **bnodecoos, int bnodeqty,
                                      int *lbnodes)
{
  int i, j, pos, *nodes1d = NULL, nodeqty1d;
  double min_dist_max = 0, *dists = NULL;
  struct MESH M1D;

  neut_mesh_set_zero (&M1D);

  // reconstructing boundary mesh (node numbers refer to N)
  neut_mesh_init_nodeelts (pM, N.NodeQty);
  neut_mesh2d_mesh1d (N, *pM, &M1D, NULL, NULL, NULL, 0);
  neut_mesh_nodes (M1D, &nodes1d, &nodeqty1d);

  dists = ut_alloc_1d (nodeqty1d);
  for (i = 0; i < bnodeqty; i++)
  {
    for (j = 0; j < nodeqty1d; j++)
      dists[j] = ut_space_dist (bnodecoos[i], N.NodeCoo[nodes1d[j]]);

    pos = ut_array_1d_min_index (dists, nodeqty1d);
    lbnodes[i] = nodes1d[pos];
    min_dist_max = ut_num_max (min_dist_max, dists[pos]);
  }

  // the boundary mesh is passed to Gmsh and kept unchanged
  if (bnodeqty != nodeqty1d || min_dist_max > 1e-9)
  {
    printf ("\n");
    printf ("bnodeqty = %d\n", bnodeqty);
    printf ("nodeqty1d = %d\n", nodeqty1d);
    printf ("min_dist_max = %f\n", min_dist_max);
    ut_print_neperbug ();
  }

  neut_mesh_free (&M1D);
  ut_free_1d (&dists);
  ut_free_1d_int (&nodes1d);

  return;
}

// boundary nodes passed to Gmsh, with recombination: one in two, from a
// vertex node (the others are the centres of the coarse boundary segments,
// see nem_meshing); they are bnodecoos[offset + 2 i]
void
nem_meshing_2d_face_mesh_gmsh_coarse (struct TESS Tess, struct MESH *Mesh, int face,
                                      int *bnodes, double **bnodecoos, double *cls,
                                      int bnodeqty, int *poffset, double ***pgcoos,
                                      double **pgcls, int *pgqty)
{
  int i, ver, node, pos = -1;

  for (i = 1; i <= Tess.FaceVerQty[face] && pos == -1; i++)
  {
    ver = Tess.FaceVerNb[face][i];
    node = Mesh[0].EltNodes[Mesh[0].Elsets[ver][1]][0];
    pos = ut_array_1d_int_eltpos (bnodes, bnodeqty, node);
  }

  if (pos == -1 || bnodeqty % 2)
    ut_print_neperbug ();

  (*poffset) = pos % 2;
  (*pgqty) = bnodeqty / 2;
  (*pgcoos) = ut_alloc_2d (*pgqty, 3);
  (*pgcls) = ut_alloc_1d (*pgqty);

  for (i = 0; i < *pgqty; i++)
  {
    ut_array_1d_memcpy (bnodecoos[*poffset + 2 * i], 3, (*pgcoos)[i]);
    (*pgcls)[i] = cls[*poffset + 2 * i];
  }

  return;
}

// splitting a (recombined) face mesh, of quad elts and tri elts (recorded
// as quad elts of last node 0), into quad elts, as Gmsh does: a quad elt (0,
// 1, 2, 3) of edge centres (01, 12, 23, 30) and centre c is split into (0,
// 01, c, 30), (1, 12, c, 01), (2, 23, c, 12) and (3, 30, c, 23), and a tri
// elt similarly into 3 quad elts.  The centres of the boundary edges are the
// boundary nodes not passed to Gmsh (gcoos[i] = bnodecoos[offset + 2 i],
// see nem_meshing_2d_face_mesh_gmsh_coarse).
void
nem_meshing_2d_face_mesh_gmsh_split (double **gcoos, int gqty, double **bnodecoos,
                                     int bnodeqty, int offset, struct NODES *pN,
                                     struct MESH *pM)
{
  int i, j, k, nn, centre, childqty, nodeqty = (*pN).NodeQty, *n = NULL;
  int mids[4], *gpos = ut_alloc_1d_int (nodeqty + 1);
  int **edgenodes = ut_alloc_1d_pint (nodeqty + 1);
  int childnodeqty = 4 * (*pM).EltQty + 1;
  int **childnodes = ut_alloc_2d_int (childnodeqty, 4);
  double coo[3], dist, mindist;
  int *elts = NULL;

  // positions of the boundary nodes in gcoos
  ut_array_1d_int_set (gpos + 1, nodeqty, -1);
  for (k = 0; k < gqty; k++)
  {
    j = -1;
    mindist = DBL_MAX;
    for (i = 1; i <= nodeqty; i++)
    {
      dist = ut_space_dist (gcoos[k], (*pN).NodeCoo[i]);
      if (dist < mindist)
      {
        mindist = dist;
        j = i;
      }
    }
    gpos[j] = k;
  }

  childqty = 0;
  for (i = 1; i <= (*pM).EltQty; i++)
  {
    n = (*pM).EltNodes[i];
    nn = n[3] ? 4 : 3;

    for (j = 0; j < nn; j++)
      mids[j] = nem_meshing_2d_face_mesh_gmsh_split_edgenode (gpos, gqty, bnodecoos,
                                                              bnodeqty, offset, n[j],
                                                              n[(j + 1) % nn], edgenodes,
                                                              pN);

    neut_nodes_bary (*pN, n, nn, coo, NULL);
    centre = neut_nodes_addnode (pN, coo, -1);

    for (j = 0; j < nn; j++)
    {
      childqty++;
      childnodes[childqty][0] = n[j];
      childnodes[childqty][1] = mids[j];
      childnodes[childqty][2] = centre;
      childnodes[childqty][3] = mids[(j + nn - 1) % nn];
    }
  }

  neut_mesh_free (pM);
  (*pM) = neut_mesh_alloc (2, "quad", 1, childqty, 0);
  for (i = 1; i <= childqty; i++)
    ut_array_1d_int_memcpy (childnodes[i], 4, (*pM).EltNodes[i]);

  elts = ut_alloc_1d_int (childqty);
  for (i = 0; i < childqty; i++)
    elts[i] = i + 1;
  neut_mesh_addelset (pM, elts, childqty);
  neut_mesh_init_eltelset (pM, NULL);

  ut_free_1d_int (&gpos);
  ut_free_2d_int (&edgenodes, nodeqty + 1);
  ut_free_2d_int (&childnodes, childnodeqty);
  ut_free_1d_int (&elts);

  return;
}

// Laplacian smoothing of a face mesh of quad elts, which is reverted if it
// decreases the minimum scaled Jacobian of the elts
void
nem_meshing_2d_face_mesh_gmsh_smooth (struct MESH M, int *lbnodes, int bnodeqty,
                                      struct NODES *pN)
{
  double sjmin, **coos = ut_alloc_2d ((*pN).NodeQty + 1, 3);

  ut_array_2d_memcpy ((*pN).NodeCoo + 1, (*pN).NodeQty, 3, coos + 1);
  sjmin = nem_meshing_2d_face_mesh_gmsh_sjmin (*pN, M);

  neut_mesh_2d_laplaciansmooth (pN, M, lbnodes, bnodeqty);

  if (nem_meshing_2d_face_mesh_gmsh_sjmin (*pN, M) < sjmin)
    ut_array_2d_memcpy (coos + 1, (*pN).NodeQty, 3, (*pN).NodeCoo + 1);

  ut_free_2d (&coos, (*pN).NodeQty + 1);

  return;
}
