/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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

  if (mode == 2)
  {
    if (!(*pTess).CellTrue)
      neut_tess_init_celltrue (pTess);
    if (!(*pTess).CellBody)
      neut_tess_init_cellbody (pTess);
  }

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
