/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_meshing_str_.h"

void
nem_meshing_tess_str (struct IN_M In, struct MESHPARA MeshPara,
		      struct TESS Tess, struct NODES *pNodes,
		      struct MESH *Mesh, struct NSET *NSet)
{
  int i;
  int *msize = ut_alloc_1d_int (3);
  double **bbox = ut_alloc_2d (3, 2);
  double *bboxsize = ut_alloc_1d (3);
  double cl;

  neut_tess_bbox (Tess, bbox);
  neut_tess_bboxsize (Tess, bboxsize);

  if (Tess.Dim == 3)
    cl = MeshPara.poly_cl[1];
  else if (Tess.Dim == 2)
    cl = MeshPara.face_cl[1];
  else
  {
    ut_error_reportbug ();
    abort ();			// for warnings
  }

  for (i = 0; i < 3; i++)
    msize[i] = ut_num_d2ri (bboxsize[i] / cl);

  // neut_utils_nset_expand (In.nset, &expandnset);
  // neut_utils_nset_expand (In.faset, &expandfaset);

  /* Creating mapped mesh ------------------------------------------ */
  printf ("\n");
  ut_print_message (0, 2, "%dD meshing... ", Tess.Dim);

  neut_mesh_str (Tess.Dim, msize, pNodes, Mesh + Tess.Dim,
		 NSet + Tess.Dim - 1);
  neut_nodes_scale (pNodes, bboxsize[1], bboxsize[1], bboxsize[1]);
  neut_nodes_shift (pNodes, bbox[0][0], bbox[1][0], bbox[2][0]);

  /* Searching elsets ---------------------------------------------- */

  // ut_print_message (0, 2, "Searching elsets... ");
  nem_meshing_str_tess (Tess, pNodes, Mesh);

  neut_mesh_init_elsets (Mesh + Tess.Dim);
  neut_mesh_init_elsetlabels (Mesh + Tess.Dim);

  nem_reconstruct_mesh (In.dimout, pNodes, Mesh, NULL);

  int *poly = ut_alloc_1d_int (Tess.PolyQty + 1);
  int polyqty;

  ut_free_2d_int (Mesh[Tess.Dim].NodeElts, (*pNodes).NodeQty + 1);
  neut_mesh_init_nodeelts (Mesh + Tess.Dim, (*pNodes).NodeQty);

  if (strcmp (In.meshpoly, "all") != 0)
  {
    if (Mesh[Tess.Dim].ElsetId)
      ut_error_reportbug ();

    ut_print_message (0, 2, "Removing elsets other than `%s'... \n",
		      In.meshpoly);
    neut_tess_expr_polylist (Tess, In.meshpoly, &poly, &polyqty);
    for (i = 1; i <= Tess.PolyQty; i++)
      if (ut_array_1d_int_eltpos (poly, polyqty, i) == -1)
	neut_mesh_rmelset (Mesh + Tess.Dim, *pNodes, i);
  }

  // ut_free_1d_char (expandnset);
  // ut_free_1d_char (expandfaset);
  ut_free_1d_int (msize);
  ut_free_2d (bbox, 3);
  ut_free_1d (bboxsize);

  return;
}

void
nem_meshing_tesr_str (struct IN_M In, struct MESHPARA MeshPara,
		      struct TESR Tesr, struct NODES *pNodes,
		      struct MESH *Mesh, struct NSET *NSet)
{
  int i, j, k, elt;
  double cl;
  struct TESR Tesr2;
  int *msize = ut_alloc_1d_int (3);
  double *scale = ut_alloc_1d (3);
  double **bbox = ut_alloc_2d (3, 2);
  double *bboxsize = ut_alloc_1d (3);

  neut_tesr_bbox (Tesr, bbox);
  neut_tesr_bboxsize (Tesr, bboxsize);

  if (Tesr.Dim == 3)
    cl = MeshPara.poly_cl[1];
  else if (Tesr.Dim == 2)
    cl = MeshPara.face_cl[1];
  else if (Tesr.Dim == 1)
    cl = MeshPara.edge_cl[1];
  else
    abort ();

  for (i = 0; i < 3; i++)
  {
    msize[i] = ut_num_d2ri ((bbox[i][1] - bbox[i][0]) / cl);
    msize[i] = ut_num_max_int (msize[i], 1);
  }

  // Creating mapped mesh
  printf ("\n");
  ut_print_message (0, 2, "%dD meshing... ", Tesr.Dim);

  neut_mesh_str (Tesr.Dim, msize, pNodes, Mesh + Tesr.Dim,
		 NSet + Tesr.Dim - 1);

  neut_nodes_scale (pNodes, bboxsize[1], bboxsize[1], Tesr.Dim == 3 ? bboxsize[1] : cl);
  neut_nodes_shift (pNodes, bbox[0][0], bbox[1][0], bbox[2][0]);

  printf ("\n");

  ut_array_1d_set (scale, 3, 1);
  for (i = 0; i < Tesr.Dim; i++)
    scale[i] = (double) msize[i] / Tesr.size[i];

  neut_tesr_set_zero (&Tesr2);
  neut_tesr_memcpy (Tesr, &Tesr2);
  neut_tesr_rasterscale (&Tesr2, scale[0], scale[1], scale[2]);

  elt = 0;
  for (k = 1; k <= msize[2]; k++)
    for (j = 1; j <= msize[1]; j++)
      for (i = 1; i <= msize[0]; i++)
	Mesh[Tesr.Dim].EltElset[++elt] = Tesr2.VoxCell[i][j][k];

  neut_mesh_init_elsets (Mesh + Tesr.Dim);
  neut_mesh_init_elsetlabels (Mesh + Tesr.Dim);

  neut_mesh_rmelset (Mesh + Tesr.Dim, *pNodes, 0);

  neut_nodes_rmorphans (pNodes, Mesh + Tesr.Dim, NSet + Tesr.Dim - 1);

  if (Tesr.CellId)
  {
    Mesh[Tesr.Dim].ElsetId = ut_alloc_1d_int (Tesr.CellQty + 1);
    ut_array_1d_int_memcpy (Mesh[Tesr.Dim].ElsetId + 1, Tesr.CellQty,
			    Tesr.CellId + 1);
  }

  if (In.meshpoly && strcmp (In.meshpoly, "all") != 0)
  {
    if (Mesh[Tesr.Dim].ElsetId)
      ut_error_reportbug ();

    nem_meshing_str_meshcell (In.meshpoly, Tesr, Mesh, pNodes, NSet);
  }

  nem_reconstruct_mesh (In.dimout, pNodes, Mesh, NULL);

  ut_free_1d_int (msize);
  ut_free_1d (scale);
  neut_tesr_free (&Tesr2);

  return;
}
