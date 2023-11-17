/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_load_.h"

void
nev_load (char *string, struct SIM *pSim, struct TESS *pTess, struct TESR *pTesr,
          struct NODES *pNodes, struct MESH *Mesh, struct POINT **pPoints,
          int *pPointQty, struct DATA *TessData, struct DATA *TesrData,
          struct DATA *pNodeData, struct DATA **MeshData,
          struct DATA **pPointData)
{
  int i, j, qty, dim, topology;
  char **list = NULL;

  if (!neut_tess_isvoid (*pTess) || !neut_tesr_isvoid (*pTesr)
      || !neut_nodes_isvoid (*pNodes) || *pPointQty > 0)
    ut_print_message (2, 0, "Input file already loaded (reading `%s').\n",
                      string);

  // simulation directory
  if (ut_sys_isdir (string))
  {
    neut_sim_fscanf (string, pSim, "R");

    qty = 0;
    list = ut_alloc_1d_pchar (3);
    if ((*pSim).tess)
      list[qty++] = ut_string_paste3 (string, "/inputs/", (*pSim).tess);
    if ((*pSim).tesr)
      list[qty++] = ut_string_paste3 (string, "/inputs/", (*pSim).tesr);
    if ((*pSim).msh)
      list[qty++] = ut_string_paste3 (string, "/inputs/", (*pSim).msh);
  }

  // other
  else
    ut_list_break (string, NEUT_SEP_NODEP, &list, &qty);

  for (i = 0; i < qty; i++)
  {
    char *input = NULL;
    char **list2 = NULL;
    int qty2;
    char *file = NULL;

    ut_list_break (list[i], NEUT_SEP_DEP, &list2, &qty2);

    if (qty2 == 1)
    {
      ut_string_string (list[i], &file);
      ut_file_format (list[i], &input);
      if (!strlen (input))
        ut_string_string ("point", &input); // we give it the default name
    }
    else if (qty2 == 2)
    {
      ut_string_string (list2[0], &input);
      ut_string_string (list2[1], &file);
    }

    if (!input)
      ut_print_message (2, 1, "Input file `%s' is void.\n", list[i]);

    if (!strcmp (input, "tess"))
    {
      ut_print_message (0, 1, "Loading tessellation...\n");
      neut_tess_fnscanf (file, pTess);
      (*pTess).pSim = pSim;
    }

    else if (!strcmp (input, "tesr"))
    {
      ut_print_message (0, 1, "Loading raster tessellation...\n");
      neut_tesr_fnscanf (file, pTesr);
      (*pTesr).pSim = pSim;
    }

    else if (!strcmp (input, "gmsh:msh"))
    {
      ut_print_message (0, 1, "Loading mesh...\n");
      neut_mesh_fnscanf_msh (file, pNodes, Mesh, Mesh + 1, Mesh + 2,
                             Mesh + 3, Mesh + 4, &topology, "r");
      (*pNodes).pSim = pSim;
      for (i = 0; i <= 4; i++)
        Mesh[i].pSim = pSim;

      if (topology && Mesh[3].EltQty > 0)
      {
        ut_print_message (0, 1, "Reconstructing mesh...\n");
        // does not work for hex meshes, due to topological issues
        if (!strcmp (Mesh[neut_mesh_array_dim (Mesh)].EltType, "tri")
            && (*pTesr).Dim == 0 && (*pTess).Dim == 0)
          nem_reconstruct_mesh ("3,2,1,0", pNodes, Mesh, pTess);
      }

      for (j = 0; j <= 3; j++)
        if (!neut_mesh_isvoid (Mesh[j]) && !(Mesh[j].NodeElts))
          neut_mesh_init_nodeelts (Mesh + j, (*pNodes).NodeQty);
    }

    else                      // point
    {
      (*pPointQty)++;
      (*pPoints) = (struct POINT *) realloc (*pPoints, (*pPointQty) * sizeof (struct POINT));
      neut_point_set_zero ((*pPoints) + (*pPointQty) - 1);

      char *fct = NULL;
      int varqty;
      char **vals = NULL, **vars = NULL;

      ut_string_function (input, &fct, &vars, &vals, &varqty);

      ut_print_message (0, 1, "Loading %s...\n", strlen (input) > 0 ? fct : "points");

      ut_string_string (fct, &((*pPoints)[(*pPointQty) - 1].Name));
      ut_string_string ("none", &((*pPoints)[(*pPointQty) - 1].Type));
      ut_string_string ("triclinic", &((*pPoints)[(*pPointQty) - 1].crysym));

      for (j = 0; j < varqty; j++)
      {
        if (!strcmp (vars[j], "type"))
          ut_string_string (vals[j], &((*pPoints)[(*pPointQty) - 1].Type));
        else if (!strcmp (vars[j], "crysym"))
          ut_string_string (vals[j], &((*pPoints)[(*pPointQty) - 1].crysym));
        else
          ut_print_exprbug (input);
      }

      if (!strcmp ((*pPoints)[(*pPointQty) - 1].Type, "ori"))
      {
        struct OL_SET OSet;

        ol_set_zero (&OSet);

        net_ori_file (file, &OSet);

        (*pPoints)[(*pPointQty) - 1].Dim = 4;
        (*pPoints)[(*pPointQty) - 1].Qty = OSet.size;
        (*pPoints)[(*pPointQty) - 1].Coo = ut_alloc_2d (OSet.size + 1, 4);
        ut_array_2d_memcpy (OSet.q, OSet.size, 4, (*pPoints)[(*pPointQty) - 1].Coo + 1);

        ol_set_free (&OSet);
      }

      else
        neut_point_fnscanf (file, (*pPoints) + (*pPointQty) - 1);

      (*pPoints)[(*pPointQty) - 1].pSim = pSim;

      ut_free_1d_char (&fct);
      ut_free_2d_char (&vals, varqty);
      ut_free_2d_char (&vars, varqty);
    }

    ut_free_2d_char (&list2, qty2);
    ut_free_1d_char (&input);
    ut_free_1d_char (&file);
  }

  neut_sim_testinputs (*pSim, *pTess, Mesh);

  ut_free_2d_char (&list, qty);

  if (!neut_tess_isvoid (*pTess))
    for (dim = 0; dim <= 5; dim++)
      nev_load_init_data_tess (*pTess, dim, TessData + dim);

  if (!neut_tesr_isvoid (*pTesr))
    for (i = 0; i <= 1; i++)
      nev_load_init_data_tesr (*pTesr, i, TesrData + i);

  if (!neut_nodes_isvoid (*pNodes))
  {
    nev_load_init_data_node (*pNodes, pNodeData);
    for (dim = 0; dim <= 3; dim++)
    {
      nev_load_init_data_mesh (Mesh[dim], "elt", MeshData[dim]);
      nev_load_init_data_mesh (Mesh[dim], "elset", MeshData[dim] + 1);
      nev_load_init_data_mesh (Mesh[dim], "mesh", MeshData[dim] + 2);
    }
  }

  (*pPointData) = calloc ((*pPointQty), sizeof (struct DATA));
  for (i = 0; i < *pPointQty; i++)
  {
    neut_data_set_default ((*pPointData) + i);
    if (!neut_point_isvoid ((*pPoints)[i]))
      nev_load_init_data_point ((*pPoints)[i], (*pPointData) + i);
  }

  return;
}
