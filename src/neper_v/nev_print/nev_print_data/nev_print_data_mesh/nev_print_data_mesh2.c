/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_data_mesh_.h"

void
nev_print_data_mesh_init (struct MESH *Mesh, struct DATA **MeshData,
                    char **pentity, char *attribute, char *datastring,
                    int *pdim, int *pentityqty, char **ptype,
                    char **pvalue, struct DATA **ppData)
{
  ut_print_message (0, 1, "Reading data (%s, %s)...\n", *pentity, attribute);

  (*pdim) = ((*pentity)[strlen (*pentity) - 1] == 'd') ?
            (*pentity)[strlen (*pentity) - 2] - '0' : neut_mesh_array_dim (Mesh);

  if (*pdim > 5)
    abort ();

  if (strstr (*pentity, "edge"))
  {
    (*pdim) = (*pentity)[strlen (*pentity) - 6] - '0'; // different here
    *pentityqty = 1;
    (*ppData) = MeshData[*pdim];
  }
  else if (!strncmp (*pentity, "elt", 3))
    (*ppData) = MeshData[*pdim];
  else if (!strncmp (*pentity, "elset", 5))
  {
    ut_string_fnrs (*pentity, "elset", "elt", 1);
    (*ppData) = MeshData[*pdim] + 1;
  }
  else if (!strncmp (*pentity, "mesh", 4))
  {
    ut_string_fnrs (*pentity, "mesh", "elt", 1);
    (*ppData) = MeshData[*pdim] + 2;
  }
  else
    abort ();

  *pentityqty = (**ppData).Qty;

  if (*pdim < 0 || *pentityqty < 0)
    abort ();

  neut_data_datastring_type_value (*pentity, attribute, datastring, ptype, pvalue);

  return;
}

void
nev_print_data_mesh_elt (struct SIM Sim, struct TESS *pTess, struct NODES *pNodes,
                   struct MESH **pMesh, char *entity, int dim, int entityqty,
                   char *attribute, char *type, char *datastring, char *datavalue,
                   struct DATA *pData)

{
  struct DATAINPUT DataInput;
  neut_datainput_set_default (&DataInput);
  ut_string_string ("mesh", &DataInput.input);
  DataInput.pSim = &Sim;
  DataInput.pTess = pTess;
  DataInput.pNodes = pNodes;
  DataInput.pMesh = pMesh;

  if (!strcmp (attribute, ""))
    ut_string_string (datastring, &(*pData).Value);

  else if (!strcmp (attribute, "colscheme"))
    ut_string_string (datastring, &(*pData).ColScheme);

  else if (!strcmp (attribute, "scale"))
    ut_string_string (datastring, &(*pData).Scale);

  else if (!strcmp (attribute, "scaletitle"))
    ut_string_string (datastring, &(*pData).ScaleTitle);

  else
    neut_data_fscanf_general (DataInput, entity, dim, entityqty, attribute, type,
        datavalue, pData);

  return;
}

void
nev_print_data_mesh_eltedge (char *attribute, char *value, struct DATA *pData)
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
nev_print_data_mesh_elset2elt (struct MESH Mesh, char *entity, char *attribute,
                        struct DATA MeshDataSet, struct DATA *pData)
{
  int i, j, size;
  int elsetqty, **elsets = NULL;

  if (!strncmp (entity, "elset", 5))
  {
    elsetqty = Mesh.ElsetQty;
    elsets = Mesh.Elsets;
  }
  else if (!strncmp (entity, "mesh", 4))
    neut_mesh_aselsets (Mesh, &elsets, &elsetqty);
  else
    abort ();

  if (entity[strlen (entity) - 1] != 'b')
  {
    if (!strcmp (attribute, "col"))
    {
      ut_string_string (MeshDataSet.ColDataType, &(*pData).ColDataType);

      neut_data_type_size ((*pData).ColDataType, &size);

      (*pData).ColData = ut_alloc_2d ((*pData).Qty + 1, size);
      for (i = 1; i <= elsetqty; i++)
        for (j = 1; j <= elsets[i][0]; j++)
          ut_array_1d_memcpy (MeshDataSet.ColData[i], size,
                              (*pData).ColData[elsets[i][j]]);
    }
    else if (!strcmp (attribute, "rad"))
    {
      ut_string_string (MeshDataSet.RadDataType, &(*pData).RadDataType);

      (*pData).RadData = ut_alloc_2d ((*pData).Qty + 1, 1);
      for (i = 1; i <= elsetqty; i++)
        for (j = 1; j <= elsets[i][0]; j++)
          (*pData).RadData[elsets[i][j]][0] = MeshDataSet.RadData[i][0];
    }
    else if (!strcmp (attribute, "weight"))
    {
      ut_string_string (MeshDataSet.WeightDataType, &(*pData).WeightDataType);

      (*pData).WeightData = ut_alloc_2d ((*pData).Qty + 1, 1);
      for (i = 1; i <= elsetqty; i++)
        for (j = 1; j <= elsets[i][0]; j++)
          (*pData).WeightData[elsets[i][j]][0] = MeshDataSet.WeightData[i][0];
    }
    else if (!strcmp (attribute, "colscheme"))
      ut_string_string (MeshDataSet.ColScheme, &(*pData).ColScheme);
    else if (!strcmp (attribute, "scale"))
      ut_string_string (MeshDataSet.Scale, &(*pData).Scale);
    else if (!strcmp (attribute, "scaletitle"))
      ut_string_string (MeshDataSet.ScaleTitle, &(*pData).ScaleTitle);
    else if (!strcmp (attribute, "symbol"))
    {}
    else if (strlen (attribute) == 0)
    {}
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

  if (elsets != Mesh.Elsets)
    ut_free_2d_int (&elsets, elsetqty);

  return;
}
