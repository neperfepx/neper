/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

void
nes_pproc_entity_pre (struct SIM *pSim, char *entity,
                      char *entityresexpr, int *pmemberqty, char **pdir,
                      char ***presults, int *presultqty)
{
  int i, pos;

  neut_sim_entity_pos (*pSim, entity, &pos);

  if (pos == -1)
  {
    neut_sim_addentity (pSim, entity);
    pos = (*pSim).EntityQty - 1;
  }

  (*pmemberqty) = (*pSim).EntityMemberQty[pos];

  (*pdir) = ut_string_paste3 ((*pSim).simdir, "/results/", entity);

  ut_list_break (entityresexpr, NEUT_SEP_NODEP, presults, presultqty);

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

  return;
}

void
nes_pproc_entity_remove (struct SIM *pSim, char *entity, char *result)
{
  int i, j;
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  neut_sim_simres (*pSim, entity, result, &SimRes);

  ut_print_message (0, 4, "%s removal ", result);
  for (j = 0; j < 41 - (int) strlen (result); j++)
    printf (".");
  printf (" ");

  if (strcmp (SimRes.status, "result"))
    printf ("\b\b\b\b missing\n");

  else
  {
    for (i = 0; i <= (*pSim).StepQty; i++)
    {
      neut_simres_setstep (&SimRes, i);
      remove (SimRes.file);
    }
    remove (SimRes.dir);

    neut_sim_rmres (pSim, entity, result);
    neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

    printf ("100%%\n");
  }

  neut_simres_free (&SimRes);

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

  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&simfile);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_subres (struct SIM *pSim, char *entity,
                         struct SIMRES *pSimRes)
{
  int i, step, pos;
  FILE *file = NULL;
  double *tmp = NULL;
  char *prev = ut_alloc_1d_char (1000);

  neut_sim_entity_pos (*pSim, entity, &pos);
  if (pos == -1)
    ut_print_neperbug ();

  tmp = ut_alloc_1d ((*pSim).EntityMemberQty[pos]);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    if (!ut_file_exist ((*pSimRes).parentfile))
      abort ();

    file = ut_file_open ((*pSimRes).newfile, "W");

    ut_array_1d_fnscanf ((*pSimRes).file, tmp, (*pSim).EntityMemberQty[pos], "R");
    for (i = 0; i < (*pSim).EntityMemberQty[pos]; i++)
      fprintf (file, "%.12f\n", tmp[i]);

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, (*pSimRes).res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d (&tmp);
  ut_free_1d_char (&prev);

  return;
}

int
nes_pproc_entity_expr (struct SIM *pSim, struct TESS Tess, struct NODES *pNodes,
                       struct MESH *Mesh, char *entity, struct SIMRES *pSimRes)
{
  int i, j, status, status2, step, varqty, pos;
  double *resval = NULL, **simvals = NULL;
  char **vars = NULL;
  char *prev = ut_alloc_1d_char (1000);
  char *expression = (*pSimRes).expr ? (*pSimRes).expr : (*pSimRes).res;
  FILE *file = NULL;
  char *parent = NULL;
  struct SIMRES SimRes2;

  status = neut_sim_entity_parent (*pSim, entity, &parent);
  if (status)
    ut_print_message (2, 3, "Entity `%s' has no parent.\n", entity);

  neut_sim_entity_pos (*pSim, entity, &pos);
  if (pos == -1)
    ut_print_neperbug ();

  neut_simres_set_zero (&SimRes2);

  ut_math_vars (expression, &vars, &varqty);

  resval = ut_alloc_1d ((*pSim).EntityMemberQty[pos]);
  simvals = ut_alloc_2d (varqty, (*pSim).EntityMemberQty[pos]);

  status = -1;

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    status = -1;

    for (i = 0; i < varqty; i++)
    {
      neut_sim_simres (*pSim, entity, vars[i], &SimRes2);

      if (ut_file_exist (SimRes2.file))
      {
        if (SimRes2.colqty != 1)
          ut_print_message (2, 5, "\nVariable `%s' is not scalar (%d value%s).\n",
                            vars[i], SimRes2.colqty, SimRes2.colqty ? "s" : "");

        status2 = ut_array_1d_fnscanf (SimRes2.file, simvals[i], (*pSim).EntityMemberQty[pos], "R");
        if (status2 != 1)
          ut_print_message (2, 5, "\nFailed to read file `%s' (%d values vs %d entities)\n",
                            SimRes2.file, ut_file_nbwords (SimRes2.file), (*pSim).EntityMemberQty[pos]);

        status = 0;
      }

      else
      {
        if (!strcmp (parent, "mesh"))
        {
          // known entity
          if (neut_mesh_entity_known (entity))
          {
            int *tmp = ut_alloc_1d_int ((*pSim).EntityMemberQty[pos]);
#pragma mp parallel for private(j)
            for (j = 0; j < (*pSim).EntityMemberQty[pos]; j++)
              tmp[j] = neut_mesh_var_val_one (*pNodes, Mesh[0], Mesh[1], Mesh[2],
                                              Mesh[3], Mesh[4], Tess, NULL, NULL,
                                              NULL, NULL, 0, entity, j + 1,
                                              vars[i], simvals[i] + j, NULL);
            status = ut_array_1d_int_sum (tmp, (*pSim).EntityMemberQty[pos]);
            ut_free_1d_int (&tmp);
          }

          // custom entity
          else
          {
            // entity type is elset
            if (!strcmp ((*pSim).EntityType[pos], "elset"))
            {
              int *tmp = ut_alloc_1d_int (Mesh[Tess.Dim].EltQty);
              double **eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 1);
              double **elsetdata = ut_alloc_2d ((*pSim).EntityMemberQty[pos] + 1, 1);

#pragma mp parallel for private(j)
              for (j = 0; j < Mesh[Tess.Dim].EltQty; j++)
                tmp[j] = neut_mesh_var_val_one (*pNodes, Mesh[0], Mesh[1], Mesh[2],
                                       Mesh[3], Mesh[4], Tess, NULL, NULL,
                                       NULL, NULL, 0, "elt", j + 1,
                                       vars[i], eltdata[j + 1], NULL);
              status = ut_array_1d_int_sum (tmp, Mesh[Tess.Dim].EltQty);
              ut_free_1d_int (&tmp);

              if (!status)
              {
#pragma mp parallel for private(j)
                for (j = 0; j < (*pSim).EntityMemberQty[pos]; j++)
                  neut_mesh_eltdata_elsetdata (*pNodes, Mesh[Tess.Dim], (*pSim).EntityMembers[pos] - 1,
                                               (*pSim).EntityMemberQty[pos],
                                               eltdata, 1, elsetdata);

                for (j = 0; j < (*pSim).EntityMemberQty[pos]; j++)
                  simvals[i][j] = elsetdata[j + 1][0];
              }

              ut_free_2d (&eltdata, Mesh[Tess.Dim].EltQty + 1);
              ut_free_2d (&elsetdata, (*pSim).EntityMemberQty[pos] + 1);
            }

            else
              abort ();
          }
        }

        else if (!strcmp (parent, "tess"))
        {
          if (1) // neut_tess_entity_known (entity))
          {
            int *tmp = ut_alloc_1d_int ((*pSim).EntityMemberQty[pos]);
#pragma mp parallel for private(j)
            for (j = 0; j < (*pSim).EntityMemberQty[pos]; j++)
              tmp[j] = neut_tess_var_val_one (Tess, NULL, NULL,
                                              NULL, entity, j + 1,
                                              vars[i], simvals[i] + j, NULL);
            status = ut_array_1d_int_sum (tmp, (*pSim).EntityMemberQty[pos]);
            ut_free_1d_int (&tmp);
          }
        }

        if (status)
          break;
      }
    }

    if (!status)
    {
      file = ut_file_open ((*pSimRes).file, "W");

      // running multiparser in multithreading
      ut_math_evals (expression, varqty, vars, simvals, (*pSim).EntityMemberQty[pos], resval);

      for (i = 0; i < (*pSim).EntityMemberQty[pos]; i++)
        fprintf (file, "%.12f\n", resval[i]);

      ut_file_close (file, (*pSimRes).file, "W");

      ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
    }

    else
      break;
  }

  if (!status)
  {
    neut_sim_addres (pSim, entity, (*pSimRes).res, (*pSimRes).expr);
    neut_sim_fprintf ((*pSim).simdir, *pSim, "W");
  }
  else
    printf ("\b\b\b\b");

  ut_free_2d (&simvals, varqty);
  ut_free_1d_char (&prev);
  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&resval);
  neut_simres_free (&SimRes2);
  ut_free_1d_char (&parent);

  return status;
}
