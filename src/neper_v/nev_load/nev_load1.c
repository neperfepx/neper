/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_load_.h"

void
nev_load (char *string, struct SIM *pSim, struct TESS *pTess, struct TESR *pTesr,
          struct NODES *pNodes, struct MESH *Mesh, struct POINT *pPoint,
          struct DATA *TessData, struct DATA *pTesrData,
          struct DATA *pNodeData, struct DATA *MeshData,
          struct DATA *pPointData)
{
  int i, j, qty, dim, isdir = ut_sys_isdir (string), topology;
  char **list = NULL;

  if (neut_tess_isvoid (*pTess) && neut_tesr_isvoid (*pTesr)
      && neut_nodes_isvoid (*pNodes) && neut_point_isvoid (*pPoint))
  {
    if (!isdir)
      ut_list_break (string, NEUT_SEP_NODEP, &list, &qty);

    else
    {
      neut_sim_fscanf (string, pSim, "R");

      qty = 0;
      list = ut_alloc_1d_pchar (3);
      if (ut_file_exist ("%s/inputs/%s", string, (*pSim).tess))
        list[qty++] = ut_string_paste3 (string, "/inputs/", (*pSim).tess);
      if (ut_file_exist ("%s/inputs/simulation.tesr", string))
        list[qty++] = ut_string_paste3 (string, "/inputs/", (*pSim).tesr);
      if (ut_file_exist ("%s/inputs/%s", string, (*pSim).msh))
        list[qty++] = ut_string_paste3 (string, "/inputs/", (*pSim).msh);
    }

    for (i = 0; i < qty; i++)
    {
      char *format = NULL;
      char **list2 = NULL;
      int qty2;

      ut_list_break (list[i], NEUT_SEP_DEP, &list2, &qty2);

      ut_file_format (list2[0], &format);

      if (!format)
        ut_print_message (2, 1, "Input file `%s' is void.\n", list2[0]);

      if (!strcmp (format, "tess"))
      {
        ut_print_message (0, 1, "Loading tessellation...\n");
        neut_tess_fnscanf (list[i], pTess);
        (*pTess).pSim = pSim;
      }
      else if (!strcmp (format, "tesr"))
      {
        ut_print_message (0, 1, "Loading raster tessellation...\n");
        neut_tesr_fnscanf (list[i], pTesr);
        (*pTesr).pSim = pSim;
      }
      else if (!strcmp (format, "gmsh:msh"))
      {
        ut_print_message (0, 1, "Loading mesh...\n");
        neut_mesh_fnscanf_msh (list[i], pNodes, Mesh, Mesh + 1, Mesh + 2,
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
        ut_print_message (0, 1, "Loading points...\n");
        neut_point_fnscanf (list[i], pPoint);
        (*pPoint).pSim = pSim;
      }
      /*
         ut_print_message (2, 0, "Unknown input file format (%s).",
         format);
       */

      ut_free_2d_char (&list2, qty2);
      ut_free_1d_char (&format);
    }

    ut_free_2d_char (&list, qty);
  }
  else
  {
    ut_print_message (2, 0, "Input file already loaded (reading `%s').\n",
                      string);
    abort ();
  }

  if (!neut_tess_isvoid (*pTess))
    for (dim = 0; dim <= 5; dim++)
      nev_load_init_data_tess (*pTess, dim, TessData + dim);

  if (!neut_tesr_isvoid (*pTesr))
    nev_load_init_data_tesr (*pTesr, pTesrData);

  if (!neut_nodes_isvoid (*pNodes))
  {
    nev_load_init_data_node (*pNodes, pNodeData);
    for (dim = 0; dim <= 3; dim++)
      nev_load_init_data_mesh (Mesh[dim], MeshData + dim);
  }

  if (!neut_point_isvoid (*pPoint))
    nev_load_init_data_point (*pPoint, pPointData);

  return;
}
