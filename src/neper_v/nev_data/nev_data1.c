/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_.h"

void
nev_data (char **argv, int *pi, struct TESS Tess, struct TESR Tesr,
          struct MESH *Mesh, struct TESSDATA *pTessData,
          struct TESRDATA *pTesrData, struct NODEDATA *pNodeData,
          struct MESHDATA *MeshData, struct CSYSDATA *pCsysData,
          struct POINTDATA *pPointData)
{
  int dim;
  char *entity = ut_alloc_1d_char (100);
  char *type = ut_alloc_1d_char (100);

  dim = neut_mesh_array_dim (Mesh);
  nev_data_string_entity_type (argv[(*pi)], entity, type);

  if (!strcmp (entity, "elt"))
    sprintf (entity, "elt%dd", dim);
  if (!strcmp (entity, "elset"))
    sprintf (entity, "elset%dd", dim);

  if (!strcmp (entity, "poly") || !strcmp (entity, "edge")
      || !strcmp (entity, "face") || !strcmp (entity, "ver")
      || !strcmp (entity, "cell") || !strcmp (entity, "seed"))
  {
    if ((*pTessData).VerQty > 0)
      nev_tessdata_fscanf (Tess, entity, type, argv[++(*pi)], pTessData);
    else if ((*pTesrData).Qty > 0)
      nev_tesrdata_fscanf (Tesr, entity, type, argv[++(*pi)], pTesrData);
    else
      abort ();
  }
  else if (!strncmp (entity, "vox", 3))
    nev_tesrdata_fscanf (Tesr, entity, type, argv[++(*pi)], pTesrData);
  else if (!strncmp (entity, "elt", 3) || !strncmp (entity, "elset", 5))
    nev_meshdata_fscanf (Mesh, entity, type, argv[++(*pi)], MeshData);

  else if (!strcmp (entity, "node"))
    nev_nodedata_fscanf (type, argv[++(*pi)], pNodeData);

  else if (!strcmp (entity, "csys"))
    nev_csysdata_fscanf (type, argv[++(*pi)], pCsysData);

  else if (!strcmp (entity, "point"))
    nev_pointdata_fscanf (type, argv[++(*pi)], pPointData);

  else
    ut_print_message (1, 0, "Unknown entity `%s'.  Skipping...\n", entity);

  ut_free_1d_char (&entity);
  ut_free_1d_char (&type);

  return;
}

void
nev_data_init (struct TESS Tess, struct TESSDATA *pTessData, struct TESR Tesr,
               struct TESRDATA *pTesrData, struct NODES Nodes,
               struct MESH *Mesh, struct POINT Point,
               struct NODEDATA *pNodeData, struct MESHDATA *MeshData,
               struct CSYSDATA *pCsysData, struct POINTDATA *pPointData)
{
  int dim, meshdim;
  double size;

  if (Tess.Dim == 0 && Tess.CellQty < 1)
    Tess.CellQty = 1;

  if ((*pTessData).VerQty > 0)
    nev_tessdata_init (Tess, pTessData);

  if ((*pTesrData).Qty > 0)
    nev_tesrdata_init (Tesr, pTesrData);

  if (Nodes.NodeQty > 0)
    nev_nodedata_init (Nodes, Tess.CellQty, pNodeData);

  meshdim = neut_mesh_array_dim (Mesh);
  if (meshdim > 0)
  {
    neut_mesh_size (Nodes, Mesh[meshdim], &size);

    for (dim = 0; dim <= 3; dim++)
      nev_meshdata_init (Mesh[dim], size, Tess.CellQty, MeshData + dim);
  }

  nev_csysdata_init (pCsysData);

  if (Point.PointQty > 0)
    nev_pointdata_init (Point, pPointData);

  return;
}
