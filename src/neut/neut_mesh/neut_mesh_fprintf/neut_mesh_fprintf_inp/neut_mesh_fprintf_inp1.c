/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_inp_.h"

void
neut_mesh_fprintf_inp (FILE * file, char *dim, struct TESS Tess,
                       struct NODES Nodes, struct MESH Mesh0D,
                       struct MESH Mesh1D, struct MESH Mesh2D,
                       struct MESH Mesh3D, struct MESH MeshCo,
                       struct NSET NSet0D, struct NSET NSet1D,
                       struct NSET NSet2D, char *nset, char *faset,
                       struct BOUNDARY Bound)
{
  int i, j, meshdim;
  int *shift = ut_alloc_1d_int (5);

  if (Mesh3D.EltQty > 0)
    meshdim = 3;
  else if (Tess.Dim > 0)
    meshdim = Tess.Dim;
  else if (Nodes.NodeQty > 0)
    meshdim = neut_nodes_dim (Nodes);
  else
    meshdim = 0;

/*----------------------------------------------------------------------
 * header */
  fprintf (file, "*Part, name=tess\n\n");

/*----------------------------------------------------------------------
 * nodes */

  fprintf (file, "*Node\n");
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "%d", i);
    for (j = 0; j < 3; j++)
      fprintf (file, ", " REAL_PRINT_FORMAT,
               (fabs (Nodes.NodeCoo[i][j]) <
                1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (file, "\n");
  }
  fprintf (file, "\n");

/*----------------------------------------------------------------------
 * elements (numbered contiguously over the dimensions, as in msh) */

  neut_mesh_fprintf_inp_shift (dim, Mesh0D, Mesh1D, Mesh2D, Mesh3D, MeshCo,
                               shift);

// 0D elts -------------------------------------------------------------

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "0") && Mesh0D.EltQty > 0)
    neut_mesh_fprintf_inp_mesh0d (file, Mesh0D, shift[0]);

// 1D elts -------------------------------------------------------------

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1") && Mesh1D.EltQty > 0)
    neut_mesh_fprintf_inp_mesh1d (file, Mesh1D, shift[1], meshdim);

// 2D elts -------------------------------------------------------------

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2") && Mesh2D.EltQty > 0)
    neut_mesh_fprintf_inp_mesh2d (file, Mesh2D, shift[2],
                                  meshdim == 3 ? "surface" : "standard");

// 3D elts -------------------------------------------------------------

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3") && Mesh3D.EltQty > 0)
    neut_mesh_fprintf_inp_mesh3d (file, Mesh3D, shift[3], "standard");

// Cohesive elts -------------------------------------------------------

  if (MeshCo.EltQty > 0)
  {
    if (MeshCo.Dimension == 2)
      neut_mesh_fprintf_inp_mesh2d (file, MeshCo, shift[4], "cohesive");
    if (MeshCo.Dimension == 3)
      neut_mesh_fprintf_inp_mesh3d (file, MeshCo, shift[4], "cohesive");
  }

// 0D elsets -----------------------------------------------------------

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "0"))
    neut_mesh_fprintf_inp_elsets (file, Mesh0D, shift[0]);

// 1D elsets -----------------------------------------------------------

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "1"))
    neut_mesh_fprintf_inp_elsets (file, Mesh1D, shift[1]);

// 2D elsets -----------------------------------------------------------

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "2"))
    neut_mesh_fprintf_inp_elsets (file, Mesh2D, shift[2]);

// 3D elsets -----------------------------------------------------------

  if (ut_list_testelt (dim, NEUT_SEP_NODEP, "3"))
    neut_mesh_fprintf_inp_elsets (file, Mesh3D, shift[3]);

// Cohesive elsets -----------------------------------------------------

  if (MeshCo.EltQty > 0)
    neut_mesh_fprintf_inp_elsets (file, MeshCo, shift[4]);

// node sets -----------------------------------------------------------

  if (Bound.BoundQty > 0)
  {
    fprintf (file, "*End Part\n");
    fprintf (file, "\n");
    fprintf (file, "*Assembly, name=tess\n");
    fprintf (file, "*Instance, name=tess, part=tess\n");
    fprintf (file, "*End Instance\n");
  }

  if (nset != NULL)
    neut_mesh_fprintf_inp_nsets (file, NSet0D, NSet1D, NSet2D, nset,
                                 (Bound.BoundQty > 0) ? "tess" : NULL);

// fasets --------------------------------------------------------------

  if (faset)
    neut_mesh_fprintf_inp_fasets (file, Tess, Mesh2D, Mesh3D, Nodes, faset,
                                  shift[meshdim]);

// foot ----------------------------------------------------------------

  if (Bound.BoundQty <= 0)
    fprintf (file, "*End Part\n");

  if (Bound.BoundQty > 0 && faset
      && ut_list_testelt (faset, NEUT_SEP_NODEP, "internal"))
    neut_mesh_fprintf_inp_boundary (file, Bound, shift[meshdim]);

  if (Bound.BoundQty > 0)
    fprintf (file, "*End Assembly\n");

  ut_free_1d_int (&shift);

  return;
}
