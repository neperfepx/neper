/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "nem_reconstruct_mesh_.h"

void
nem_reconstruct_mesh (char *dim, struct NODES *pNodes, struct MESH *Mesh,
                      struct TESS *pTess)
{
  int i, input_dim, recon_dim, mode;

  // Testing out input-output dimension compatibility
  input_dim = neut_mesh_array_dim (Mesh);

  if (input_dim == -1)
    ut_print_message (2, 0, "All input meshes are void!\n");

  recon_dim = input_dim;
  for (i = 0; i < input_dim; i++)
    if (!strcmp (dim, "all") || ut_list_testelt_int (dim, NEUT_SEP_NODEP, i))
    {
      recon_dim = i;
      break;
    }

  if (recon_dim > input_dim)
    ut_print_message (2, 0, "Wrong dimensions for mesh reconstruction.\n");

  // Testing if topology must be reconstructed
  // no tess in input: do nothing
  if (!pTess)
    mode = 0;
  // full tess in input: do morphology
  else if ((*pTess).VerQty != 0)
    mode = 1;
  // void tess in input: do everything
  else
    mode = 2;

  if (mode == 1)
    for (i = recon_dim; i <= input_dim; i++)
      if (Mesh[i].EltQty == 0 && (*pTess).VerQty > 0)
      {
        ut_print_message (1, 2,
                          "%dD mesh is missing. Tessellation will be disregarded.\n",
                          i);
        mode = 2;
      }

  for (i = 0; i <= 3; i++)
  {
    if (!(Mesh[i].EltElset))
      neut_mesh_init_eltelset (Mesh + i, NULL);

    if (!(Mesh[i].NodeElts))
      neut_mesh_init_nodeelts (Mesh + i, (*pNodes).NodeQty);
  }

  if (mode == 2)
  {
    (*pTess).Level = 1;
    (*pTess).Dim = input_dim;
    (*pTess).CellQty = Mesh[input_dim].ElsetQty;
    ut_string_string ("standard", &((*pTess).Type));
    ut_string_string ("unknown", &((*pTess).DomType));
  }

  // Reconstructing
  if (input_dim >= 2 && recon_dim <= 2)
    nem_reconstruct_mesh_2d (*pNodes, Mesh, mode, pTess);

  if (input_dim >= 1 && recon_dim <= 1)
    nem_reconstruct_mesh_1d (*pNodes, Mesh, mode, pTess);

  if (input_dim >= 0 && recon_dim <= 0)
    nem_reconstruct_mesh_0d (*pNodes, Mesh, mode, pTess);

  if (mode == 2)                // Tess is void; reconstructing domain
    neut_tess_init_domain (pTess);

  if (mode >= 1)
    neut_tess_init_domfaceeq (pTess);

  for (i = 0; i <= 2; i++)
    if (strcmp (dim, "all") != 0
        && !ut_list_testelt_int (dim, NEUT_SEP_NODEP, i))
      neut_mesh_free (Mesh + i);

  /*
     if (pTess != NULL && neut_tess_test (*pTess) != 0)
     {
     ut_print_message (2, 0, "nem_reconstruct_mesh: tess is not valid!\n");
     ut_print_neperbug ();
     }
 */

  return;
}

// Setting the labels of the domain faces of a tessellation reconstructed
// from a mesh to the ones of the fasets of the mesh (each faset is made of
// the 2D elts of a domain face, and the faces of the tessellation are the
// elsets of the 2D mesh), and the labels of the domain edges and vertices
// accordingly
void
nem_reconstruct_mesh_fasets (struct MESH Mesh2D, struct MESH MeshFasets,
                             struct TESS *pTess)
{
  int i, elt, face, eltqty, *elts = NULL;
  int vernodeqty = neut_elt_nodeqty (MeshFasets.EltType, 2, 1);

  if ((*pTess).Dim != 3)
    return;

  for (i = 1; i <= MeshFasets.ElsetQty; i++)
  {
    if (MeshFasets.Elsets[i][0] == 0)
      continue;

    // 2D elt of the first elt of the faset, and its face
    elt = MeshFasets.Elsets[i][1];
    neut_mesh_nodes_comelts (Mesh2D, MeshFasets.EltNodes[elt], vernodeqty,
                             &elts, &eltqty);
    if (eltqty != 1)
      ut_print_neperbug ();

    face = Mesh2D.EltElset[elts[0]];
    if ((*pTess).FaceDom[face][0] != 2)
      ut_print_neperbug ();

    ut_string_string (MeshFasets.ElsetLabels[i],
                      (*pTess).DomFaceLabel + (*pTess).FaceDom[face][1]);
  }

  neut_tess_init_domain_label_3d_edgever (pTess);

  ut_free_1d_int (&elts);

  return;
}

// Setting the labels of the domain faces of a tessellation reconstructed
// from a mesh to the ones of the face nsets of the mesh (NSet2D): a domain
// face takes the label of the (non-body) nset that contains all the nodes of
// its 2D elts, and the labels of the domain edges and vertices are set
// accordingly.  Returns 0 if all domain faces have been labeled, and -1
// otherwise (labels unchanged).
int
nem_reconstruct_mesh_nsets (struct NODES Nodes, struct MESH Mesh2D, struct NSET NSet2D,
                            struct TESS *pTess)
{
  int i, j, k, l, m, face, elt, isin, status = 0;
  int vernodeqty = neut_elt_nodeqty (Mesh2D.EltType, 2, 1);
  int *mask = ut_alloc_1d_int (Nodes.NodeQty + 1);
  int *nsets = ut_alloc_1d_int ((*pTess).DomFaceQty + 1);

  if ((*pTess).Dim != 3 || (*pTess).DomFaceQty == 0)
    status = -1;

  for (i = 1; i <= (*pTess).DomFaceQty && !status; i++)
  {
    for (j = 1; j <= NSet2D.qty && !nsets[i]; j++)
    {
      if (strstr (NSet2D.names[j], "body"))
        continue;

      for (k = 0; k < NSet2D.NodeQty[j]; k++)
        mask[NSet2D.nodes[j][k]] = 1;

      isin = 1;
      for (k = 1; k <= (*pTess).DomTessFaceQty[i] && isin; k++)
      {
        face = (*pTess).DomTessFaceNb[i][k];
        for (l = 1; l <= Mesh2D.Elsets[face][0] && isin; l++)
        {
          elt = Mesh2D.Elsets[face][l];
          for (m = 0; m < vernodeqty; m++)
            if (!mask[Mesh2D.EltNodes[elt][m]])
            {
              isin = 0;
              break;
            }
        }
      }

      if (isin)
        nsets[i] = j;

      for (k = 0; k < NSet2D.NodeQty[j]; k++)
        mask[NSet2D.nodes[j][k]] = 0;
    }

    if (!nsets[i])
      status = -1;
  }

  if (!status)
  {
    for (i = 1; i <= (*pTess).DomFaceQty; i++)
      ut_string_string (NSet2D.names[nsets[i]], (*pTess).DomFaceLabel + i);

    neut_tess_init_domain_label_3d_edgever (pTess);
  }

  ut_free_1d_int (&mask);
  ut_free_1d_int (&nsets);

  return status;
}
