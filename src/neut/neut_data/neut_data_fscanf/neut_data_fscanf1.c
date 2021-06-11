/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_data_fscanf_.h"

void
neut_data_fscanf_ori (struct SIM Sim, char *datavalue, int qty,
                     double **dataembed, double ***pColData, char **pColDataType,
                     int *pColDataSize)
{
  double **g = ol_g_alloc ();

  *pColDataSize = 4;
  (*pColData) = ut_alloc_2d (qty + 1, *pColDataSize);

  if (!strcmp (datavalue, "internal") && !Sim.simdir && dataembed)
    ut_array_2d_memcpy (dataembed + 1, qty, 4, *pColData + 1);
  else if ((!strcmp (datavalue, "internal") || !strcmp (datavalue, "from_sim")) && Sim.simdir)
  {
    char *filename = ut_alloc_1d_char (1000);

    neut_sim_res_file (Sim, "element", "ori", filename);
    if (neut_ori_fnscanf (filename, Sim.OriDes, "ascii", *pColData + 1, NULL, qty, NULL, "r") != 1)
      ut_print_message (2, 3, "Failed to read file.\n");

    ut_free_1d_char (&filename);
  }
  else if (!ol_label_g (datavalue, g))
    neut_data_fscanf_ori_label (g, qty, pColData, pColDataType);
  else if (!datavalue)
    ut_print_message (2, 3, "No orientation data available.\n");

  else
    neut_data_fscanf_ori_file (Sim, datavalue, qty, pColData);

  ol_g_free (g);

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
neut_data_fscanf_scal (struct SIM Sim, char *datavalue, int qty, double **dataembed,
                      double ***pColData)
{
  char *value2 = ut_alloc_1d_char (1000);

  (*pColData) = ut_alloc_2d (qty + 1, 1);

  if (!datavalue && dataembed)
    ut_array_2d_memcpy (dataembed + 1, qty, 1, *pColData + 1);

  else if (!neut_sim_res_file (Sim, NULL, datavalue, value2))
    ut_array_2d_fnscanf_wcard (value2, (*pColData) + 1, qty, 1, "numeral,size", "r");

  else
    ut_print_message (2, 3, "No scalar data available.\n");

  ut_free_1d_char (&value2);

  return;
}

void
neut_data_fscanf_col (struct SIM Sim, void *pdata1, void *pdata2, void *pdata3,
                     void *pdata4, char *input, char *entity,
                     int dim, int entityqty, char *type, char *value,
                     struct DATA *pData)
{
  struct TESS *pTess = NULL;
  struct TESR *pTesr = NULL;
  struct NODES *pNodes = NULL;
  struct MESH **pMesh = NULL;
  struct POINT *pPoints = NULL;

  if (!strcmp (input, "tess"))
    pTess = (struct TESS *) pdata1;
  if (!strcmp (input, "tesr"))
    pTesr = (struct TESR *) pdata1;
  else if (!strcmp (input, "nodes") || !strcmp (input, "mesh"))
  {
    pTess = (struct TESS *) pdata1;
    pNodes = (struct NODES *) pdata2;
    pMesh = (struct MESH **) pdata3;
  }
  else if (!strncmp (input, "point", 5))
  {
    pTess = (struct TESS *) pdata1;
    pNodes = (struct NODES *) pdata2;
    pMesh = (struct MESH **) pdata3;
    pPoints = (struct POINT *) pdata4;
  }
  else if (!strcmp (input, "csys"))
  {
  }

  if (!strcmp (value, "from_nodes"))
  {
    ut_string_string (value, &(*pData).ColDataType);
    return;
  }

  (*pData).Dim = dim;

  if (strcmp (type, "none"))
    ut_string_string (type, &(*pData).ColDataType);
  else
    if (neut_data_value_type (Sim, entity, "col", value, &(*pData).ColDataType))
      ut_print_message (2, 2, "Failed to determine data type for `%s'.\n", value);

  if (!strcmp ((*pData).ColDataType, "col"))
    neut_data_fscanf_col_col (entityqty, value, pData);

  else if (!strcmp ((*pData).ColDataType, "int")
       || !strcmp ((*pData).ColDataType, "real")
       || !strcmp ((*pData).ColDataType, "expr"))
    neut_data_fscanf_col_scal (input, Sim, pTess, pTesr, pNodes, pMesh, pPoints,
                              entity, entityqty, value, pData);

  else if (!strcmp ((*pData).ColDataType, "vector")
        || !strcmp ((*pData).ColDataType, "tensor"))
    neut_data_fscanf_col_tensor (Sim, entity, entityqty, value, pData);

  else if (!strcmp ((*pData).ColDataType, "ori"))
  {
    if (!strcmp (input, "tess"))
      neut_data_fscanf_ori (Sim, value, (*pTess).CellQty, (*pTess).CellOri,
                           &(*pData).ColData,
                           &(*pData).ColDataType, &(*pData).ColDataSize);
    else if (!strcmp (input, "tesr"))
    {
      if (!strcmp (entity, "cell"))
        neut_data_fscanf_ori (Sim, value, (*pTesr).CellQty, (*pTesr).CellOri,
                              &(*pData).ColData,
                              &(*pData).ColDataType, &(*pData).ColDataSize);

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

        neut_data_fscanf_ori (Sim, value, neut_tesr_totvoxqty (*pTesr), ori,
                              &(*pData).ColData, &(*pData).ColDataType,
                              &(*pData).ColDataSize);

        ut_free_1d_pdouble (&ori);
      }
    }
    else if (!strcmp (input, "mesh"))
    {
      if (!strncmp (entity, "elt", 3) && !(*pMesh)[dim].EltOri)
        neut_mesh_init_eltori ((*pMesh) + dim);

      neut_data_fscanf_ori (Sim, value, entityqty,
                           !strncmp (entity, "elset", 5)?
                           (*pMesh)[dim].ElsetOri : (*pMesh)[dim].EltOri,
                           &(*pData).ColData, &(*pData).ColDataType,
                           &(*pData).ColDataSize);
    }
  }

  else if (!strcmp ((*pData).ColDataType, "disori"))
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
                ol_q_q_qdisori ((*pTesr).CellOri[cell],
                                (*pTesr).VoxOri[i][j][k],
                                (*pTesr).CellCrySym ? (*pTesr).CellCrySym : "triclinic", ori[++id]);

              else
                ++id;
            }

        neut_data_fscanf_ori (Sim, value, neut_tesr_totvoxqty (*pTesr), ori,
                              &(*pData).ColData, &(*pData).ColDataType,
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
    abort ();

  return;
}

void
neut_data_fscanf_trs (int entityqty, char *type, char *value,
                     struct DATA *pData)
{
  ut_string_string (type, &(*pData).TrsDataType);

  if (!strcmp (type, "none") || !strcmp (type, "real"))
  {
    (*pData).TrsData = ut_alloc_2d (entityqty + 1, 3);
    ut_array_2d_fnscanf_wcard (value, (*pData).TrsData + 1, entityqty,
                               1, "numeral,size", "r");
  }
  else
    abort ();

  return;
}

void
neut_data_fscanf_rad (int entityqty, char *type, char *value,
                          struct DATA *pData)
{
  ut_string_string (type, &(*pData).RadDataType);

  if (!strcmp ((*pData).RadDataType, "none")
   || !strcmp ((*pData).RadDataType, "rad"))
  {
    (*pData).RadData = ut_alloc_2d (entityqty + 1, 3);
    ut_array_2d_fnscanf_wcard (value, (*pData).RadData + 1, entityqty,
                               1, "numeral,size", "r");
  }
  else if (!strcmp ((*pData).RadDataType, "cube"))
  {
    (*pData).RadData = ut_alloc_2d ((*pData).Qty + 1, 10);
    ut_array_2d_fnscanf_wcard (value, (*pData).RadData + 1,
                               (*pData).Qty, 10, "numeral,size", "r");
  }
  else if (!strcmp ((*pData).RadDataType, "cyl"))
  {
    (*pData).RadData = ut_alloc_2d ((*pData).Qty + 1, 5);
    ut_array_2d_fnscanf_wcard (value, (*pData).RadData + 1,
                               (*pData).Qty, 5, "numeral,size", "r");
  }
  else if (!strcmp ((*pData).RadDataType, "arr"))
  {
    (*pData).RadData = ut_alloc_2d ((*pData).Qty + 1, 5);
    ut_array_2d_fnscanf_wcard (value, (*pData).RadData + 1,
                               (*pData).Qty, 5, "numeral,size", "r");
  }
  else if (!strcmp ((*pData).RadDataType, "tor"))
  {
    (*pData).RadData = ut_alloc_2d ((*pData).Qty + 1, 5);
    ut_array_2d_fnscanf_wcard (value, (*pData).RadData + 1,
                               (*pData).Qty, 5, "numeral,size", "r");
  }
  else if (!strcmp ((*pData).RadDataType, "disc"))
  {
    (*pData).RadData = ut_alloc_2d ((*pData).Qty + 1, 4);
    ut_array_2d_fnscanf_wcard (value, (*pData).RadData + 1,
                               (*pData).Qty, 4, "numeral,size", "r");
  }
  else if (!strcmp ((*pData).RadDataType, "ell"))
  {
    (*pData).RadData = ut_alloc_2d ((*pData).Qty + 1, 12);
    ut_array_2d_fnscanf_wcard (value, (*pData).RadData + 1,
                               (*pData).Qty, 12, "numeral,size", "r");
  }
  else
    abort ();

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
  ut_string_string (type, &(*pData).CooDataType);

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
      neut_sim_res_file (Sim, "node", "coo", filename);
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
