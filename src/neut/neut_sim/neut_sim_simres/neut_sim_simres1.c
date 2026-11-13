/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
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

//  if (!strncmp (res, "file(", 5) && !expr)
//    ut_string_string (res, &(*pSimRes).file);

  if (neut_sim_isvoid (Sim))
    return;

  (*pSimRes).step = Sim.step;

  neut_sim_entity_res (Sim, entity, &entityres, &entityresqty);

  // is it an expression?
  if (expr)
  {
    // testing whether the label is valid (not a result and not a subresult)
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

        ut_string_sprintf (&(*pSimRes).dir, "%s/results/%s/%s", Sim.simdir, entitydir,
             (*pSimRes).res);

        ut_string_sprintf (&(*pSimRes).file, "%s/results/%s/%s/%s.step%d", Sim.simdir, entitydir,
             (*pSimRes).res, (*pSimRes).res, (*pSimRes).step);
  }

  // if no expression provided, copying res to both res and expr
  else
  {
    ut_string_string (res, &expr);
    ut_string_string (res, &(*pSimRes).res);
    ut_string_string (expr, &(*pSimRes).expr);
  }

  // is it a result?
  if (!ut_string_strcmp ((*pSimRes).status, "unknown"))
  {
    for (i = 0; i < entityresqty; i++)
      if (!strcmp (entityres[i], res))
      {
        ut_string_string ("result", &(*pSimRes).status);

        ut_string_string (entity, &(*pSimRes).entity);

                ut_string_sprintf (&(*pSimRes).dir, "%s/results/%s/%s", Sim.simdir, entitydir,
                 (*pSimRes).res);
                ut_string_sprintf (&(*pSimRes).file, "%s/results/%s/%s/%s.step%d", Sim.simdir, entitydir,
                 (*pSimRes).res, (*pSimRes).res, (*pSimRes).step);

        neut_sim_simres_type (Sim, entity, res, pSimRes);

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

          // parent file, used to test the component against its number of columns
          ut_string_sprintf (&(*pSimRes).file, "%s/results/%s/%s/%s.step%d", Sim.simdir,
                             entitydir, tmp, tmp, (*pSimRes).step);

          neut_sim_simres_parentres (Sim, entity, res, pSimRes);

          ut_string_string ("real", &(*pSimRes).type);

                    ut_string_sprintf (&(*pSimRes).dir, "%s/results/%s/%s", Sim.simdir, entitydir,
                   (*pSimRes).res);

                    ut_string_sprintf (&(*pSimRes).file, "%s/results/%s/%s/%s.step%d,col=%d", Sim.simdir, entitydir,
                   (*pSimRes).parentres, (*pSimRes).parentres, (*pSimRes).step, (*pSimRes).parentcol);

                    ut_string_sprintf (&(*pSimRes).newfile, "%s/%s.step%d", (*pSimRes).dir, (*pSimRes).res,
                    (*pSimRes).step);

                    ut_string_sprintf (&(*pSimRes).parentdir, "%s/results/%s/%s", Sim.simdir, entitydir,
                   (*pSimRes).parentres);

                    ut_string_sprintf (&(*pSimRes).parentfile, "%s/%s.step%d", (*pSimRes).parentdir,
                   (*pSimRes).parentres, (*pSimRes).step);

          (*pSimRes).colqty = 1;

          break;
        }
  }

  //
  if (!ut_string_strcmp ((*pSimRes).status, "unknown"))
  {
    ut_string_string (entity, &(*pSimRes).entity);

        ut_string_sprintf (&(*pSimRes).dir, "%s/results/%s/%s", Sim.simdir, entitydir,
             (*pSimRes).res);
        ut_string_sprintf (&(*pSimRes).file, "%s/results/%s/%s/%s.step%d", Sim.simdir, entitydir,
             (*pSimRes).res, (*pSimRes).res, (*pSimRes).step);
  }

  ut_free_1d_char (&tmp);
  ut_free_2d_char (&entityres, entityresqty);
  ut_free_1d_char (&res);
  ut_free_1d_char (&entity);
  ut_free_1d_char (&entitydir);

  return;
}

int
neut_sim_simres_type_colqty (char *type, int *pcolqty)
{
  if (!strcmp (type, "int") || !strcmp (type, "real"))
    (*pcolqty) = 1;
  else if (!strcmp (type, "vector") || !strcmp (type, "ori"))
    (*pcolqty) = 0;
  else if (!strcmp (type, "skewtensor"))
    (*pcolqty) = 3;
  else if (!strcmp (type, "symtensor"))
    (*pcolqty) = 6;
  else if (!strcmp (type, "tensor"))
    (*pcolqty) = 9;
  else
    return -1;

  return 0;
}

// the results written by FEPX and Neper under fixed names (other results have their
// type given in their definition file, if any)
int
neut_sim_simres_knowntype (char *res, char **ptype)
{
  int i, qty;
  const char *known[][2] = {
    {"coo", "vector"}, {"disp", "vector"}, {"vel", "vector"},
    {"ori", "ori"},
    {"rss", "vector"}, {"crss", "vector"}, {"slip", "vector"}, {"sliprate", "vector"},
    {"rotrate", "vector"}, {"rotrate_spin", "vector"}, {"rotrate_slip", "vector"},
    {"stress", "symtensor"}, {"stress_eq", "real"},
    {"strain", "symtensor"}, {"strain_eq", "real"},
    {"strain_el", "symtensor"}, {"strain_el_eq", "real"},
    {"strain_pl", "symtensor"}, {"strain_pl_eq", "real"},
    {"defrate", "symtensor"}, {"defrate_eq", "real"},
    {"defrate_pl", "symtensor"}, {"defrate_pl_eq", "real"},
    {"velgrad", "tensor"}, {"spinrate", "skewtensor"},
    {"work", "real"}, {"work_pl", "real"}, {"workrate", "real"}, {"workrate_pl", "real"}
  };

  qty = sizeof (known) / sizeof (known[0]);

  for (i = 0; i < qty; i++)
    if (!strcmp (res, known[i][0]))
    {
      if (ptype)
        ut_string_string (known[i][1], ptype);
      return 0;
    }

  return -1;
}
