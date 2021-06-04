/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

void
nes_pproc_entity_pre (struct IN_S In, struct SIM Sim, char *entity,
                      int *pentityqty, char **pdir, char ***presults,
                      int *presultqty)
{
  int i;
  char *tmp = NULL;

  if (neut_sim_entityisnode (entity))
  {
    (*pentityqty) = Sim.NodeQty;
    (*pdir) = ut_string_paste (In.simdir, "/results/nodes");
    ut_string_string (In.noderes, &tmp);
    ut_list_rmelt (&tmp, NEUT_SEP_NODEP, "inputres");
    ut_list_break (tmp, NEUT_SEP_NODEP, presults, presultqty);
  }
  else if (neut_sim_entityiselt (entity))
  {
    (*pentityqty) = Sim.EltQty;
    (*pdir) = ut_string_paste (In.simdir, "/results/elements");
    ut_string_string (In.eltres, &tmp);
    ut_list_rmelt (&tmp, NEUT_SEP_NODEP, "inputres");
    ut_list_break (tmp, NEUT_SEP_NODEP, presults, presultqty);
  }
  else if (neut_sim_entityiselset (entity))
  {
    (*pentityqty) = Sim.ElsetQty;
    (*pdir) = ut_string_paste (In.simdir, "/results/elsets");
    ut_string_string (In.elsetres, &tmp);
    ut_list_rmelt (&tmp, NEUT_SEP_NODEP, "inputres");
    ut_list_break (tmp, NEUT_SEP_NODEP, presults, presultqty);
  }
  else
    abort ();

  // testing

  for (i = 0; i < *presultqty; i++)
  {
    char *result = NULL, *expr = NULL;

    ut_list_break_2 ((*presults)[i], NEUT_SEP_DEP, &result, &expr);

    // pre
    if (strstr (result, "/"))
    {
      ut_print_message (0, 2, "Testing results...\n");
      if (expr)
        ut_print_message (2, 3, "%s contains a '/'.  Invalid name.\n");
      else
        ut_print_message (2, 3, "%s contains a '/'.  It must be assigned a name.\n", result);
    }

    ut_free_1d_char (&result);
    ut_free_1d_char (&expr);
  }

  ut_free_1d_char (&tmp);

  return;
}

void
nes_pproc_entity_file (struct SIM *pSim, char *entity, char *dir, char *res, char *expr)
{
  int step;
  char *simfile = ut_alloc_1d_char (1000);
  char *filename = ut_alloc_1d_char (1000);
  char *prev = ut_alloc_1d_char (1000);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  expr[strlen (expr) - 1] = '\0';

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    sprintf (simfile, "%s.step%d)", expr, step);
    sprintf (filename, "%s/%s/%s.step%d", dir, res, res, step);

    ut_file_cp (simfile, filename);

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_addres (pSim, entity, res, NULL, 1);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&simfile);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_subres (struct SIM *pSim, char *entity, int entityqty, char *dir, char *res)
{
  int i, step;
  char *simfile = ut_alloc_1d_char (1000);
  char *filename = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  double *tmp = ut_alloc_1d (entityqty + 1);
  char *prev = ut_alloc_1d_char (1000);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);

    neut_sim_res_file (*pSim, entity, res, simfile);
    if (!ut_file_exist (simfile))
      abort ();

    sprintf (filename, "%s/%s/%s.step%d", dir, res, res, step);
    file = ut_file_open (filename, "W");

    ut_array_1d_fnscanf (simfile, tmp + 1, entityqty, "R");
    for (i = 1; i <= entityqty; i++)
      fprintf (file, "%.12f\n", tmp[i]);

    ut_file_close (file, filename, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL, 1);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&simfile);
  ut_free_1d_char (&filename);
  ut_free_1d (&tmp);
  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_eltres (struct SIM *pSim, struct TESS Tess,
                         struct NODES *pNodes, struct MESH *Mesh, char *entity,
                         int entityqty, char *dir, char *res)
{
  int i, step, colqty;
  char *simfile = ut_alloc_1d_char (1000);
  char *filename = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  char *prev = ut_alloc_1d_char (1000);
  char *type = NULL;

  if (strncmp (entity, "elset", 5))
    abort ();

  neut_sim_res_type (*pSim, "element", res, &type, &colqty);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);

    neut_sim_res_file (*pSim, "element", res, simfile);

    if (!ut_file_exist (simfile))
      abort ();

    sprintf (filename, "%s/%s/%s.step%d", dir, res, res, step);

    double **eltdata = NULL;
    double **elsetdata = NULL;

    file = ut_file_open (filename, "W");

    if (!strcmp (res, "ori"))
    {
      if (!Tess.CellCrySym)
        ut_print_message (2, 5, "\nCrystal symmetry not defined.\n");

      eltdata = ut_alloc_2d (Mesh[3].EltQty + 1, 4);
      elsetdata = ut_alloc_2d (entityqty + 1, 4);
      neut_ori_fnscanf (simfile, (*pSim).OriDes, eltdata + 1, NULL, Mesh[3].EltQty, NULL, "R");
      neut_mesh_eltdata_elsetdata_ori (*pNodes, Mesh[3], eltdata, Tess.CellCrySym, elsetdata);

      neut_ori_fprintf (file, (*pSim).OriDes, elsetdata + 1, NULL, NULL, entityqty, NULL);
    }

    else if (!strncmp (res, "oridis", 6))
    {
      double ***evect = ut_alloc_3d (Mesh[3].ElsetQty + 1, 3, 3);
      double **eval = ut_alloc_2d (Mesh[3].ElsetQty + 1, 3);

      eltdata = ut_alloc_2d (Mesh[3].EltQty + 1, 4);
      neut_ori_fnscanf (simfile, (*pSim).OriDes, eltdata + 1, NULL, Mesh[3].EltQty, NULL, "R");
      neut_mesh_eltdata_elsetdata_oridis (*pNodes, Mesh[3], eltdata, Tess.CellCrySym, evect, eval);

      for (i = 1; i <= entityqty; i++)
      {
        if (!strcmp (res, "oridisanisoangles"))
          ut_array_1d_fprintf (file, eval[i], 3, "%.12f");
        else if (!strcmp (res, "oridisanisoaxes"))
          ut_array_2d_fprintf_oneline (file, evect[i], 3, 3, "%.12f");
        else if (!strcmp (res, "oridisanisofact"))
          fprintf (file, "%.12f\n", eval[i][0] / pow (ut_array_1d_prod (eval[i], 3), 1. / 3));
      }

      ut_free_3d (&evect, Mesh[3].ElsetQty + 1, 3);
      ut_free_2d (&eval, Mesh[3].ElsetQty + 1);
    }

    else
    {
      eltdata = ut_alloc_2d (Mesh[3].EltQty + 1, colqty);
      elsetdata = ut_alloc_2d (entityqty + 1, colqty);
      ut_array_2d_fnscanf (simfile, eltdata + 1, Mesh[3].EltQty, colqty, "R");
      neut_mesh_eltdata_elsetdata (*pNodes, Mesh[3], eltdata, colqty, elsetdata);

      for (i = 1; i <= entityqty; i++)
        ut_array_1d_fprintf (file, elsetdata[i], colqty, "%.12f");
    }

    ut_file_close (file, filename, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);

    ut_free_2d (&eltdata, Mesh[3].EltQty + 1);
    ut_free_2d (&elsetdata, entityqty + 1);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL, 1);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&simfile);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&prev);
  ut_free_1d_char (&type);

  return;
}

void
nes_pproc_entity_known (struct SIM *pSim, char *entity, int entityqty,
                        char *dir, char *res)
{
  int step;
  char *simfile = ut_alloc_1d_char (1000);
  char *filename = ut_alloc_1d_char (1000);
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  if (!strcmp (entity, "node") && !strcmp (res, "disp"))
  {
    double ***coo = ut_alloc_3d (2, entityqty, 3);
    double **disp = ut_alloc_2d (entityqty, 3);

    neut_sim_setstep (pSim, 0);

    neut_sim_res_file (*pSim, "node", "coo", simfile);

    ut_array_2d_fnscanf (simfile, coo[0], entityqty, 3, "R");

    for (step = 0; step <= (*pSim).StepQty; step++)
    {
      neut_sim_setstep (pSim, step);

      neut_sim_res_file (*pSim, "node", "coo", simfile);

      if (!ut_file_exist (simfile))
        abort ();

      ut_array_2d_fnscanf (simfile, coo[1], entityqty, 3, "R");

      ut_array_2d_sub (coo[0], coo[1], entityqty, 3, disp);

      sprintf (filename, "%s/%s/%s.step%d", dir, res, res, step);

      file = ut_file_open (filename, "W");
      ut_array_2d_fprintf (file, disp, entityqty, 3, "%.12f");
      ut_file_close (file, filename, "W");

      ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
    }

    ut_free_3d (&coo, 2, entityqty);
    ut_free_2d (&disp, entityqty);
  }

  neut_sim_addres (pSim, entity, res, NULL, 1);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&simfile);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_expr (struct SIM *pSim, struct TESS Tess, struct NODES *pNodes,
                       struct MESH *Mesh, char *entity, int entityqty,
                       char *dir, char *res, char *expr)
{
  int i, step;
  double **simvals = NULL;
  char *filename = ut_alloc_1d_char (1000);
  char *simfile = ut_alloc_1d_char (1000);
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  char *entity2 = NULL;
  int varqty;
  char **vars = NULL;
  char *expression = expr ? expr : res;

  if (neut_sim_entityisnode (entity))
    ut_string_string ("node", &entity2);
  else if (neut_sim_entityiselt (entity))
    ut_string_string ("elt3d", &entity2);
  else if (neut_sim_entityiselset (entity))
    ut_string_string ("elset3d", &entity2);
  else
    abort ();

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  ut_math_vars (expression, &vars, &varqty);

  simvals = ut_alloc_2d (varqty, entityqty);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);

    int j, status;
    for (i = 0; i < varqty; i++)
    {
      if (neut_sim_res_file (*pSim, entity, vars[i], simfile) == 0)
      {
        status = ut_array_1d_fnscanf (simfile, simvals[i], entityqty, "R");
        if (status != 1)
          ut_print_message (2, 5, "\nFailed to read file `%s' (%d values vs %d entities)\n",
                            simfile, ut_file_nbwords (simfile), entityqty);
      }

      else
#pragma omp parallel for private(j,status)
        for (j = 0; j < entityqty; j++)
        {
          status = neut_mesh_var_val_one (*pNodes, Mesh[0], Mesh[1], Mesh[2],
                                          Mesh[3], Mesh[4], Tess, NULL, NULL,
                                          NULL, NULL, 0, entity2, j + 1,
                                          vars[i], simvals[i] + j, NULL);
          if (status != 0)
#ifdef HAVE_OPENMP
            if (omp_get_thread_num() == 0)
#endif
            ut_print_message (2, 5, "\nVariable `%s' could not be processed.\n", vars[i]);
        }
    }

    sprintf (filename, "%s/%s/%s.step%d", dir, res, res, step);
    file = ut_file_open (filename, "W");

    double *resval = ut_alloc_1d (entityqty);

    // this will run in multithreaded muparser
    ut_math_evals (expression, varqty, vars, simvals, entityqty, resval);

    for (i = 0; i < entityqty; i++)
      fprintf (file, "%.12f\n", resval[i]);

    ut_free_1d (&resval);

    ut_file_close (file, filename, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_addres (pSim, entity, res, expr, 1);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_2d (&simvals, varqty);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&simfile);
  ut_free_1d_char (&prev);
  ut_free_1d_char (&entity2);
  ut_free_2d_char (&vars, varqty);

  return;
}

void
nes_pproc_entity_remove (struct SIM *pSim, char *entity, char *dir, char *result)
{
  int i;
  char *tmp = ut_alloc_1d_char (1000);

  if (!neut_sim_res_exist (*pSim, entity, result + 1, NULL, NULL))
    printf ("\b\b\b\b\b\b\b\b\b\b\b does not exist\n");

  else
  {
    for (i = 0; i <= (*pSim).StepQty; i++)
    {
      sprintf (tmp, "%s/%s/%s.step%d", dir, result + 1, result + 1, i);
      remove (tmp);
    }
    sprintf (tmp, "%s/%s", dir, result + 1);
    remove (tmp);

    neut_sim_rmres (pSim, entity, result + 1);
    neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

    printf ("100%%\n");
  }

  ut_free_1d_char (&tmp);

  return;
}
