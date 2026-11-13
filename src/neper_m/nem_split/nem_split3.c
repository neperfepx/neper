/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_split_.h"

// a face centre node is created for each face of the 3D elts and shared by
// the (1 or 2) 3D elts and the (0 or 1) 2D elt of the face
void
nem_split_facenodes_3d (struct NODES *pNodes, struct MESH *Mesh, int **pfacenode,
                        int ***peltfacenodes)
{
  int i, j, k, l, elt, node, nodes[3], *elts = NULL, eltqty;
  double coo[3], cl;

  (*peltfacenodes) = ut_alloc_2d_int (Mesh[3].EltQty + 1, 4);

  for (i = 1; i <= Mesh[3].EltQty; i++)
    for (j = 0; j < 4; j++)
      if (!(*peltfacenodes)[i][j])
      {
        for (k = 0, l = 0; k < 4; k++)
          if (k != j)
            nodes[l++] = Mesh[3].EltNodes[i][k];

        neut_nodes_bary (*pNodes, nodes, 3, coo, &cl);
        node = neut_nodes_addnode (pNodes, coo, cl);

        neut_mesh_nodes_comelts (Mesh[3], nodes, 3, &elts, &eltqty);

        for (k = 0; k < eltqty; k++)
        {
          elt = elts[k];
          for (l = 0; l < 4; l++)
            if (ut_array_1d_int_eltpos (nodes, 3, Mesh[3].EltNodes[elt][l]) == -1)
              break;
          (*peltfacenodes)[elt][l] = node;
        }
      }

  (*pfacenode) = ut_alloc_1d_int (Mesh[2].EltQty + 1);

  for (i = 1; i <= Mesh[2].EltQty; i++)
  {
    neut_mesh_nodes_comelts (Mesh[3], Mesh[2].EltNodes[i], 3, &elts, &eltqty);
    if (eltqty == 0)
      ut_print_neperbug ();

    elt = elts[0];
    for (l = 0; l < 4; l++)
      if (ut_array_1d_int_eltpos (Mesh[2].EltNodes[i], 3, Mesh[3].EltNodes[elt][l]) == -1)
        break;
    (*pfacenode)[i] = (*peltfacenodes)[elt][l];
  }

  ut_free_1d_int (&elts);

  return;
}

// projecting the face centre nodes of the elements of curved faces onto
// their domain faces (as in nem_order_curved)
void
nem_split_facenodes_curved (struct TESS Tess, int *facenode, struct NODES *pNodes,
                            struct MESH *Mesh)
{
  int i, face, domface;
  double *coo = NULL;

  if (Tess.Dim != 3 || !Tess.DomFaceType || Mesh[2].ElsetQty != Tess.FaceQty)
    return;

  for (i = 1; i <= Mesh[2].EltQty; i++)
  {
    face = Mesh[2].EltElset[i];

    if (neut_tess_face_iscurved (Tess, face))
    {
      domface = Tess.FaceDom[face][1];
      coo = (*pNodes).NodeCoo[facenode[i]];
      neut_primparms_point_proj (Tess.DomFaceType[domface], Tess.DomFaceParms[domface],
                                 coo, coo);
    }
  }

  return;
}

// tying the face centre nodes of the elements of the periodic slave faces
// to the ones of the elements of their master faces (as in
// nem_order_periodic_faces)
void
nem_split_facenodes_periodic (struct TESS Tess, int *facenode, struct NODES *pNodes,
                              struct MESH *Mesh)
{
  int i, j, k, elt, master, nodes[3], shift[3];

  if (Tess.Dim != 3)
    return;

  for (i = 1; i <= Tess.FaceQty; i++)
    if (neut_tess_face_isperslave (Tess, i))
    {
      ut_array_1d_int_memcpy (Tess.PerFaceShift[i], 3, shift);
      ut_array_1d_int_scale (shift, 3, -1);

      for (j = 1; j <= Mesh[2].Elsets[i][0]; j++)
      {
        elt = Mesh[2].Elsets[i][j];

        for (k = 0; k < 3; k++)
          neut_nodes_node_shift_pernode (*pNodes, Mesh[2].EltNodes[elt][k], shift,
                                         nodes + k);

        if (neut_mesh_nodes_comelt (Mesh[2], nodes, 3, &master) != 0)
          ut_print_neperbug ();

        neut_nodes_markasslave (pNodes, facenode[elt], facenode[master],
                                Tess.PerFaceShift[i]);
      }
    }

  neut_nodes_init_nodeslave (pNodes);

  return;
}

// replacing each elt of a mesh by its childqty children (of nodes
// childnodes, indexed [1...EltQty * childqty]), which are recorded in the
// elsets of the elt, in place of the elt
void
nem_split_mesh (int childqty, int **childnodes, struct MESH *pMesh)
{
  int i, j, k, eltqty = childqty * (*pMesh).EltQty, eltnodeqty, *elset = NULL;

  if ((*pMesh).Dimension >= 2)
    ut_string_string ("quad", &(*pMesh).EltType);
  (*pMesh).EltOrder = 1;
  eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, 1);

  // elsets
  for (i = 1; i <= (*pMesh).ElsetQty; i++)
  {
    elset = ut_alloc_1d_int (childqty * (*pMesh).Elsets[i][0] + 1);
    elset[0] = childqty * (*pMesh).Elsets[i][0];
    for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      for (k = 0; k < childqty; k++)
        elset[childqty * (j - 1) + k + 1] = childqty * ((*pMesh).Elsets[i][j] - 1) + k + 1;

    ut_free_1d_int ((*pMesh).Elsets + i);
    (*pMesh).Elsets[i] = elset;
  }

  // elts
  ut_free_2d_int (&(*pMesh).EltNodes, (*pMesh).EltQty + 1);
  (*pMesh).EltNodes = ut_alloc_2d_int (eltqty + 1, eltnodeqty);
  for (i = 1; i <= eltqty; i++)
    ut_array_1d_int_memcpy (childnodes[i], eltnodeqty, (*pMesh).EltNodes[i]);

  ut_free_1d_int (&(*pMesh).EltId);
  ut_free_1d_int (&(*pMesh).EltPart);
  ut_free_1d_int (&(*pMesh).EltBody);
  ut_free_1d (&(*pMesh).EltWeight);
  ut_free_2d (&(*pMesh).EltOri, (*pMesh).EltQty + 1);
  ut_free_2d (&(*pMesh).SimEltOri, (*pMesh).EltQty + 1);

  (*pMesh).EltQty = eltqty;
  neut_mesh_init_eltelset (pMesh, NULL);
  neut_mesh_init_nodeelts (pMesh, 0);

  return;
}
