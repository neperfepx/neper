/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_transport_.h"

void
nem_transport_elt (char *type, char *filename, char *method,
                   struct NODES RNodes, struct MESH RMesh, struct NODES Nodes,
                   struct MESH Mesh, int **poldelt)
{
  int j, dim;
  FILE *file = NULL;
  char *name = NULL;

  if (!(*poldelt))
    nem_transport_elt_oldelt (RNodes, RMesh, Nodes, Mesh, method, poldelt);

  if (sscanf (type, "real%d", &dim) == 1)
  {
    double **data = ut_alloc_2d (RMesh.EltQty + 1, dim);

    file = ut_file_open (filename, "r");
    for (j = 1; j <= RMesh.EltQty; j++)
      ut_array_1d_fscanf (file, data[j], dim);
    ut_file_close (file, filename, "r");

    name = ut_alloc_1d_char (strlen (filename) + 5);
    sprintf (name, "%s.rem", filename);

    file = ut_file_open (name, "w");
    for (j = 1; j <= Mesh.EltQty; j++)
      ut_array_1d_fprintf (file, data[(*poldelt)[j]], dim, "%11.6f");
    ut_file_close (file, name, "w");
    ut_free_1d_char (name);

    ut_free_2d (data, RMesh.EltQty + 1);
  }
  else if (sscanf (type, "int%d", &dim) == 1)
  {
    int **data = ut_alloc_2d_int (RMesh.EltQty + 1, dim);

    file = ut_file_open (filename, "r");
    for (j = 1; j <= RMesh.EltQty; j++)
      ut_array_1d_int_fscanf (file, data[j], dim);
    ut_file_close (file, filename, "r");

    name = ut_alloc_1d_char (strlen (filename) + 5);
    sprintf (name, "%s.rem", filename);

    file = ut_file_open (name, "w");
    for (j = 1; j <= Mesh.EltQty; j++)
      ut_array_1d_int_fprintf (file, data[(*poldelt)[j]], dim, "%d");
    ut_file_close (file, name, "w");
    ut_free_1d_char (name);

    ut_free_2d_int (data, RMesh.EltQty + 1);
  }

  return;
}

void
nem_transport_node (char *type, char *filename, struct NODES RNodes,
                    struct MESH RMesh, struct NODES Nodes)

{
  int i, j, k, dim, elt, status;
  FILE *file = NULL;
  char *name = NULL;
  double *shfct = ut_alloc_1d (3);
  double **coo = ut_alloc_2d (3, 3);

  if (RMesh.Dimension != 2)
    ut_error_reportbug ();

  if (sscanf (type, "real%d", &dim) == 1)
  {
    double *val = ut_alloc_1d (dim);
    double **data = ut_alloc_2d (RNodes.NodeQty + 1, dim);

    file = ut_file_open (filename, "r");
    ut_array_2d_fscanf (file, data + 1, RNodes.NodeQty, dim);
    ut_file_close (file, filename, "r");

    name = ut_alloc_1d_char (strlen (filename) + 5);
    sprintf (name, "%s.rem", filename);

    file = ut_file_open (name, "w");
    for (i = 1; i <= Nodes.NodeQty; i++)
    {
      status = neut_mesh_point_elt (RMesh, RNodes, Nodes.NodeCoo[i], &elt);

      if (status == -1)
	abort ();

      for (j = 0; j < 3; j++)
	ut_array_1d_memcpy (coo[j], 3, RNodes.NodeCoo[RMesh.EltNodes[elt][j]]);

      neut_elt_tri_shapefct (RNodes.NodeCoo[RMesh.EltNodes[elt][0]],
                             RNodes.NodeCoo[RMesh.EltNodes[elt][1]],
                             RNodes.NodeCoo[RMesh.EltNodes[elt][2]],
			     Nodes.NodeCoo[i], shfct);

      ut_array_1d_zero (val, dim);
      for (k = 0; k < dim; k++)
	for (j = 0; j < 3; j++)
	  val[k] += shfct[j] * data[RMesh.EltNodes[elt][j]][k];

      ut_array_1d_fprintf (file, val, dim, "%.12f");
    }
    ut_file_close (file, name, "w");
    ut_free_1d_char (name);

    ut_free_2d (data, RNodes.NodeQty + 1);
    ut_free_1d (val);
  }

  else
    abort ();

  ut_free_1d (shfct);
  ut_free_2d (coo, 3);

  return;
}

void
nem_transportfepx_fepxfiles (struct IN_M In, struct TESS Tess, struct MESH *Mesh,
                             char **transport)
{
  int i, dim = Tess.Dim;
  double *tmp = ut_alloc_1d (4);
  char *orikr = ut_string_addextension (transport[0], ".rem");
  char *taur = ut_string_addextension (transport[1], ".rem");
  char *kocks = ut_string_addextension (In.body, ".kocks");
  char *opt = ut_string_addextension (In.body, ".opt");
  FILE *fpin1 = NULL, *fpin2 = NULL, *fpout = NULL;

  // kocks file

  fpin1 = ut_file_open (orikr, "R");
  fpin2 = ut_file_open (taur, "R");
  fpout = ut_file_open (kocks, "w");

  fprintf (fpout, "grain-properties\n");
  fprintf (fpout, "%d\n", Mesh[dim].EltQty);
  for (i = 1; i <= Mesh[dim].EltQty; i++)
  {
    ol_e_fscanf (fpin1, tmp);
    if (fscanf (fpin2, "%lf", tmp + 3) != 1)
      abort ();
    ut_array_1d_fprintf_nonl (fpout, tmp, 4, "%.6f");
    fprintf (fpout, " %d\n", i);
  }
  fprintf (fpout, "EOF\n");
  ut_free_1d (tmp);

  ut_file_close (fpin1, orikr, "R");
  ut_file_close (fpin2, taur, "R");
  ut_file_close (fpout, kocks, "w");

  // opt file

  fpout = ut_file_open (opt, "w");

  fprintf (fpout, "grain-input\n");
  fprintf (fpout, "%d 3\n", Mesh[dim].EltQty);
  for (i = 1; i <= Mesh[dim].EltQty; i++)
    fprintf (fpout, "%d 1\n", i);
  fprintf (fpout, "end-options\n");

  ut_file_close (fpout, opt, "w");

  ut_free_1d_char (orikr);
  ut_free_1d_char (taur);
  ut_free_1d_char (kocks);
  ut_free_1d_char (opt);

  return;
}
