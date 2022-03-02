/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_simres_.h"

void
neut_sim_simres (struct SIM Sim, char *entity_in, char *res_in, struct SIMRES *pSimRes)
{
  int i, entityresqty;
  char *tmp = NULL, **entityres = NULL, *res = NULL, *expr = NULL;
  char *entity = NULL, *entitydir = NULL;

  neut_sim_entity_entity (entity_in, &entity);
  neut_sim_entity_entitydir (entity, &entitydir);

  ut_list_break_2 (res_in, NEUT_SEP_DEP, &res, &expr);

  neut_simres_free (pSimRes);
  neut_simres_set_zero (pSimRes);

  if (!strncmp (res, "file(", 5) && !expr)
    ut_string_string (res, &(*pSimRes).file);

  if (neut_sim_isvoid (Sim))
    return;

  (*pSimRes).step = Sim.step;

  neut_sim_entity_res (Sim, entity, &entityres, &entityresqty);

  // is it an expression?
  if (expr)
  {
    // testing whether the label is valid (not a result and not a subresut)
    for (i = 0; i < entityresqty; i++)
      if (!strcmp (entityres[i], res))
        ut_print_message (2, 4, "%s: label is a result.\n", res);

    ut_string_removetrailingdigits (res, &tmp);
    for (i = 0; i < entityresqty; i++)
      if (!strcmp (entityres[i], tmp))
        ut_print_message (2, 4, "%s: label is a subresult.\n", res);

    if (!strncmp (expr, "file(", 5))
      ut_string_string ("file", &(*pSimRes).status);
    else
      ut_string_string ("unknown", &(*pSimRes).status);

    ut_string_string (entity, &(*pSimRes).entity);
    ut_string_string (res, &(*pSimRes).res);
    ut_string_string (expr, &(*pSimRes).expr);

    (*pSimRes).dir = ut_alloc_1d_char (1000);
    sprintf ((*pSimRes).dir, "%s/results/%s/%s", Sim.simdir, entitydir,
             (*pSimRes).res);

    (*pSimRes).file = ut_alloc_1d_char (1000);
    sprintf ((*pSimRes).file, "%s/results/%s/%s/%s.step%d", Sim.simdir, entitydir,
             (*pSimRes).res, (*pSimRes).res, (*pSimRes).step);
  }

  // is it a result?
  if (!ut_string_strcmp ((*pSimRes).status, "unknown"))
  {
    for (i = 0; i < entityresqty; i++)
      if (!strcmp (entityres[i], res))
      {
        ut_string_string ("result", &(*pSimRes).status);

        ut_string_string (entity, &(*pSimRes).entity);
        ut_string_string (res, &(*pSimRes).res);

        (*pSimRes).dir = ut_alloc_1d_char (1000);
        sprintf ((*pSimRes).dir, "%s/results/%s/%s", Sim.simdir, entitydir,
                 (*pSimRes).res);
        (*pSimRes).file = ut_alloc_1d_char (1000);
        sprintf ((*pSimRes).file, "%s/results/%s/%s/%s.step%d", Sim.simdir, entitydir,
                 (*pSimRes).res, (*pSimRes).res, (*pSimRes).step);

        neut_sim_simres_type (res, pSimRes);

        break;
      }
  }

  // if not a result, is it a subresult?
  if (!ut_string_strcmp ((*pSimRes).status, "unknown"))
  {
    ut_string_removetrailingdigits (res, &tmp);

    if (strlen (tmp) < strlen (res))
      for (i = 0; i < entityresqty; i++)
        if (!strcmp (entityres[i], tmp))
        {
          ut_string_string ("subresult", &(*pSimRes).status);

          ut_string_string (entity, &(*pSimRes).entity);
          ut_string_string (res, &(*pSimRes).res);
          neut_sim_simres_parentres (res, pSimRes);

          ut_string_string ("real", &(*pSimRes).type);

          (*pSimRes).dir = ut_alloc_1d_char (1000);
          sprintf ((*pSimRes).dir, "%s/results/%s/%s", Sim.simdir, entitydir,
                   (*pSimRes).res);

          (*pSimRes).file = ut_alloc_1d_char (1000);
          sprintf ((*pSimRes).file, "%s/results/%s/%s/%s.step%d,col=%d", Sim.simdir, entitydir,
                   (*pSimRes).parentres, (*pSimRes).parentres, (*pSimRes).step, (*pSimRes).parentcol);

          (*pSimRes).newfile = ut_alloc_1d_char (1000);
          sprintf ((*pSimRes).newfile, "%s/%s.step%d", (*pSimRes).dir, (*pSimRes).res,
                    (*pSimRes).step);

          (*pSimRes).parentdir = ut_alloc_1d_char (1000);
          sprintf ((*pSimRes).parentdir, "%s/results/%s/%s", Sim.simdir, entitydir,
                   (*pSimRes).parentres);

          (*pSimRes).parentfile = ut_alloc_1d_char (1000);
          sprintf ((*pSimRes).parentfile, "%s/%s.step%d", (*pSimRes).parentdir,
                   (*pSimRes).parentres, (*pSimRes).step);

          (*pSimRes).colqty = 1;

          break;
        }
  }

  //
  if (!ut_string_strcmp ((*pSimRes).status, "unknown"))
  {
    ut_string_string (entity, &(*pSimRes).entity);
    ut_string_string (res, &(*pSimRes).res);

    (*pSimRes).dir = ut_alloc_1d_char (1000);
    sprintf ((*pSimRes).dir, "%s/results/%s/%s", Sim.simdir, entitydir,
             (*pSimRes).res);
    (*pSimRes).file = ut_alloc_1d_char (1000);
    sprintf ((*pSimRes).file, "%s/results/%s/%s/%s.step%d", Sim.simdir, entitydir,
             (*pSimRes).res, (*pSimRes).res, (*pSimRes).step);
  }

  ut_free_1d_char (&tmp);
  ut_free_2d_char (&entityres, entityresqty);
  ut_free_1d_char (&res);
  ut_free_1d_char (&entity);
  ut_free_1d_char (&entitydir);

  return;
}
