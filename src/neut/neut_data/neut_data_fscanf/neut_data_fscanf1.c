/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_data_fscanf_.h"

void
neut_data_fscanf_ori (struct SIM Sim, char *entity, char *datavalue, int qty,
                     double **dataembed, double ***pColData, char **pColDataType,
                     int *pColDataSize)
{
  double **g = ol_g_alloc ();
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  *pColDataSize = 4;
  (*pColData) = ut_alloc_2d (qty + 1, *pColDataSize);

  if (!strcmp (datavalue, "internal") && dataembed && (!Sim.simdir || Sim.step == 0))
    ut_array_2d_memcpy (dataembed + 1, qty, 4, *pColData + 1);
  else if (!strcmp (datavalue, "internal"))
  {
    neut_sim_simres (Sim, entity, "ori", &SimRes);

    if (neut_ori_fnscanf (SimRes.file, Sim.OriDes, "ascii", *pColData + 1, NULL, qty, NULL, "r") != 1)
      ut_print_message (2, 3, "Failed to read file.\n");
  }
  else if (!strcmp (datavalue, "internal"))
    ut_print_message (2, 3, "No orientation data available.\n");
  else if (!ol_label_g (datavalue, g))
    neut_data_fscanf_ori_label (g, qty, pColData, pColDataType);
  else if (!datavalue)
    ut_print_message (2, 3, "No orientation data available.\n");

  else
    neut_data_fscanf_ori_file (Sim, datavalue, qty, pColData);

  ol_g_free (g);
  neut_simres_free (&SimRes);

  return;
}

void
neut_data_fscanf_ori_tesr (struct SIM Sim, struct TESR Tesr,
                          char *datavalue, int qty,
                          double ****dataembedvox, double **dataembedcell,
                          double ***pColData)
{
  int i, j, k, id;

  (*pColData) = ut_alloc_2d (qty + 1, 4);

  if (!datavalue)
  {
    if (dataembedvox)
    {
      id = 0;
      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
            ut_array_1d_memcpy (dataembedvox[i][j][k], 4, (*pColData)[++id]);
    }
    else if (dataembedcell)
    {
      id = 0;
      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
            ut_array_1d_memcpy (dataembedcell[Tesr.VoxCell[i][j][k]], 4,
                                (*pColData)[++id]);
    }
    else
      ut_print_message (2, 3, "No orientation data available.\n");
  }

  else
    neut_data_fscanf_ori_file (Sim, datavalue, qty, pColData);

  return;
}

void
neut_data_fscanf_general (struct DATAINPUT DataInput, char *entity, int dim,
                          int entityqty, char *attribute, char *type,
                          char *value, struct DATA *pData)
{
  struct SIM *pSim = DataInput.pSim;
  struct TESS *pTess = DataInput.pTess;
  struct TESR *pTesr = DataInput.pTesr;
  struct NODES *pNodes = DataInput.pNodes;
  struct MESH **pMesh = DataInput.pMesh;
  struct POINT *pPoints = DataInput.pPoints;
  char *input = DataInput.input;
  char **pDataName = NULL, *DataName = NULL;
  char **pDataType = NULL;
  int DataSize, *pDataSize = NULL;
  double ***pDataArray = NULL;

  if (!strcmp (attribute, ""))
  {
    pDataSize = &(*pData).DataSize;
    pDataArray = &(*pData).Data;
    pDataType = &(*pData).DataType;
    pDataName = &(*pData).DataName;
  }
  else if (!strcmp (attribute, "col"))
  {
    pDataSize = &(*pData).ColDataSize;
    pDataArray = &(*pData).ColData;
    pDataType = &(*pData).ColDataType;
    pDataName = &(*pData).ColDataName;
  }
  else if (!strcmp (attribute, "rad"))
  {
    pDataSize = &DataSize;
    pDataArray = &(*pData).RadData;
    pDataType = &(*pData).RadDataType;
    pDataName = &(*pData).RadDataName;
  }
  else if (!strcmp (attribute, "symbol"))
  {
    pDataSize = &DataSize;
    pDataType = &(*pData).SymbolDataType;
    pDataName = &(*pData).SymbolDataName;
  }
  else if (!strcmp (attribute, "trs"))
  {
    pDataSize = &DataSize;
    pDataArray = &(*pData).TrsData;
    pDataType = &(*pData).TrsDataType;
    pDataName = &DataName;
  }
  else if (!strcmp (attribute, "coo"))
  {
    pDataSize = &DataSize;
    pDataArray = &(*pData).CooData;
    pDataType = &(*pData).CooDataType;
    pDataName = &(*pData).CooDataName;
  }
  else if (!strcmp (attribute, "length"))
  {
    pDataSize = &DataSize;
    pDataArray = &(*pData).LengthData;
    pDataType = &(*pData).LengthDataType;
    pDataName = &DataName;
  }
  else if (!strcmp (attribute, "weight"))
  {
    pDataSize = &DataSize;
    pDataArray = &(*pData).WeightData;
    pDataType = &(*pData).WeightDataType;
    pDataName = &DataName;
  }

  ut_string_string (entity, &(*pData).Entity);

  if (!strcmp (value, "from_nodes"))
  {
    ut_string_string (value, pDataType);
    return;
  }

  (*pData).Dim = dim;

  if (strcmp (type, "none"))
    ut_string_string (type, pDataType);
  else
    if (neut_data_value_type (*pSim, entity, attribute, value, pDataType))
      ut_print_message (2, 2, "Failed to determine data type for `%s'.\n", value);

  if (!*pDataType || !strcmp (*pDataType, "col"))
    neut_data_fscanf_col_col (entityqty, value, pData);

  else if (!strcmp (attribute, "symbol"))
    neut_data_fscanf_string (input, pSim, pTess, pTesr, pNodes, pMesh, pPoints,
                             entity, entityqty, value,
                             pDataName, pDataSize,
                             &(*pData).SymbolData, pDataType);

  else if (!strcmp (*pDataType, "int")
       || !strcmp (*pDataType, "real")
       || !strcmp (*pDataType, "scal"))
    neut_data_fscanf_scal (input, pSim, pTess, pTesr, pNodes, pMesh, pPoints,
                           entity, entityqty, value,
                           pDataName, pDataSize,
                           pDataArray, pDataType);

  else if (!strcmp (*pDataType, "vector")
        || !strcmp (*pDataType, "tensor"))
    neut_data_fscanf_tensor (*pSim, entity, entityqty, value, pDataName,
                             pDataSize, pDataArray, pDataType);

  else if (!strcmp (*pDataType, "ori"))
  {
    if (!strcmp (input, "tess"))
      neut_data_fscanf_ori (*pSim, entity, value, (*pTess).CellQty, (*pTess).CellOri,
                           pDataArray,
                           pDataType, pDataSize);

    else if (!strcmp (input, "tesr"))
    {
      if (!strcmp (entity, "cell") || !strcmp (entity, "crystal"))
        neut_data_fscanf_ori (*pSim, entity, value, (*pTesr).CellQty, (*pTesr).CellOri,
                              pDataArray,
                              pDataType, pDataSize);

      else if (!strcmp (entity, "vox"))
      {
        int id, i, j, k;
        double **ori = ut_alloc_1d_pdouble (neut_tesr_totvoxqty (*pTesr) + 1);

        if (!(*pTesr).VoxOri)
          neut_tesr_init_voxori (pTesr);

        id = 0;
        for (k = 1; k <= (*pTesr).size[2]; k++)
          for (j = 1; j <= (*pTesr).size[1]; j++)
            for (i = 1; i <= (*pTesr).size[0]; i++)
              ori[++id] = (*pTesr).VoxOri[i][j][k];

        neut_data_fscanf_ori (*pSim, entity, value, neut_tesr_totvoxqty (*pTesr), ori,
                              pDataArray, pDataType,
                              pDataSize);

        ut_free_1d_pdouble (&ori);
      }
    }
    else if (!strcmp (input, "mesh"))
    {
      if (!strncmp (entity, "elt", 3) && !(*pMesh)[dim].EltOri)
        neut_mesh_init_eltori ((*pMesh) + dim);

      neut_data_fscanf_ori (*pSim, entity, value, entityqty,
                           !strncmp (entity, "elset", 5)?
                           (*pMesh)[dim].ElsetOri : (*pMesh)[dim].EltOri,
                           pDataArray, pDataType,
                           pDataSize);
    }
  }

  else if (!strcmp (*pDataType, "disori"))
  {
    if (!strcmp (input, "tesr"))
    {
      if (!strcmp (entity, "vox"))
      {
        int cell, id, i, j, k;
        double **ori = ut_alloc_2d (neut_tesr_totvoxqty (*pTesr) + 1, 4);

        if (!(*pTesr).VoxOri)
          neut_tesr_init_voxori (pTesr);

        id = 0;
        for (k = 1; k <= (*pTesr).size[2]; k++)
          for (j = 1; j <= (*pTesr).size[1]; j++)
            for (i = 1; i <= (*pTesr).size[0]; i++)
            {
              cell = (*pTesr).VoxCell[i][j][k];
              if (cell > 0)
              {
                double *q = ol_q_alloc ();
                int status = neut_tesr_cell_ori (*pTesr, cell, q);

                if (!status)
                  ol_q_q_qdisori (q, (*pTesr).VoxOri[i][j][k],
                                  (*pTesr).CellCrySym, ori[++id]);
                ol_q_free (q);
              }

              else
                ++id;
            }

        neut_data_fscanf_ori (*pSim, entity, value, neut_tesr_totvoxqty (*pTesr), ori,
                              pDataArray, pDataType,
                              &(*pData).ColDataSize);

        ut_free_2d (&ori, neut_tesr_totvoxqty (*pTesr) + 1);
      }
      else
        abort ();
    }
    else
      abort ();
  }

  else
  {
    printf ("*pDataType = %s\n", *pDataType);
    abort ();
  }

  ut_free_1d_char (&DataName);

  return;
}

void
neut_data_fscanf_length (int entityqty, char *type, char *value,
                        struct DATA *pData)
{
  ut_string_string (type, &(*pData).LengthDataType);

  (*pData).LengthData = ut_alloc_2d (entityqty + 1, 3);
  ut_array_2d_fnscanf_wcard (value, (*pData).LengthData + 1, entityqty,
                             1, "numeral,size", "r");

  return;
}

void
neut_data_fscanf_coo (struct SIM Sim, char *entity, int entityqty, char *type,
                     char *value, struct DATA *pData)
{
  struct SIMRES SimRes;

  ut_string_string (type, &(*pData).CooDataType);

  neut_simres_set_zero (&SimRes);

  if (strcmp (type, "none"))
    ut_string_string (type, &(*pData).CooDataType);
  else
    if (neut_data_value_type (Sim, entity, "coo", value, &(*pData).CooDataType))
      ut_print_message (2, 2, "Failed to determine data type for `%s'.\n", value);

  if (!(*pData).CooData)
    (*pData).CooData = ut_alloc_2d (entityqty + 1, 3);

  ut_string_string (value, &(*pData).CooDataName);

  if (!strcmp ((*pData).CooDataType, "coo")
   || !strcmp ((*pData).CooDataType, "vector"))
  {
    ut_string_string ("coo", &(*pData).CooDataType);

    char *filename = ut_alloc_1d_char (1000);
    if (!strncmp (entity, "node", 4) && !strcmp (value, "coo"))
    {
      neut_sim_simres (Sim, "node", "coo", &SimRes);
      ut_string_string (SimRes.file, &filename);
    }
    else
      strcpy (filename, value);

    ut_array_2d_fnscanf_wcard (filename, (*pData).CooData + 1,
                               entityqty, 3, "size", "r");

    ut_free_1d_char (&filename);
  }

  else if (!strcmp ((*pData).CooDataType, "disp"))
    ut_array_2d_fnscanf_wcard (value, (*pData).CooData + 1,
                               entityqty, 3, "size", "r");
  else
    ut_print_message (2, 2, "Failed to process `%s'.\n", value);

  neut_simres_free (&SimRes);
}

void
neut_data_fscanf_label (char *value, struct DATA *pData)
{
  int i, argqty;
  char **args = NULL;

  ut_list_break (value, NEUT_SEP_DEP, &args, &argqty);

  if (argqty == 3)
  {
    if (!(*pData).Label)
      (*pData).Label = ut_alloc_1d_pchar (3);

    for (i = 0; i < 3; i++)
      ut_string_string (args[i], (*pData).Label + i);
  }

  ut_free_2d_char (&args, argqty);

  return;
}
