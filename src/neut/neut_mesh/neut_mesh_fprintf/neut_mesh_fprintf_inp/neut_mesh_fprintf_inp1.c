/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_inp_.h"

void
neut_mesh_fprintf_inp (FILE * file, char *dim,
                       struct TESS Tess, struct NODES Nodes,
		       struct MESH Mesh2D, struct MESH Mesh3D,
		       struct MESH MeshCo,
		       struct NSET NSet0D, struct NSET NSet1D,
		       struct NSET NSet2D, char *nset, char *faset,
		       struct PART Part, struct BOUNDARY Bound)
{
  int i, j;
  int shift_elt2D = 0, shift_elt3D = 0, shift_eltCo = 0;

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
      fprintf (file, ", %.12f",
	       (fabs (Nodes.NodeCoo[i][j]) <
		1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (file, "\n");
  }
  fprintf (file, "\n");

/*----------------------------------------------------------------------
 * elements */

// 2D elts -------------------------------------------------------------

  shift_elt2D = 0;
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2") && Mesh2D.EltQty > 0)
    neut_mesh_fprintf_inp_mesh2d (file, Mesh2D, shift_elt2D);

// 3D elts -------------------------------------------------------------

  shift_elt3D = 0;
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2"))
    shift_elt3D += Mesh2D.EltQty;
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "3") && Mesh3D.EltQty > 0)
    neut_mesh_fprintf_inp_mesh3d (file, Mesh3D, shift_elt3D);

// Cohesive elts -------------------------------------------------------

  shift_eltCo = 0;
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2"))
    shift_eltCo += Mesh2D.EltQty;
  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "3"))
    shift_eltCo += Mesh3D.EltQty;

  if (MeshCo.EltQty > 0)
  {
    shift_eltCo = 0;
    if (MeshCo.Dimension == 2)
      shift_eltCo = shift_elt2D + Mesh2D.EltQty;
    else if (MeshCo.Dimension == 3)
      shift_eltCo = shift_elt3D + Mesh3D.EltQty;

    if (MeshCo.Dimension == 2)
      neut_mesh_fprintf_inp_mesh2d (file, MeshCo, shift_eltCo);
    if (MeshCo.Dimension == 3)
      neut_mesh_fprintf_inp_mesh3d (file, MeshCo, shift_eltCo);
  }

// 2D elsets -----------------------------------------------------------

  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "2"))
    neut_mesh_fprintf_inp_elsets (file, Mesh2D, "face", shift_elt2D);

// 3D elsets -----------------------------------------------------------

  if (ut_string_inlist (dim, NEUT_SEP_NODEP, "3"))
    neut_mesh_fprintf_inp_elsets (file, Mesh3D, "poly", shift_elt3D);

// 3D elsets -----------------------------------------------------------

  if (MeshCo.EltQty > 0)
    neut_mesh_fprintf_inp_elsets (file, MeshCo, "bound", shift_eltCo);

  if (Part.qty > 0)
    neut_mesh_fprintf_inp_part_elset (file, Part);

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

  if (Part.qty > 0)
    neut_mesh_fprintf_inp_part_nset (file, Part);

// fasets --------------------------------------------------------------

  if (faset)
    neut_mesh_fprintf_inp_fasets (file, Tess, Mesh2D, Mesh3D, Nodes, faset);

// foot ----------------------------------------------------------------

  if (Bound.BoundQty <= 0)
    fprintf (file, "*End Part\n");

  if (Bound.BoundQty > 0 && faset && ut_string_inlist (faset, NEUT_SEP_NODEP, "internal"))
  {
    neut_mesh_fprintf_inp_boundary (file, Bound);

    fprintf (file, "*End Assembly\n");
  }

  return;
}
