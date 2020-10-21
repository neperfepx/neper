/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_mesh_.h"

struct DATA *
nev_data_mesh_init (struct MESH *Mesh, struct DATA *MeshData,
                    char **pentity, char *attribute, char *datastring,
                    int *pdim, int *pentityqty, char **ptype,
                    char **pvalue, struct DATA *pDataSet)
{
  struct DATA *pData = NULL;

  ut_print_message (0, 1, "Reading data (%s, %s)...\n", *pentity, attribute);

  if (strstr (*pentity, "edge"))
  {
    (*pdim) = (*pentity)[strlen (*pentity) - 6] - '0';
    *pentityqty = 1;
    pData = MeshData + (*pdim);
  }
  else if (!strncmp (*pentity, "elset", 5))
  {
    (*pdim) = (*pentity)[strlen (*pentity) - 2] - '0';
    ut_string_fnrs (*pentity, "elset", "elt", 1);
    (*pDataSet).Dim = Mesh[*pdim].Dimension;
    (*pDataSet).Qty = Mesh[*pdim].ElsetQty;
    pData = pDataSet;
    *pentityqty = (*pData).Qty;
  }
  else // elt
  {
    (*pdim) = (*pentity)[strlen (*pentity) - 2] - '0';
    pData = MeshData + (*pdim);
    *pentityqty = (*pData).Qty;
  }

  if (*pdim < 0 || *pentityqty < 0)
    abort ();

  neut_data_datastring_type_value (*pentity, attribute, datastring, ptype, pvalue);

  return pData;
}

void
nev_data_mesh_elt (struct SIM Sim, struct TESS *pTess, struct NODES *pNodes,
                          struct MESH **pMesh, char *entity,
                          int dim, int entityqty, char *attribute, char *type,
                          char *value, struct DATA *pData)
{
  if (!strcmp (attribute, "col"))
    neut_data_fscanf_col (Sim, pTess, pNodes, pMesh, NULL, "mesh", entity, dim,
                         entityqty, type, value, pData);

  else if (!strcmp (attribute, "rad"))
    nev_data_mesh_elt_rad (value, pData);

  else if (!strcmp (attribute, "colscheme"))
    ut_string_string (value, &(*pData).ColScheme);

  else if (!strcmp (attribute, "scale"))
    ut_string_string (value, &(*pData).Scale);

  else if (!strcmp (attribute, "scaletitle"))
    ut_string_string (value, &(*pData).ScaleTitle);

  else
    ut_print_exprbug (attribute);

  return;
}

void
nev_data_mesh_eltedge (char *attribute, char *value, struct DATA *pData)
{
  if (!strcmp (attribute, "col"))
  {
    (*pData).BCol = ut_alloc_1d_int (3);
    ut_array_1d_int_fnscanf_wcard (value, (*pData).BCol, 3, "color", "r");
  }
  else if (!strcmp (attribute, "rad"))
    (*pData).BRad = atof (value);
  else
    ut_print_exprbug (attribute);

  return;
}

void
nev_data_mesh_elset2elt (struct MESH Mesh, char *entity, char *attribute,
                        struct DATA MeshDataSet, struct DATA *pData)
{
  int i, j, size;

  if (entity[strlen (entity) - 1] != 'b')
  {
    if (!strcmp (attribute, "col"))
    {
      ut_string_string (MeshDataSet.ColDataType, &(*pData).ColDataType);

      size = -1;
      if (!strcmp ((*pData).ColDataType, "int"))
        size = 1;
      else if (!strcmp ((*pData).ColDataType, "col")
               || !strncmp ((*pData).ColDataType, "ori", 3))
        size = 4;
      else if (!strcmp ((*pData).ColDataType, "rad"))
        size = 1;
      else if (!strcmp ((*pData).ColDataType, "real"))
        size = 1;
      else
        abort ();

      (*pData).ColData = ut_alloc_2d ((*pData).Qty + 1, size);
      for (i = 1; i <= Mesh.ElsetQty; i++)
        for (j = 1; j <= Mesh.Elsets[i][0]; j++)
          ut_array_1d_memcpy (MeshDataSet.ColData[i], size,
                              (*pData).ColData[Mesh.Elsets[i][j]]);
    }
    else if (!strcmp (attribute, "rad"))
    {
      ut_string_string (MeshDataSet.RadDataType, &(*pData).RadDataType);

      (*pData).RadData = ut_alloc_2d ((*pData).Qty + 1, 1);
      for (i = 1; i <= Mesh.ElsetQty; i++)
        for (j = 1; j <= Mesh.Elsets[i][0]; j++)
          (*pData).RadData[Mesh.Elsets[i][j]][0] = MeshDataSet.RadData[i][0];
    }
    else if (!strcmp (attribute, "colscheme"))
      ut_string_string (MeshDataSet.ColScheme, &(*pData).ColScheme);
    else if (!strcmp (attribute, "scale"))
      ut_string_string (MeshDataSet.Scale, &(*pData).Scale);
    else if (!strcmp (attribute, "scaletitle"))
      ut_string_string (MeshDataSet.ScaleTitle, &(*pData).ScaleTitle);
    else
      ut_print_exprbug (attribute);
  }
  else
  {
    if (!strcmp (attribute, "col"))
    {
      (*pData).BCol = ut_alloc_1d_int (3);
      ut_array_1d_int_memcpy (MeshDataSet.BCol, 3, (*pData).BCol);
    }
    else if (!strcmp (attribute, "rad"))
      (*pData).BRad = MeshDataSet.BRad;
    else
      ut_print_exprbug (attribute);
  }

  return;
}
