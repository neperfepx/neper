/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_load_.h"

void
nev_load (char *string, struct TESS *pTess, struct TESR *pTesr,
	  struct NODES *pNodes, struct MESH *Mesh, struct POINT *pPoint,
	  struct TESSDATA *pTessData, struct TESRDATA *pTesrData,
	  struct NODEDATA *pNodeData, struct MESHDATA *MeshData,
	  struct POINTDATA *pPointData)
{
  int i, j, qty, dim;
  char **list = NULL;

  if ((*pTess).VerQty == 0 && (*pTesr).CellQty == 0
      && (*pNodes).NodeQty == 0 && (*pPoint).PointQty == 0)
  {
    ut_string_separate (string, NEUT_SEP_NODEP, &list, &qty);

    for (i = 0; i < qty; i++)
    {
      char *format = NULL;
      char **list2 = NULL;
      int qty2;

      ut_string_separate (list[i], NEUT_SEP_DEP, &list2, &qty2);

      ut_file_format (list2[0], &format);

      if (!format)
	ut_print_message (2, 1, "Input file `%s' is void.\n", list2[0]);

      if (!strcmp (format, "tess"))
      {
	ut_print_message (0, 1, "Loading tessellation...\n");
	neut_tess_name_fscanf (list[i], pTess);
      }
      else if (!strcmp (format, "tesr"))
      {
	ut_print_message (0, 1, "Loading raster tessellation...\n");
	neut_tesr_name_fscanf (list[i], pTesr);
      }
      else if (!strcmp (format, "gmsh_msh"))
      {
	ut_print_message (0, 1, "Loading mesh...\n");
	neut_mesh_name_fscanf_msh (list[i], pNodes, Mesh, Mesh + 1, Mesh + 2,
				   Mesh + 3, Mesh + 4);

	if (Mesh[3].EltQty > 0)
	{
	  ut_print_message (0, 1, "Reconstructing mesh...\n");
          // does not work for hex meshes, due to topological issues
	  if (!strcmp (Mesh[neut_mesh_array_dim (Mesh)].EltType,
		       "tri") && (*pTesr).Dim == 0 && (*pTess).Dim == 0)
	    nem_reconstruct_mesh ("3,2,1,0", pNodes, Mesh, pTess);
	}

        for (j = 0; j <= 3; j++)
          if (!neut_mesh_isvoid (Mesh[j]) && !(Mesh[j].NodeElts))
            neut_mesh_init_nodeelts (Mesh + j, (*pNodes).NodeQty);
      }
      else			// point
      {
	ut_print_message (0, 1, "Loading points...\n");
	neut_point_name_fscanf (list[i], pPoint);
      }
      /*
         ut_print_message (2, 0, "Unknown input file format (%s).",
         format);
       */

      ut_free_2d_char (list2, qty2);
      ut_free_1d_char (format);
    }

    ut_free_2d_char (list, qty);
  }
  else
  {
    ut_print_message (2, 0, "Input file already loaded (reading `%s').\n",
		      string);
    abort ();
  }

  if ((*pTess).VerQty > 0)
    nev_load_init_tessdata (*pTess, pTessData);

  if ((*pTesr).CellQty > 0)
    nev_load_init_tesrdata (*pTesr, pTesrData);

  if ((*pNodes).NodeQty > 0)
  {
    nev_load_init_nodedata (*pNodes, pNodeData);
    for (dim = 0; dim <= 3; dim++)
      nev_load_init_meshdata (Mesh[dim], &(MeshData[dim]));
  }

  if ((*pPoint).PointQty > 0)
    nev_load_init_pointdata (*pPoint, pPointData);

  return;
}
