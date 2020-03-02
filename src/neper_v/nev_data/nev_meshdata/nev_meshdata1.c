/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_meshdata_.h"

void
nev_meshdata_init (struct MESH Mesh, double size, int Qty,
                   struct MESHDATA *pMeshData)
{
  if (Qty == 0)
    ut_print_neperbug ();

  if (Mesh.EltQty == 0)
    return;

  if (!(*pMeshData).Col)
    (*pMeshData).Col = ut_alloc_2d_int ((*pMeshData).EltQty + 1, 3);

  if (Mesh.Dimension == 0)
    ut_array_2d_int_set ((*pMeshData).Col + 1, (*pMeshData).EltQty, 3, 0);
  else if (Mesh.Dimension == 1)
    ut_array_2d_int_set ((*pMeshData).Col + 1, (*pMeshData).EltQty, 3, 0);
  else if (Mesh.Dimension == 2)
    ut_array_2d_int_set ((*pMeshData).Col + 1, (*pMeshData).EltQty, 3, 255);
  else if (Mesh.Dimension == 3)
    ut_array_2d_int_set ((*pMeshData).Col + 1, (*pMeshData).EltQty, 3, 255);

  if (!(*pMeshData).Rad)
    (*pMeshData).Rad = ut_alloc_1d ((*pMeshData).EltQty + 1);

  if (Mesh.Dimension == 0)
    ut_array_1d_set ((*pMeshData).Rad + 1, (*pMeshData).EltQty,
                     pow (size,
                          1. / 3) * 0.02 * sqrt (Mesh.Dimension +
                                                 1) / pow (Qty, 0.25));
  else if (Mesh.Dimension == 1)
    ut_array_1d_set ((*pMeshData).Rad + 1, (*pMeshData).EltQty,
                     pow (size, 1. / 3) * 0.01414 / pow (Qty, 0.25));
  else if (Mesh.Dimension == 2)
    ut_array_1d_set ((*pMeshData).Rad + 1, (*pMeshData).EltQty,
                     pow (size, 1. / 3) * 0.01000 / pow (Qty, 0.25));
  else if (Mesh.Dimension == 3)
    ut_array_1d_set ((*pMeshData).Rad + 1, (*pMeshData).EltQty,
                     pow (size, 1. / 3) * 0.00707 / pow (Qty, 0.25));

  if (!(*pMeshData).BCol)
  {
    (*pMeshData).BCol = ut_alloc_1d_int (3);

    if (Mesh.Dimension == 0)
      ut_array_1d_int_set ((*pMeshData).BCol, 3, 0);
    else if (Mesh.Dimension == 1)
      ut_array_1d_int_set ((*pMeshData).BCol, 3, 0);
    else if (Mesh.Dimension == 2)
      ut_array_1d_int_set ((*pMeshData).BCol, 3, 0);
    else if (Mesh.Dimension == 3)
      ut_array_1d_int_set ((*pMeshData).BCol, 3, 0);
  }

  if ((*pMeshData).BRad < 0)
  {
    if (Mesh.Dimension == 0)
      (*pMeshData).BRad =
        pow (size, 1. / 3) * 0.02 / pow (Mesh.ElsetQty, 0.25);
    else if (Mesh.Dimension == 1)
      (*pMeshData).BRad =
        pow (size, 1. / 3) * 0.01414 / pow (Mesh.ElsetQty, 0.25);
    else if (Mesh.Dimension == 2)
      (*pMeshData).BRad =
        pow (size, 1. / 3) * 0.01000 / pow (Mesh.ElsetQty, 0.25);
    else if (Mesh.Dimension == 3)
      (*pMeshData).BRad =
        pow (size, 1. / 3) * 0.00707 / pow (Mesh.ElsetQty, 0.25);
  }

  if ((*pMeshData).ColDataType)
  {
    if (!strcmp ((*pMeshData).ColDataType, "from_nodes"))
    {
    }                           // do nothing

    else if (!strcmp ((*pMeshData).ColDataType, "id"))
      nev_data_id_colour ((*pMeshData).ColData, (*pMeshData).EltQty,
                          (*pMeshData).Col);

    else if (!strcmp ((*pMeshData).ColDataType, "col"))
      nev_data_col_colour ((*pMeshData).ColData, (*pMeshData).EltQty,
                           (*pMeshData).Col);

    else if (!strncmp ((*pMeshData).ColDataType, "ori", 3))
      nev_data_ori_colour ((*pMeshData).ColData, (*pMeshData).EltQty,
                           (*pMeshData).ColScheme, (*pMeshData).Col);

    else if (!strcmp ((*pMeshData).ColDataType, "scal"))
      nev_data_scal_colour ((*pMeshData).ColData, NULL, (*pMeshData).EltQty,
                            (*pMeshData).Scale, (*pMeshData).ColScheme,
                            (*pMeshData).Col, &((*pMeshData).Scale));

    else
      ut_print_exprbug ((*pMeshData).ColDataType);
  }

  if ((*pMeshData).RadData)
    nev_data_rad_radius ((*pMeshData).RadData, (*pMeshData).EltQty,
                         (*pMeshData).Rad);

  return;
}

void
nev_meshdata_fscanf (struct MESH *Mesh, char *entity, char *type,
                     char *argument, struct MESHDATA *MeshData)
{
  int dim, elset;
  struct MESHDATA MeshDataSet;
  struct MESHDATA *pMD = NULL;
  char *entity2 = ut_alloc_1d_char (10);

  ut_print_message (0, 1, "Reading data (%s, %s)...\n", entity, type);

  neut_meshdata_set_default (&MeshDataSet);

  elset = (strlen (entity) >= 5 && !strncmp (entity, "elset", 5));

  entity2 = ut_alloc_1d_char (strlen (entity) + 1);
  strcpy (entity2, entity);
  if (elset)
    ut_string_fnrs (entity2, "elset", "elt", 1);

  dim = entity2[3] - '0';

  if (!elset)
    pMD = MeshData + dim;
  else
  {
    MeshDataSet.Dimension = Mesh[dim].Dimension;
    MeshDataSet.EltQty = Mesh[dim].ElsetQty;
    pMD = &MeshDataSet;
  }

  // element data: elt0d, elt1d, elt2d or elt3d
  if (strlen (entity2) == 5)
    nev_meshdata_fscanf_elt (Mesh[dim], pMD, type, argument, elset);

  // element boundary data
  else
    nev_meshdata_fscanf_eltb (pMD, type, argument);

  // copying elset data in elts
  if (elset)
    nev_meshdata_elset2elt (Mesh[dim], entity, type, MeshDataSet,
                            MeshData + dim);

  neut_meshdata_free (&MeshDataSet);
  ut_free_1d_char (&entity2);

  return;
}
