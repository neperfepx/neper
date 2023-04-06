/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_builtin_.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

int
nes_pproc_entity_builtin_nodes_disp (struct SIM *pSim, struct NODES Nodes,
                                     char *dir, char *res,
                                     struct SIMRES *pSimRes)
{
  int step;
  char *simfile = ut_alloc_1d_char (1000);
  char *filename = ut_alloc_1d_char (1000);
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  double ***coo = ut_alloc_3d (2, Nodes.NodeQty, 3);
  double **disp = ut_alloc_2d (Nodes.NodeQty, 3);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  neut_sim_setstep (pSim, 0);

  ut_array_2d_fnscanf ((*pSimRes).file, coo[0], Nodes.NodeQty, 3, "R");

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    if (!ut_file_exist ((*pSimRes).file))
      abort ();

    ut_array_2d_fnscanf ((*pSimRes).file, coo[1], Nodes.NodeQty, 3, "R");

    ut_array_2d_sub (coo[0], coo[1], Nodes.NodeQty, 3, disp);

    sprintf (filename, "%s/%s.step%d", dir, res, step);

    file = ut_file_open (filename, "W");
    ut_array_2d_fprintf (file, disp, Nodes.NodeQty, 3, REAL_PRINT_FORMAT);
    ut_file_close (file, filename, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  ut_free_3d (&coo, 2, Nodes.NodeQty);
  ut_free_2d (&disp, Nodes.NodeQty);

  neut_sim_addres (pSim, "node", res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&simfile);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&prev);

  return 0;
}

void
nes_pproc_entity_builtin_elsets_pre (struct SIM *pSim,
                                  struct MESH *Mesh,
                                  char *entity, char *res,
                                  struct SIMRES *pSimRes,
                                  int ***pelsets, int *pelsetqty)
{
  int i, pos;
  char *type = NULL;
  int dim = neut_mesh_array_dim (Mesh);

  neut_sim_entity_pos (*pSim, entity, &pos);
  neut_sim_entity_type (*pSim, entity, &type);

  neut_simres_set_zero (pSimRes);

  neut_sim_simres (*pSim, entity, res, pSimRes);

  *pelsetqty = (*pSim).EntityMemberQty[pos];
  (*pelsets) = ut_alloc_1d_pint (*pelsetqty + 1);

  if (!strcmp (entity, "elset"))
    for (i = 1; i <= *pelsetqty; i++)
    {
      (*pelsets)[i] = ut_alloc_1d_int (Mesh[dim].Elsets[i][0] + 1);
      ut_array_1d_int_memcpy (Mesh[dim].Elsets[i], Mesh[dim].Elsets[i][0] + 1, (*pelsets)[i]);
    }

  else if (!strcmp (entity, "elt"))
  {
    *pelsetqty = Mesh[dim].EltQty;
    *pelsets = ut_alloc_2d_int (*pelsetqty + 1, 2);

    for (i = 1; i <= *pelsetqty; i++)
    {
      (*pelsets)[i][0] = 1;
      (*pelsets)[i][1] = i;
    }
  }

  else if (!strcmp (entity, "mesh"))
    neut_mesh_aselsets (Mesh[dim], pelsets, pelsetqty);

  else if (neut_sim_entity_hasexpr (*pSim, entity))
    for (i = 1; i <= *pelsetqty; i++)
    {
      (*pelsets)[i] = ut_alloc_1d_int ((*pSim).EntityMembers[pos][i - 1][0] + 1);
      ut_array_1d_int_memcpy ((*pSim).EntityMembers[pos][i - 1],
                              (*pSim).EntityMembers[pos][i - 1][0] + 1, (*pelsets)[i]);
    }

  else
    abort ();

  ut_free_1d_char (&type);

  return;
}

void
nes_pproc_entity_builtin_elsets_ori (struct SIM *pSim,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  char *entity, char *res, int **elsets,
                                  int elsetqty, struct SIMRES *pSimRes)
{
  int step;
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  double **eltdata = NULL;
  double **elsetdata = ut_alloc_2d (elsetqty + 1, 4);
  struct SIMRES SimRes2;
  int dim = neut_mesh_array_dim (Mesh);

  eltdata = ut_alloc_2d (Mesh[dim].EltQty + 1, 4);

  neut_simres_set_zero (&SimRes2);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_sim_simres (*pSim, "elts", res, &SimRes2);

    neut_simres_setstep (pSimRes, step);
    neut_simres_setstep (&SimRes2, step);

    if (!SimRes2.file)
      abort ();

    file = ut_file_open ((*pSimRes).file, "W");

    if (!Mesh[dim].ElsetCrySym)
      ut_print_message (2, 5, "\nCrystal symmetry not defined.\n");

    if (Mesh[dim].SimEltOri)
      ut_free_2d (&(Mesh[dim].SimEltOri), Mesh[dim].EltQty + 1);
    Mesh[dim].SimEltOri = ut_alloc_2d (Mesh[dim].EltQty + 1, 4);

    neut_ori_fnscanf (SimRes2.file, (*pSim).OriDes, "ascii",
                      Mesh[dim].SimEltOri + 1, NULL,
                      Mesh[dim].EltQty, NULL, "R");
    neut_mesh_eltdata_elsetdata_ori (*pNodes, Mesh[dim], elsets, elsetqty,
                                     elsetdata);

    neut_ori_fprintf (file, (*pSim).OriDes, "ascii", elsetdata + 1, NULL, NULL, elsetqty, NULL);

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&prev);
  ut_free_2d (&eltdata, Mesh[dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);
  neut_simres_free (&SimRes2);

  return;
}

void
nes_pproc_entity_builtin_cells_ori (struct SIM *pSim, struct TESS Tess,
                                    struct TESR Tesr, char *entity, char *res,
                                    struct SIMRES *pSimRes)
{
  int step;
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    file = ut_file_open ((*pSimRes).file, "W");

    if (!neut_tess_isvoid (Tess) && !Tess.CellCrySym)
      ut_print_message (2, 5, "\nCrystal symmetry not defined.\n");
    else if (!neut_tesr_isvoid (Tesr) && !Tesr.CellCrySym)
      ut_print_message (2, 5, "\nCrystal symmetry not defined.\n");

    neut_tesr_oriaverage (&Tesr);
    neut_ori_fprintf (file, (*pSim).OriDes, "ascii", Tesr.CellOri + 1, NULL, NULL, Tesr.CellQty, NULL);

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);

  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_builtin_cells_gos (struct SIM *pSim, struct TESS Tess,
                                    struct TESR Tesr, char *entity, char *res,
                                    struct SIMRES *pSimRes)
{
  int i, step;
  double gos;
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    file = ut_file_open ((*pSimRes).file, "W");

    if (!neut_tess_isvoid (Tess) && !Tess.CellCrySym)
      ut_print_message (2, 5, "\nCrystal symmetry not defined.\n");
    else if (!neut_tesr_isvoid (Tesr) && !Tesr.CellCrySym)
      ut_print_message (2, 5, "\nCrystal symmetry not defined.\n");

    if (!neut_tesr_isvoid (Tesr))
      for (i = 1; i <= Tesr.CellQty; i++)
      {
        neut_tesr_cell_gos (Tesr, i, &gos);
        fprintf (file, REAL_PRINT_FORMAT "\n", gos);
      }

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_builtin_cells_pre (struct SIM *pSim, struct TESS Tess,
                                  struct TESR Tesr, char *entity, char *res,
                                  struct SIMRES *pSimRes,
                                  int ***pcells, int *pcellqty)
{
  int i, pos;
  char *type = NULL;

  neut_sim_entity_pos (*pSim, entity, &pos);
  neut_sim_entity_type (*pSim, entity, &type);

  neut_simres_set_zero (pSimRes);

  neut_sim_simres (*pSim, entity, res, pSimRes);

  if (!strcmp (entity, "tess"))
  {
    *pcellqty = 1;
    (*pcells) = ut_alloc_1d_pint (2);
    (*pcells)[1] = ut_alloc_1d_int (Tess.CellQty + 1);
    ut_array_1d_int_set_id ((*pcells)[1], Tess.CellQty + 1);
    (*pcells)[1][0] = Tess.CellQty;
  }

  else if (!strcmp (entity, "tesr"))
  {
    *pcellqty = 1;
    (*pcells) = ut_alloc_1d_pint (2);
    (*pcells)[1] = ut_alloc_1d_int (Tesr.CellQty + 1);
    ut_array_1d_int_set_id ((*pcells)[1], Tesr.CellQty + 1);
    (*pcells)[1][0] = Tesr.CellQty;
  }

  else if (!strcmp (entity, "cell"))
  {
    if (!neut_tess_isvoid (Tess))
    {
      *pcellqty = Tess.CellQty;
      *pcells = ut_alloc_2d_int (*pcellqty + 1, 2);

      for (i = 1; i <= *pcellqty; i++)
      {
        (*pcells)[i][0] = 1;
        (*pcells)[i][1] = i;
      }
    }
    else if (!neut_tesr_isvoid (Tesr))
    {
      *pcellqty = Tesr.CellQty;
      *pcells = ut_alloc_2d_int (*pcellqty + 1, 2);

      for (i = 1; i <= *pcellqty; i++)
      {
        (*pcells)[i][0] = 1;
        (*pcells)[i][1] = i;
      }
    }
  }

  else if (neut_sim_entity_hasexpr (*pSim, entity))
    for (i = 1; i <= *pcellqty; i++)
    {
      (*pcells)[i] = ut_alloc_1d_int ((*pSim).EntityMembers[pos][i - 1][0] + 1);
      ut_array_1d_int_memcpy ((*pSim).EntityMembers[pos][i - 1],
                              (*pSim).EntityMembers[pos][i - 1][0] + 1, (*pcells)[i]);
    }

  else
    abort ();

  ut_free_1d_char (&type);

  return;
}

void
nes_pproc_entity_builtin_cells_odf (struct SIM *pSim, struct TESS *pTess, struct TESR Tesr,
                                    char *entity, char *res, struct SIMRES *pSimRes)
{
  int i, step, varqty;
  char *prev = ut_alloc_1d_char (1000);
  char *filename = NULL;
  struct OL_SET OSet;
  struct ODF Odf;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  int CellQty;
  double **CellOri = NULL; // pointer only
  char *crysym = NULL;
  char *input = NULL;
  char *weight = NULL;
  char *cutoff = NULL;
  struct SIMRES SimRes2;

  neut_simres_set_zero (&SimRes2);

  if (!neut_tess_isvoid (*pTess))
  {
    ut_string_string ("tess", &input);
    CellQty = (*pTess).CellQty;
    CellOri = (*pTess).CellOri;
    ut_string_string ((*pTess).CellCrySym, &crysym);
  }
  else if (!neut_tesr_isvoid (Tesr))
  {
    ut_string_string ("tesr", &input);
    CellQty = Tesr.CellQty;
    CellOri = Tesr.CellOri;
    ut_string_string (Tesr.CellCrySym, &crysym);
  }
  else
    abort ();

  // Initializing operation
  // fct = odf or odfn, vars and vals = arguments
  ut_string_function (res, &fct, &vars, &vals, &varqty);

  // Initializing orientation / ODF
  neut_sim_orispace (*pSim, &Odf, "R");
  neut_odf_setsigma (&Odf, "avthetaeq", CellQty, crysym);
  ut_string_string ("1", &weight);
  ut_string_string ("all", &cutoff);
  for (i = 0; i < varqty; i++)
  {
    if (!strcmp (vars[i], "theta"))
      Odf.sigma =  M_PI / 180 * atof (vals[i]);
    else if (!strcmp (vars[i], "weight"))
      ut_string_string (vals[i], &weight);
    else if (!strcmp (vars[i], "cutoff"))
      ut_string_string (vals[i], &cutoff);
  }

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);
  printf ("\n");
  ut_print_message (0, 5, "(theta = %4.1f°) ................................     ", Odf.sigma * 180 / M_PI);

  // Allocating OSet
  OSet = ol_set_alloc (CellQty, crysym);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_sim_simres (*pSim, "cells", "ori", &SimRes2);

    neut_simres_setstep (pSimRes, step);
    neut_simres_setstep (&SimRes2, step);

    // Setting OSet

    // Setting orientations
    // general case
    if (ut_file_exist (SimRes2.file))
      neut_ori_fnscanf (SimRes2.file, (*pSim).OriDes, "ascii", OSet.q,
                        NULL, CellQty, NULL, "R");

    // only initial state and no ori in results
    else if (!step)
      ut_array_2d_memcpy (CellOri + 1, CellQty, 4, OSet.q);

    else
      abort ();

    // Setting weights
    if (!strcmp (input, "tess"))
      for (i = 1; i <= CellQty; i++)
        neut_tess_expr_val_one (pTess, NULL, NULL, NULL, "cell", i, weight,
                               OSet.weight + i - 1, NULL);
    else
      for (i = 1; i <= CellQty; i++)
        neut_tesr_expr_val_one (Tesr, "cell", i, weight, OSet.weight + i - 1,
                               NULL);

    if (!strcmp (fct, "odf"))
      neut_odf_comp ("m", cutoff, &OSet, &Odf);
    else if (!strcmp (fct, "odfn"))
      neut_odf_comp ("n", cutoff, &OSet, &Odf);

    // Writing results
    if (!strcmp (fct, "odf"))
      ut_array_1d_fnprintf_column ((*pSimRes).file, Odf.odf, Odf.odfqty, REAL_PRINT_FORMAT, "W");
    else if (!strcmp (fct, "odfn"))
      ut_array_1d_fnprintf_column ((*pSimRes).file, Odf.odfn, Odf.odfnqty, REAL_PRINT_FORMAT, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  ol_set_free (OSet);

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  // neut_odf_free (&Odf);
  ut_free_1d_char (&crysym);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&prev);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d_char (&input);
  ut_free_1d_char (&weight);
  neut_simres_free (&SimRes2);
  ut_free_1d_char (&cutoff);

  return;
}

void
nes_pproc_entity_builtin_elsets_gos (struct SIM *pSim, struct TESS Tess,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     char *entity, char *res, int **elsets,
                                     int elsetqty, struct SIMRES *pSimRes)
{
  int i, step;
  double **eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 4);
  double **elsetdata = ut_alloc_2d (elsetqty + 1, 4);
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  struct SIMRES SimRes2;
  double *gos = ut_alloc_1d (elsetqty + 1);

  neut_simres_set_zero (&SimRes2);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    neut_sim_simres (*pSim, "elts", "ori", &SimRes2);
    neut_simres_setstep (&SimRes2, step);

    if (!SimRes2.file)
      abort ();

    file = ut_file_open ((*pSimRes).file, "W");

    eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 4);
    neut_ori_fnscanf (SimRes2.file, (*pSim).OriDes, "ascii", eltdata + 1, NULL, Mesh[Tess.Dim].EltQty, NULL, "R");

    neut_mesh_eltdata_elsetdata_origos (*pNodes, Mesh[Tess.Dim], elsets,
                                        elsetqty, gos);

    for (i = 1; i <= elsetqty; i++)
      fprintf (file, REAL_PRINT_FORMAT "\n", gos[i]);

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_2d (&eltdata, Mesh[Tess.Dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);
  ut_free_1d_char (&prev);
  neut_simres_free (&SimRes2);
  ut_free_1d (&gos);

  return;
}

void
nes_pproc_entity_builtin_elsets_oridis (struct SIM *pSim, struct TESS Tess,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     char *entity, char *res, int **elsets,
                                     int elsetqty, struct SIMRES *pSimRes)
{
  int i, step;
  double **eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 4);
  double **elsetdata = ut_alloc_2d (elsetqty + 1, 4);
  double ***evect = ut_alloc_3d (Mesh[Tess.Dim].ElsetQty + 1, 3, 3);
  double **eval = ut_alloc_2d (Mesh[Tess.Dim].ElsetQty + 1, 3);
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    neut_mesh_init_simeltori (Mesh + Tess.Dim);

    neut_mesh_eltdata_elsetdata_oridis (*pNodes, Mesh[Tess.Dim], elsets, elsetqty,
                                        evect, eval);

    file = ut_file_open ((*pSimRes).file, "W");

    for (i = 1; i <= elsetqty; i++)
    {
      if (!strcmp (res, "oridisanisoangles"))
        ut_array_1d_fprintf (file, eval[i], 3, REAL_PRINT_FORMAT);
      else if (!strcmp (res, "oridisanisoaxes"))
        ut_array_2d_fprintf_oneline (file, evect[i], 3, 3, REAL_PRINT_FORMAT);
      else if (!strcmp (res, "oridisanisofact"))
        fprintf (file, REAL_PRINT_FORMAT "\n", eval[i][0] / pow (ut_array_1d_prod (eval[i], 3), 1. / 3));
    }

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_mesh_free_simeltori (Mesh + Tess.Dim);

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_3d (&evect, Mesh[Tess.Dim].ElsetQty + 1, 3);
  ut_free_2d (&eval, Mesh[Tess.Dim].ElsetQty + 1);
  ut_free_2d (&eltdata, Mesh[Tess.Dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);
  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_builtin_elsets_odf (struct SIM *pSim, struct TESS *pTess,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     char *entity, char *res, struct SIMRES *pSimRes)
{
  int i, step, size, status;
  double **elsetdata = ut_alloc_2d (Mesh[(*pTess).Dim].ElsetQty + 1, 4);
  double ***evect = ut_alloc_3d (Mesh[(*pTess).Dim].ElsetQty + 1, 3, 3);
  double **eval = ut_alloc_2d (Mesh[(*pTess).Dim].ElsetQty + 1, 3);
  char *prev = ut_alloc_1d_char (1000);
  char *filename = NULL;
  FILE *file = NULL;
  struct OL_SET OSet;
  struct ODF Odf;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  char *weight = NULL;
  char *thetastring = ut_alloc_1d_char (100);
  struct SIMRES SimRes2;

  neut_simres_set_zero (&SimRes2);

  ut_string_function (res, &fct, &vars, &vals, &size);

  neut_sim_orispace (*pSim, &Odf, "R");

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_sim_simres (*pSim, "elsets", "ori", &SimRes2);

    neut_simres_setstep (pSimRes, step);
    neut_simres_setstep (&SimRes2, step);

    status = neut_mesh_elsetori (Mesh[(*pTess).Dim], elsetdata);
    if (status)
      abort ();

    file = ut_file_open ((*pSimRes).file, "W");

    neut_mesh_elsets_olset (*pNodes, Mesh[(*pTess).Dim], elsetdata,
                            NULL, Mesh[(*pTess).Dim].ElsetQty, &OSet);
    ut_string_string (Mesh[(*pTess).Dim].ElsetCrySym, &(OSet.crysym));

    int user_sigma = 0;
    ut_string_string ("1", &weight);
    for (i = 0; i < size; i++)
    {
      if (!strcmp (vars[i], "theta"))
      {
        Odf.sigma = atof (vals[i]);
        Odf.sigma *= M_PI / 180;
        user_sigma = 1;
      }
      else if (!strcmp (vars[i], "weight"))
        ut_string_string (vals[i], &weight);
    }
    if (!user_sigma)
      neut_odf_setsigma (&Odf, "avthetaeq", OSet.size, OSet.crysym);

    sprintf (thetastring, " (theta = %9.6f°)     ", Odf.sigma * 180 / M_PI);
    ut_print_clearline (stdout, strlen (thetastring) - 1);
    printf ("%s", thetastring);

    ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

    // Setting weights
    for (i = 1; i <= size; i++)
      neut_mesh_entity_expr_val (*pNodes, Mesh, pTess,
                                 NULL, NULL, NULL, NULL, "elset", weight,
                                 OSet.weight + i - 1, NULL);

    if (!strcmp (fct, "odf"))
    {
      neut_odf_comp ("m", !strcmp (weight, "1") ? "5" : "all", &OSet, &Odf);
      for (i = 0; i < Odf.odfqty; i++)
        fprintf (file, REAL_PRINT_FORMAT "\n", Odf.odf[i]);
    }
    else if (!strcmp (fct, "odfn"))
    {
      neut_odf_comp ("n", !strcmp (weight, "1") ? "5" : "all", &OSet, &Odf);
      for (i = 0; i < Odf.odfnqty; i++)
        fprintf (file, REAL_PRINT_FORMAT "\n", Odf.odfn[i]);
    }

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  // neut_odf_free (&Odf);
  ut_free_1d_char (&filename);
  ut_free_3d (&evect, Mesh[(*pTess).Dim].ElsetQty + 1, 3);
  ut_free_2d (&eval, Mesh[(*pTess).Dim].ElsetQty + 1);
  ut_free_2d (&elsetdata, Mesh[(*pTess).Dim].ElsetQty + 1);
  ut_free_1d_char (&prev);
  ut_free_1d_char (&thetastring);
  ut_free_2d_char (&vars, size);
  ut_free_2d_char (&vals, size);
  neut_simres_free (&SimRes2);

  return;
}

void
nes_pproc_entity_builtin_elsets_readodf (struct SIM *pSim, struct TESS Tess,
                                         struct MESH *Mesh,
                                         char *entity, char *res, struct SIMRES *pSimRes)
{
  struct ODF Odf;
  char *orides = NULL;
  char *prev = ut_alloc_1d_char (1000);
  int i, step;
  FILE *file = NULL;
  int entityqty;
  double **ori = NULL;
  struct SIMRES SimRes2;
  double *oriodf = NULL;

  neut_simres_set_zero (&SimRes2);

  if (!strcmp (entity, "elset"))
    entityqty = Mesh[Tess.Dim].ElsetQty;
  else if (!strcmp (entity, "elt"))
    entityqty = Mesh[Tess.Dim].EltQty;
  else
    abort ();

  ori = ut_alloc_2d (entityqty + 1, 4);
  oriodf = ut_alloc_1d (entityqty + 1);

  neut_sim_orispace (*pSim, &Odf, "R");

  neut_odf_orides (Odf, &orides);
  Odf.odfqty = Odf.Mesh[3].EltQty;
  Odf.odf = ut_alloc_1d (Odf.odfqty + 1);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    neut_sim_simres (*pSim, "mesh", res, &SimRes2);

    // loading mesh ODF
    ut_array_1d_fnscanf (SimRes2.file, Odf.odf + 1, Odf.odfqty, "R");

    // loading elset orientations
    if (!strcmp (entity, "elt"))
      neut_mesh_eltori (Mesh[Tess.Dim], ori);
    else if (!strcmp (entity, "elset"))
      neut_mesh_elsetori (Mesh[Tess.Dim], ori);

#pragma omp parallel for private(i)
    for (i = 1; i <= entityqty; i++)
    {
      int elt;
      double *R = ol_R_alloc ();

      if (!strcmp (orides, "rodrigues"))
      {
        ol_q_R (ori[i], R);
        ol_R_Rcrysym (R, Mesh[Tess.Dim].ElsetCrySym, R);
      }

      neut_mesh_point_elt (Odf.Nodes, Odf.Mesh[3], R, &elt);

      if (elt == -1)
        abort ();

      oriodf[i] = Odf.odf[elt];

      ol_R_free (R);
    }

    file = ut_file_open ((*pSimRes).file, "W");
    ut_array_1d_fprintf_column (file, oriodf + 1, entityqty, REAL_PRINT_FORMAT);
    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&orides);
  ut_free_1d_char (&prev);
  ut_free_2d (&ori, entityqty + 1);
  neut_simres_free (&SimRes2);
  ut_free_1d (&oriodf);

  return;
}

void
nes_pproc_entity_builtin_cells_readodf (struct SIM *pSim, struct TESS Tess,
                                        struct TESR Tesr,
                                        char *entity, char *res, struct SIMRES *pSimRes)
{
  struct ODF Odf;
  char *orides = NULL;
  char *prev = ut_alloc_1d_char (1000);
  int i, step;
  FILE *file = NULL;
  int entityqty;
  double **ori = NULL;
  struct SIMRES SimRes2;
  double *oriodf = NULL;
  char *parent = NULL;
  char *crysym = NULL;

  neut_simres_set_zero (&SimRes2);

  if (!strcmp (entity, "cell"))
  {
    if (!neut_tess_isvoid (Tess))
    {
      entityqty = Tess.CellQty;
      ut_string_string ("tess", &parent);
      ut_string_string (Tess.CellCrySym, &crysym);
    }
    else if (!neut_tesr_isvoid (Tesr))
    {
      entityqty = Tesr.CellQty;
      ut_string_string ("tesr", &parent);
      ut_string_string (Tesr.CellCrySym, &crysym);
    }
    else
      abort ();
  }
  else
    abort ();

  ori = ut_alloc_2d (entityqty + 1, 4);
  oriodf = ut_alloc_1d (entityqty + 1);

  neut_sim_orispace (*pSim, &Odf, "R");

  neut_odf_orides (Odf, &orides);
  Odf.odfqty = Odf.Mesh[3].EltQty;
  Odf.odf = ut_alloc_1d (Odf.odfqty + 1);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    neut_sim_simres (*pSim, parent, res, &SimRes2);

    // loading mesh ODF
    ut_array_1d_fnscanf (SimRes2.file, Odf.odf + 1, Odf.odfqty, "R");

    // loading cell orientations
    if (!strcmp (parent, "tess"))
      neut_tess_cellori (Tess, ori);
    else if (!strcmp (parent, "tesr"))
      neut_tesr_cellori (Tesr, ori);

#pragma omp parallel for private(i)
    for (i = 1; i <= entityqty; i++)
    {
      int elt;
      double *R = ol_R_alloc ();

      if (!strcmp (orides, "rodrigues"))
      {
        ol_q_R (ori[i], R);
        ol_R_Rcrysym (R, crysym, R);
      }

      neut_mesh_point_elt (Odf.Nodes, Odf.Mesh[3], R, &elt);

      if (elt == -1)
      {
        ol_R_fprintf (stdout, R, "%f");
        abort ();
      }

      oriodf[i] = Odf.odf[elt];

      ol_R_free (R);
    }

    file = ut_file_open ((*pSimRes).file, "W");
    ut_array_1d_fprintf_column (file, oriodf + 1, entityqty, REAL_PRINT_FORMAT);
    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&orides);
  ut_free_1d_char (&prev);
  ut_free_2d (&ori, entityqty + 1);
  neut_simres_free (&SimRes2);
  ut_free_1d (&oriodf);
  ut_free_1d_char (&parent);
  ut_free_1d_char (&crysym);

  return;
}

void
nes_pproc_entity_builtin_elsets_gen (struct SIM *pSim,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  char *entity, char *res_in, int **elsets,
                                  int elsetqty, struct SIMRES *pSimRes)
{
  int i, step;
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  double **eltdata = NULL;
  double **elsetdata = NULL;
  struct SIMRES SimRes2;
  char *res = NULL;
  char *op = NULL;
  int dim = neut_mesh_array_dim (Mesh);

  ut_string_string (res_in, &res);
  ut_string_string ("mean", &op);
  if (strstr (res_in, "_mean"))
  {
    ut_string_fnrs (res, "_mean", "", 1);
    ut_string_string ("mean", &op);
  }
  else if (strstr (res_in, "_stddev"))
  {
    ut_string_fnrs (res, "_stddev", "", 1);
    ut_string_string ("stddev", &op);
  }
  else if (strstr (res_in, "_var"))
  {
    ut_string_fnrs (res, "_var", "", 1);
    ut_string_string ("var", &op);
  }
  else if (strstr (res_in, "_prval"))
  {
    ut_string_fnrs (res, "_prval", "", 1);
    ut_string_string ("prval", &op);
  }
  else if (strstr (res_in, "_prvect"))
  {
    ut_string_fnrs (res, "_prvect", "", 1);
    ut_string_string ("prvect", &op);
  }

  neut_simres_set_zero (&SimRes2);
  neut_sim_simres (*pSim, "elts", res, &SimRes2);

  eltdata = ut_alloc_2d (Mesh[dim].EltQty + 1, SimRes2.colqty);
  elsetdata = ut_alloc_2d (elsetqty + 1, SimRes2.colqty);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_sim_simres (*pSim, "elts", res, &SimRes2);
    neut_simres_setstep (pSimRes, step);

    if (!SimRes2.file)
      abort ();

    file = ut_file_open ((*pSimRes).file, "W");

    ut_array_2d_fnscanf (SimRes2.file, eltdata + 1, Mesh[dim].EltQty, SimRes2.colqty, "R");

    neut_mesh_eltdata_elsetdata_stat (*pNodes, Mesh[dim], elsets, elsetqty,
                                      eltdata, SimRes2.colqty, op, elsetdata);

    for (i = 1; i <= elsetqty; i++)
      ut_array_1d_fprintf (file, elsetdata[i], SimRes2.colqty, REAL_PRINT_FORMAT);

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res_in, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&prev);
  ut_free_2d (&eltdata, Mesh[dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);
  neut_simres_free (&SimRes2);
  ut_free_1d_char (&res);
  ut_free_1d_char (&op);

  return;
}
