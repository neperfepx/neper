/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_data_fscanf_.h"

void
neut_data_fscanf_ori_label (double **g, int qty, double ***pColData, char **pColDataType)
{
  int i;

  for (i = 1; i <= qty; i++)
    ol_g_q (g, (*pColData)[i]);

  ut_string_string ("ori", pColDataType);

  return;
}

void
neut_data_fscanf_ori_file (struct SIM Sim, char *datavalue, int qty, double ***pColData)
{
  int i, qty0, varqty;
  char *fct = NULL, *des = NULL, **vars = NULL, **vals = NULL, *filename = NULL;

  ut_string_function (datavalue, &fct, &vars, &vals, &varqty);

  ut_string_string (NEUT_DEFAULT_ORIDES, &des);
  for (i = 0; i < varqty; i++)
    if (!ut_string_strcmp (vars[i], "des"))
      ut_string_string (vals[i], &des);

  qty0 = ol_des_size (des);
  if (qty0 == -1)
    ut_print_message (2, 2, "Failed to process expression `%s'.\n", des);

  double *tmpd = ut_alloc_1d (4);
  int *tmpi = ut_alloc_1d_int (6);
  double **tmpdd = ut_alloc_2d (3, 3);
  FILE *file = NULL;

  if (!neut_sim_isvoid (Sim))
  {
    struct SIMRES SimRes;
    neut_simres_set_zero (&SimRes);
    neut_sim_simres (Sim, "element", datavalue, &SimRes);
    ut_string_string (SimRes.file, &filename);
    neut_simres_free (&SimRes);
  }
  else
    ut_string_string (datavalue, &filename);

  file = ut_file_open (filename, "r");

  ut_file_nbwords_testwmessage (filename, qty * qty0);

  for (i = 1; i <= qty; i++)
    if (!strcmp (des, "euler-bunge"))
    {
      ol_e_fscanf (file, tmpd);
      ol_e_q (tmpd, (*pColData)[i]);
    }
    else if (!strcmp (des, "euler-kocks"))
    {
      ol_e_fscanf (file, tmpd);
      ol_ek_e (tmpd, tmpd);
      ol_e_q (tmpd, (*pColData)[i]);
    }
    else if (!strcmp (des, "euler-roe"))
    {
      ol_e_fscanf (file, tmpd);
      ol_er_e (tmpd, tmpd);
      ol_e_q (tmpd, (*pColData)[i]);
    }
    else if (!strcmp (des, "quaternion"))
      ol_q_fscanf (file, (*pColData)[i]);
    else if (!strcmp (des, "rodrigues"))
    {
      ol_R_fscanf (file, tmpd);
      ol_R_q (tmpd, (*pColData)[i]);
    }
    else if (!strcmp (des, "miller"))
    {
      ol_m_fscanf (file, tmpi);
      ol_m_q (tmpi, (*pColData)[i]);
    }
    else if (!strcmp (des, "rotmat"))
    {
      ol_g_fscanf (file, tmpdd);
      ol_g_q (tmpdd, (*pColData)[i]);
    }
    else
      ut_print_neperbug ();

  ut_file_close (file, filename, "r");

  ut_free_1d (&tmpd);
  ut_free_1d_int (&tmpi);
  ut_free_2d (&tmpdd, 3);
  ut_free_1d_char (&des);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&fct);

  return;
}

void
neut_data_fscanf_col_col (int entityqty, char *value,
                         struct DATA *pData)
{
  (*pData).ColDataSize = 3;

  (*pData).ColData = ut_alloc_2d (entityqty + 1, (*pData).ColDataSize);

  ut_array_2d_fnscanf_wcard (value, (*pData).ColData + 1, entityqty,
                             3, "color,size", "r");

  return;
}

void
neut_data_fscanf_scal (char *input, struct SIM *pSim,
                       struct TESS *pTess, struct TESR *pTesr,
                       struct NODES *pNodes, struct MESH **pMesh,
                       struct POINT *pPoints, char *entity, int entityqty,
                       char *value, char **pDataName, int *pDataSize,
                       double ***pData, char **pDataType)
{
  int i;
  struct SIMRES SimRes;
  double *data = ut_alloc_1d (entityqty + 1);

  if (pDataName)
    ut_string_string (value, pDataName);

  if (pDataSize)
    *pDataSize = 1;

  *pData = ut_alloc_2d (entityqty + 1, 1);

  if (ut_file_exist (value))
  {
    ut_array_1d_fnscanf_wcard (value, data + 1, entityqty, NULL, "r");
    for (int j = 1; j <= entityqty; j++)
      (*pData)[j][0] = data[j];
  }

  else if (ut_string_isint (value))
  {
    for (int j = 1; j <= entityqty; j++)
      (*pData)[j][0] = strtol (value, NULL, 10);
    ut_string_string ("int", pDataType);
  }

  else if (ut_string_isreal (value))
  {
    for (int j = 1; j <= entityqty; j++)
      (*pData)[j][0] = strtod (value, NULL);
    ut_string_string ("real", pDataType);
  }

  else
  {
    neut_simres_set_zero (&SimRes);

    int varqty;
    char **vars = NULL;
    ut_math_vars (value, &vars, &varqty);
    double **vals = ut_alloc_2d (varqty, entityqty + 1);
    char **vartype = ut_alloc_1d_pchar (varqty);

    double **tmp = ut_alloc_2d (entityqty + 1, 1);
    for (i = 0; i < varqty; i++)
    {
      if (pSim)
        neut_sim_simres (*pSim, entity, vars[i], &SimRes);

      if (pSim && ut_file_exist (SimRes.file))
      {
        ut_array_2d_fnscanf_wcard (SimRes.file, tmp + 1, entityqty, 1, NULL, "r");
        for (int j = 1; j <= entityqty; j++)
          vals[i][j] = tmp[j][0];

        if (ut_array_2d_isint (tmp + 1, entityqty, 1))
          ut_string_string ("int", vartype + i);
        else
          ut_string_string ("real", vartype + i);
      }

      else if (!strcmp (input, "tess"))
        for (int j = 1; j <= entityqty; j++)
          neut_tess_var_val_one (pTess, NULL, NULL, NULL, entity, j, vars[i], vals[i] + j, vartype + i);

      else if (!strcmp (input, "tesr"))
        for (int j = 1; j <= entityqty; j++)
          neut_tesr_var_val_one (*pTesr, entity, j, vars[i], vals[i] + j, vartype + i);

      else if (!strncmp (input, "node", 4) || !strcmp (input, "mesh"))
        for (int j = 1; j <= entityqty; j++)
          neut_mesh_var_val_one (*pNodes, *pMesh, pTess, NULL, NULL, NULL, NULL, 0, entity, j, vars[i], vals[i] + j, vartype + i);

      else if (!strncmp (input, "point", 5))
        for (int j = 1; j <= entityqty; j++)
          neut_point_var_val_one (*pPoints, *pTess, *pNodes, (*pMesh)[3], j, vars[i], vals[i] + j, vartype + i);

      else
        ut_print_exprbug (input);
    }
    ut_free_2d (&tmp, entityqty);

    if (!strcmp (*pDataType, "scal"))
    {
      int all_int = 1;
      for (i = 0; i < varqty; i++)
        if (strcmp (vartype[i], "%d"))
        {
          all_int = 0;
          break;
        }

      if (all_int)
        ut_string_string ("int", pDataType);
      else
        ut_string_string ("real", pDataType);
    }

    ut_math_evals (value, varqty, vars, vals, entityqty + 1, data);

    if (!strcmp (*pDataType, "int"))
    {
      if (varqty > 1 || (varqty == 1 && strcmp (value, vars[0])))
        if (!ut_array_1d_isint (data + 1, entityqty))
          ut_string_string ("real", pDataType);
    }

    for (i = 1; i <= entityqty; i++)
      (*pData)[i][0] = data[i];

    ut_free_2d (&vals, varqty);
    neut_simres_free (&SimRes);
    ut_free_2d_char (&vartype, varqty);
  }

  ut_free_1d (&data);

  return;
}

void
neut_data_fscanf_string (char *input, struct SIM *pSim,
                         struct TESS *pTess, struct TESR *pTesr,
                         struct NODES *pNodes, struct MESH **pMesh,
                         struct POINT *pPoints, char *entity, int entityqty,
                         char *value, char **pDataName, int *pDataSize,
                         char ***pData, char **pDataType)
{
  int i;
  struct SIMRES SimRes;
  (void) input;
  (void) pSim;
  (void) pTess;
  (void) pTesr;
  (void) pNodes;
  (void) pMesh;
  (void) pPoints;

  ut_string_string ("string", pDataType);

  neut_simres_set_zero (&SimRes);

  if (pDataName)
    ut_string_string (value, pDataName);

  if (pDataSize)
    *pDataSize = 1;

  *pData = ut_alloc_1d_pchar (entityqty + 1);

  if (pSim)
    neut_sim_simres (*pSim, entity, value, &SimRes);

  if (ut_file_exist (SimRes.file))
  {
    char *tmp = ut_alloc_1d_char (1000);
    FILE *file = ut_file_open (SimRes.file, "R");

    for (i = 1; i <= entityqty; i++)
    {
      if (fscanf (file, "%s", tmp) != 1)
        abort ();
      ut_string_string (tmp, (*pData) + i);
    }

    ut_file_close (file, SimRes.file, "R");
    ut_free_1d_char (&tmp);
  }

  else
  {
    for (i = 1; i <= entityqty; i++)
      ut_string_string (value, (*pData) + i);
  }

  neut_simres_free (&SimRes);

  return;
}

void
neut_data_fscanf_tensor (struct SIM Sim,
                         char *entity, int entityqty,
                         char *value, char **pDataName, int *pDataSize,
                         double ***pData, char **pDataType)
{
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  ut_string_string (value, pDataName);

  neut_sim_simres (Sim, entity, value, &SimRes);

  (*pDataSize) = ut_file_nbcolumns (SimRes.file);

  (*pData) = ut_alloc_2d (entityqty + 1, *pDataSize);

  if (SimRes.file)
    ut_array_2d_fnscanf_wcard (SimRes.file, *pData + 1,
                               entityqty, *pDataSize, NULL, "r");
  else
    abort ();

  ut_string_string ("vector", pDataType);

  neut_simres_free (&SimRes);

  return;
}

void
neut_data_fscanf_rad_scal (char *input, struct SIM Sim,
                          struct TESS *pTess, struct TESR *pTesr,
                          struct NODES *pNodes, struct MESH **pMesh,
                          struct POINT *pPoints, char *entity, int entityqty,
                          char *value, struct DATA *pData)
{
  int i;
  char *vartype = NULL;
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  ut_string_string (value, &(*pData).RadDataName);

  (*pData).RadData = ut_alloc_2d (entityqty + 1, 1);

  neut_sim_simres (Sim, entity, value, &SimRes);

  if (ut_file_exist (SimRes.file))
    ut_array_2d_fnscanf_wcard (SimRes.file, (*pData).RadData + 1,
                               entityqty, 1, NULL, "r");
  else
  {
    double *data = ut_alloc_1d (entityqty + 1);

    if (!strcmp (input, "tess"))
      neut_tess_entity_expr_val (pTess, entity, value, data, &vartype);

    else if (!strcmp (input, "tesr"))
      neut_tesr_entity_expr_val (*pTesr, entity, value, data, &vartype);

    else if (!strcmp (input, "mesh") || !strncmp (input, "node", 4))
      neut_mesh_entity_expr_val (*pNodes, *pMesh,
                                 pTess, NULL, NULL, NULL, NULL, entity, value,
                                 data, &vartype);

    else if (!strncmp (input, "point", 5))
      neut_point_entity_expr_val (*pPoints, *pTess, *pNodes,
                                  (*pMesh)[(*pTess).Dim], value, data,
                                  &vartype);

    else
      abort ();

    for (i = 1; i <= entityqty; i++)
      (*pData).RadData[i][0] = data[i];
    ut_free_1d (&data);
  }

  if (!strcmp ((*pData).RadDataType, "scal"))
  {
    if (!strcmp (vartype, "%d"))
      ut_string_string ("int", &(*pData).RadDataType);
    else if (!strcmp (vartype, "%f"))
      ut_string_string ("real", &(*pData).RadDataType);
    else
      abort ();
  }

  ut_free_1d_char (&vartype);
  neut_simres_free (&SimRes);

  return;
}
