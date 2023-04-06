/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_stat_.h"

void
net_stat_tess (FILE * file, char *entity, char *format, struct TESS *pTess)
{
  int i, j, status, qty, invalqty, valqty;
  double *vals = NULL;
  char **invar = NULL, *type = NULL;
  double **data = NULL;
  int *dataqty = NULL;
  char **datatype = NULL;
  double *coo = ut_alloc_1d (3);

  neut_tess_entity_qty (*pTess, entity, &qty);

  ut_list_break (format, NEUT_SEP_NODEP, &invar, &invalqty);

  data = ut_alloc_2d (invalqty, qty + 1);
  dataqty = ut_alloc_1d_int (invalqty);
  datatype = ut_alloc_1d_pchar (invalqty);

  for (i = 1; i <= qty; i++)
    for (j = 0; j < invalqty; j++)
    {
      status =
        neut_tess_var_val (pTess, NULL, NULL, NULL, entity, i, invar[j], &vals,
                           &valqty, &type);

      if (!status)
        ut_array_1d_fprintf_nonl (file, vals, valqty,
                                  !strcmp (type, "%f") ? REAL_PRINT_FORMAT : type);
      else
        ut_print_exprbug (invar[j]);

      fprintf (file, (j < invalqty - 1) ? " " : "\n");
    }

  ut_free_2d_char (&invar, invalqty);
  ut_free_1d_char (&type);
  ut_free_2d (&data, invalqty);
  ut_free_1d_int (&dataqty);
  ut_free_2d_char (&datatype, invalqty);
  ut_free_1d (&coo);
  ut_free_1d (&vals);

  return;
}

void
net_stat_point (FILE * file, char *format, struct POINT Point,
                struct TESS Tess)
{
  int i, j, status, invalqty, valqty;
  double *vals = NULL;
  char **invar = NULL, *type = NULL;
  struct NODES Nodes;
  struct MESH Mesh;

  neut_nodes_set_zero (&Nodes);
  neut_mesh_set_zero (&Mesh);

  ut_list_break (format, NEUT_SEP_NODEP, &invar, &invalqty);

  for (i = 1; i <= Point.Qty; i++)
    for (j = 0; j < invalqty; j++)
    {
      status =
        neut_point_var_val (Point, i, Tess, Nodes, Mesh, invar[j], &vals,
                            &valqty, &type);

      if (!status)
        ut_array_1d_fprintf_nonl (file, vals, valqty,
                                  !strcmp (type, "%f") ? REAL_PRINT_FORMAT : type);
      else
        ut_print_exprbug (invar[j]);

      fprintf (file, (j < invalqty - 1) ? " " : "\n");
    }

  ut_free_2d_char (&invar, invalqty);
  ut_free_1d_char (&type);
  ut_free_1d (&vals);

  neut_nodes_free (&Nodes);
  neut_mesh_free (&Mesh);

  return;
}

void
net_stat_tesr (FILE * file, char *entity, char *format, struct TESR *pTesr)
{
  int i, j, qty, status, invalqty, valqty;
  double *vals = NULL;
  char **invar = NULL, *type = NULL;
  double **data = NULL;
  int *dataqty = NULL;
  char **datatype = NULL;
  double *coo = ut_alloc_1d (3);

  neut_tesr_entity_qty (*pTesr, entity, &qty);

  ut_list_break (format, NEUT_SEP_NODEP, &invar, &invalqty);

  data = ut_alloc_2d (invalqty, qty + 1);
  dataqty = ut_alloc_1d_int (invalqty);
  datatype = ut_alloc_1d_pchar (invalqty);

  for (i = 1; i <= qty; i++)
    for (j = 0; j < invalqty; j++)
    {
      status =
        neut_tesr_expr_val (*pTesr, entity, i, invar[j], &vals, &valqty,
                            &type);

      if (!status)
        ut_array_1d_fprintf_nonl (file, vals, valqty,
                                  !strcmp (type, "%f") ? REAL_PRINT_FORMAT : type);
      else
        ut_print_exprbug (invar[j]);

      fprintf (file, (j < invalqty - 1) ? " " : "\n");
    }

  ut_free_2d_char (&invar, invalqty);
  ut_free_1d_char (&type);
  ut_free_1d (&vals);
  ut_free_2d (&data, invalqty);
  ut_free_1d_int (&dataqty);
  ut_free_2d_char (&datatype, invalqty);
  ut_free_1d (&coo);

  return;
}
