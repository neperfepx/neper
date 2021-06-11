/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconstruct_mesh_.h"

void
nem_reconstruct_mesh_2d (struct NODES Nodes, struct MESH *Mesh, int mode,
                         struct TESS *pTess)
{
  int i, FaceQty = 0;

  // Tests on the input and output
  for (i = 2; i <= 3; i++)
    if (Mesh[i].EltQty > 0 && Mesh[i].Dimension != i)
      ut_print_neperbug ();

  // If 3D mesh present and 2D mesh void, reconstructing the 2D mesh
  // If mode, setting FacePoly & Poly*
  if (Mesh[3].EltQty > 0)
  {
    int **FacePoly = NULL;

    // Reconstructing 2D mesh
    if (Mesh[2].EltQty == 0)
    {
      ut_print_message (0, 3, "Reconstructing 2D mesh... ");

      neut_mesh_init_nodeelts (Mesh + 3, Nodes.NodeQty);
      neut_mesh_init_eltelset (Mesh + 3, NULL);

      neut_mesh3d_mesh2d (Nodes, Mesh[3], Mesh + 2, &FacePoly, &FaceQty, 1);
      neut_mesh_init_eltelset (Mesh + 2, NULL);
      neut_mesh_orderelsets (Mesh + 2);

      if (mode == 2)
      {
        (*pTess).FaceQty = FaceQty;
        (*pTess).FacePoly = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
        for (i = 1; i <= (*pTess).FaceQty; i++)
          ut_array_1d_int_memcpy (FacePoly[i - 1], 2, (*pTess).FacePoly[i]);
      }
    }
    else
    {
      if (mode == 2)
      {
        (*pTess).FaceQty = Mesh[2].ElsetQty;
        neut_mesh_facepoly (Nodes, Mesh[2], Mesh[3], &((*pTess).FacePoly));
      }
    }

    neut_mesh_init_nodeelts (Mesh + 2, Nodes.NodeQty);

    if (mode == 2)
      neut_tess_init_polytopo_fromfacepoly (pTess);

    ut_free_2d_int (&FacePoly, FaceQty);
  }
  else
  {
    if (mode == 2)
    {
      (*pTess).FaceQty = Mesh[2].ElsetQty;
      (*pTess).FacePoly = ut_alloc_2d_int ((*pTess).FaceQty + 1, 2);
    }
  }

  // Setting up Tess face information
  if (mode == 2)
  {
    if (!(*pTess).FaceState)
      (*pTess).FaceState = ut_alloc_1d_int ((*pTess).FaceQty + 1);

    ut_array_1d_int_set ((*pTess).FaceState + 1, (*pTess).FaceQty, 1);
  }

  if (mode >= 1)
  {
    nem_tess_updatefrommesh_geom_face (pTess, Nodes, Mesh);
    nem_tess_updatefrommesh_geom_seed (pTess, Nodes, Mesh);
  }

  return;
}

void
nem_reconstruct_mesh_1d (struct NODES Nodes, struct MESH *Mesh, int mode,
                         struct TESS *pTess)
{
  int i, EdgeQty = 0;

  // Tests on the input and output
  for (i = 1; i <= 2; i++)
    if (Mesh[i].EltQty > 0 && Mesh[i].Dimension != i)
      ut_print_neperbug ();

  // If 2D mesh present, reconstructing the 1D mesh (if not void) and
  // setting FacePoly & Poly*
  if (Mesh[2].EltQty > 0)
  {
    int *EdgeFaceQty = NULL;
    int **EdgeFaceNb = NULL;

    // Reconstructing 1D mesh
    if (Mesh[1].EltQty == 0)
    {
      ut_print_message (0, 3, "Reconstructing 1D mesh... ");

      neut_mesh_init_nodeelts (Mesh + 2, Nodes.NodeQty);
      neut_mesh_init_eltelset (Mesh + 2, NULL);
      neut_mesh2d_mesh1d (Nodes, Mesh[2], Mesh + 1, &EdgeFaceNb, &EdgeFaceQty,
                          &EdgeQty, 1);

      if (mode == 2)
      {
        (*pTess).EdgeQty = EdgeQty;
        (*pTess).EdgeFaceQty = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
        ut_array_1d_int_memcpy (EdgeFaceQty, (*pTess).EdgeQty,
                                (*pTess).EdgeFaceQty + 1);
        (*pTess).EdgeFaceNb = ut_alloc_1d_pint ((*pTess).EdgeQty + 1);
        (*pTess).EdgeFaceNb[0] = ut_alloc_1d_int (1);
        for (i = 1; i <= (*pTess).EdgeQty; i++)
        {
          (*pTess).EdgeFaceNb[i] = ut_alloc_1d_int ((*pTess).EdgeFaceQty[i]);
          ut_array_1d_int_memcpy (EdgeFaceNb[i - 1], (*pTess).EdgeFaceQty[i],
                                  (*pTess).EdgeFaceNb[i]);
        }
      }
    }
    else
    {
      if (mode == 2)
      {
        (*pTess).EdgeQty = Mesh[1].ElsetQty;
        neut_mesh_edgeface (Mesh[1], Mesh[2], &((*pTess).EdgeFaceNb),
                            &((*pTess).EdgeFaceQty));
      }
    }

    if (mode == 2)
      neut_tess_init_facetopo_fromedge (pTess);

    ut_free_1d_int (&EdgeFaceQty);
    ut_free_2d_int (&EdgeFaceNb, EdgeQty);
  }
  else
  {
    if (mode == 2)
      (*pTess).EdgeQty = Mesh[1].ElsetQty;
  }

  if (mode == 2)
  {
    (*pTess).EdgeState = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
    (*pTess).EdgeDel = ut_alloc_1d_int ((*pTess).EdgeQty + 1);
  }

  if (!Mesh[1].NodeElts)
    neut_mesh_init_nodeelts (Mesh + 1, Nodes.NodeQty);

  if (mode >= 1)
    nem_tess_updatefrommesh_geom_edge (pTess, Nodes, Mesh);

  return;
}

void
nem_reconstruct_mesh_0d (struct NODES Nodes, struct MESH *Mesh, int mode,
                         struct TESS *pTess)
{
  int i, VerQty;

  // Tests on the input and output
  for (i = 0; i <= 1; i++)
    if (Mesh[i].EltQty != 0 && Mesh[i].Dimension != i)
      ut_print_neperbug ();

  // If 1D mesh present, reconstructing the 0D mesh (if not void) and
  // setting FacePoly & Poly*
  if (Mesh[1].EltQty > 0)
  {
    int **VerEdgeNb = NULL;
    int *VerEdgeQty = NULL;

    // Reconstructing 0D mesh
    if (Mesh[0].EltQty == 0)
    {
      ut_print_message (0, 3, "Reconstructing 0D mesh... ");

      neut_mesh_init_nodeelts (Mesh + 1, Nodes.NodeQty);
      neut_mesh_init_eltelset (Mesh + 1, NULL);

      neut_mesh1d_mesh0d (Mesh[1], Mesh + 0, &VerEdgeNb, &VerEdgeQty, &VerQty,
                          1);

      if (mode == 2)
      {
        (*pTess).VerQty = VerQty;

        (*pTess).VerEdgeQty = ut_alloc_1d_int ((*pTess).VerQty + 1);
        ut_array_1d_int_memcpy (VerEdgeQty, (*pTess).VerQty,
                                (*pTess).VerEdgeQty + 1);
        (*pTess).VerEdgeNb = ut_alloc_1d_pint ((*pTess).VerQty + 1);
        (*pTess).VerEdgeNb[0] = ut_alloc_1d_int (1);
        for (i = 1; i <= (*pTess).VerQty; i++)
        {
          (*pTess).VerEdgeNb[i] = ut_alloc_1d_int ((*pTess).VerEdgeQty[i]);
          ut_array_1d_int_memcpy (VerEdgeNb[i - 1], (*pTess).VerEdgeQty[i],
                                  (*pTess).VerEdgeNb[i]);
        }
      }
    }
    else
    {
      if (mode == 2)
      {
        (*pTess).VerQty = Mesh[0].ElsetQty;
        neut_mesh_veredge (Mesh[0], Mesh[1], &((*pTess).VerEdgeNb),
                           &((*pTess).VerEdgeQty));
      }
    }

    if (mode == 2)
    {
      // Filling out VerCoo
      (*pTess).VerCoo = ut_alloc_2d ((*pTess).VerQty + 1, 3);
      for (i = 1; i <= (*pTess).VerQty; i++)
        ut_array_1d_memcpy (Nodes.NodeCoo[Mesh[0].EltNodes[i][0]], 3,
                            (*pTess).VerCoo[i]);

      neut_tess_init_edgetopo_fromver (pTess);

      // aligning edge definition with mesh elset
      int elt, node1;
      double dist1, dist2;
      for (i = 1; i <= (*pTess).EdgeQty; i++)
      {
        elt = Mesh[1].Elsets[i][1];
        node1 = Mesh[1].EltNodes[elt][0];
        dist1 =
          ut_space_dist (Nodes.NodeCoo[node1],
                         (*pTess).VerCoo[(*pTess).EdgeVerNb[i][0]]);
        dist2 =
          ut_space_dist (Nodes.NodeCoo[node1],
                         (*pTess).VerCoo[(*pTess).EdgeVerNb[i][1]]);

        if (dist1 > dist2)
          ut_array_1d_int_reverseelts ((*pTess).EdgeVerNb[i], 2);
      }

      neut_tess_init_facetopo_fromver (pTess);
    }

    ut_free_2d_int (&VerEdgeNb, VerQty);
    ut_free_1d_int (&VerEdgeQty);
  }
  else
  {
    if (mode == 2)
      (*pTess).VerQty = Mesh[0].ElsetQty;
  }

  // Filling out VerState
  if (mode == 2)
    (*pTess).VerState = ut_alloc_1d_int ((*pTess).VerQty + 1);

  if (mode >= 1)
    nem_tess_updatefrommesh_geom_ver (pTess, Nodes, Mesh);

  return;
}
