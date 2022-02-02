/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_gen_.h"

int
neut_sim_name_type (char *name, char **ptype, char **pname, int *prestart)
{
  int i, qty1, *qty2 = NULL, status;
  char ***parts = NULL;

  ut_list_break2 (name, NEUT_SEP_NODEP, NEUT_SEP_DEP, &parts, &qty2, &qty1);

  status = 0;
  if (pname)
    ut_string_string (name, pname);
  if (prestart)
    (*prestart) = 0;

  if (qty1 > 1)
    ut_string_string ("merge", ptype);
  else if (ut_file_exist ("%s/report", name))
    ut_string_string ("sim", ptype);
  else if (qty2[0] == 1 && ut_file_exist ("%s/post.report", name))
    ut_string_string ("fepx", ptype);
  else if (qty2[0] == 2 && ut_file_exist ("%s/post.report.rst%s", parts[0][0], parts[0][1]))
  {
    ut_string_string ("fepx", ptype);
    if (pname)
      ut_string_string (parts[0][0], pname);
    if (prestart)
      *prestart = atoi (parts[0][1]);
  }
  else if (qty2[0] == 1)
  {
    int found = 0;

    for (i = 1e3; i >= 1; i--)
      if (ut_file_exist ("%s/post.report.rst%d", parts[0][0], i))
      {
        ut_string_string ("fepx", ptype);
        if (pname)
          ut_string_string (parts[0][0], pname);
        if (prestart)
          *prestart = i;

        found = 1;
        status = 1;
        break;
      }

    if (!found)
      status = -1;
  }
  else
    status = -1;

  // ut_free_3d_char (&parts, ...);
  ut_free_1d_int (&qty2);

  return status;
}

int
neut_sim_isvoid (struct SIM Sim)
{
  return (Sim.simdir == NULL);
}

void
neut_sim_entity_entity (char *entity,  char **pentity2)
{
  char *entity_cpy = NULL;

  ut_string_string (entity, &entity_cpy);

  if (!strcmp (entity_cpy, "elt3d")
   || !strcmp (entity_cpy, "elts")
   || !strcmp (entity_cpy, "element")
   || !strcmp (entity_cpy, "elements"))
    ut_string_string ("elt", pentity2);
  else if (!strcmp (entity_cpy, "elset3d")
        || !strcmp (entity_cpy, "elsets"))
    ut_string_string ("elset", pentity2);
  else if (!strcmp (entity_cpy, "nodes"))
    ut_string_string ("node", pentity2);
  else
    ut_string_string (entity_cpy, pentity2);

  ut_free_1d_char (&entity_cpy);

  return;
}

void
neut_sim_entity_entitydir (char *entity,  char **pentitydir)
{
  if (!strcmp (entity, "node") || !strcmp (entity, "nodes"))
    ut_string_string ("nodes", pentitydir);
  else if (!strcmp (entity, "elt") || !strcmp (entity, "elts"))
    ut_string_string ("elts", pentitydir);
  else if (!strcmp (entity, "elset") || !strcmp (entity, "elsets"))
    ut_string_string ("elsets", pentitydir);
  else
    ut_string_string (entity, pentitydir);

  return;
}

int
neut_sim_entityisnode (char *entity)
{
  return !strncmp (entity, "node", 4);
}

int
neut_sim_entityiselt (char *entity)
{
  return !strncmp (entity, "element", 7) || !strncmp (entity, "elt", 3);
}

int
neut_sim_entityiselset (char *entity)
{
  return !strncmp (entity, "elset", 5);
}

int
neut_sim_entityismesh (char *entity)
{
  return !strncmp (entity, "mesh", 4);
}

void
neut_sim_res_fepxfile (struct SIM Sim, char *res, int core, char *filename)
{
  char *type = NULL, *name = NULL;

  if (!Sim.RestartId)
    sprintf (filename, "%s/post.%s.core%d", Sim.fepxdir, res, core);
  else
    sprintf (filename, "%s/post.%s.rst%d.core%d", Sim.fepxdir, res, Sim.RestartId, core);

  ut_free_1d_char (&type);
  ut_free_1d_char (&name);

  return;
}

void
neut_sim_input_files (struct SIM Sim, char ***pfullfiles, char ***pfiles, int *pfileqty)
{
  *pfileqty = 0;
  *pfullfiles = ut_alloc_1d_pchar (10);
  *pfiles = ut_alloc_1d_pchar (10);

  if (ut_file_exist ("%s/inputs/%s", Sim.simdir, Sim.tess))
  {
    if (pfullfiles)
      (*pfullfiles)[(*pfileqty)] = ut_string_paste3 (Sim.simdir, "/inputs/", Sim.tess);
    if (pfiles)
      ut_string_string (Sim.tess, (*pfiles) + (*pfileqty));
    (*pfileqty)++;
  }

  if (ut_file_exist ("%s/inputs/%s", Sim.simdir, Sim.tesr))
  {
    if (pfullfiles)
      (*pfullfiles)[(*pfileqty)] = ut_string_paste3 (Sim.simdir, "/inputs/", Sim.tesr);
    if (pfiles)
      ut_string_string (Sim.tesr, (*pfiles) + (*pfileqty));
    (*pfileqty)++;
  }

  if (ut_file_exist ("%s/inputs/%s", Sim.simdir, Sim.msh))
  {
    if (pfullfiles)
      (*pfullfiles)[(*pfileqty)] = ut_string_paste3 (Sim.simdir, "/inputs/", Sim.msh);
    if (pfiles)
      ut_string_string (Sim.msh, (*pfiles) + (*pfileqty));
    (*pfileqty)++;
  }

  if (ut_file_exist ("%s/inputs/%s", Sim.simdir, "simulation.config"))
  {
    if (pfullfiles)
      (*pfullfiles)[(*pfileqty)] = ut_string_paste3 (Sim.simdir, "/inputs/", "simulation.config");
    if (pfiles)
      ut_string_string ("simulation.config", (*pfiles) + (*pfileqty));
    (*pfileqty)++;
  }

  return;
}

int
neut_sim_entity_pos (struct SIM Sim, char *entity_in, int *ppos)
{
  int i;
  char *entity = NULL;

  if (!strcmp (entity_in, "elt3d") || !strcmp (entity_in, "elts"))
    ut_string_string ("elt", &entity);
  else if (!strcmp (entity_in, "elset3d") || !strcmp (entity_in, "elsets"))
    ut_string_string ("elset", &entity);
  else if (!strcmp (entity_in, "mesh3d") || !strcmp (entity_in, "mesh"))
    ut_string_string ("mesh", &entity);
  else
    ut_string_string (entity_in, &entity);

  *ppos = -1;
  for (i = 0; i < Sim.EntityQty; i++)
    if (!strcmp (Sim.Entities[i], entity))
    {
      *ppos = i;
      break;
    }

  ut_free_1d_char (&entity);

  return (*ppos != -1) ? 0 : -1;
}

int
neut_sim_entity_type (struct SIM Sim, char *entity_in, char **ptype)
{
  int status, pos;

  status = neut_sim_entity_pos (Sim, entity_in, &pos);

  if (!status)
    ut_string_string (Sim.EntityType[pos], ptype);

  return status;
}

void
neut_sim_entity_res (struct SIM Sim, char *entity, char ***pres, int *presqty)
{
  int i, pos;

  neut_sim_entity_pos (Sim, entity, &pos);

  if (pos == -1)
  {
    *presqty = 0;
    *pres = NULL;
  }

  else
  {
    *presqty = Sim.EntityResQty[pos];
    *pres = ut_alloc_1d_pchar (*presqty);
    for (i = 0; i < *presqty; i++)
      ut_string_string (Sim.EntityRes[pos][i], (*pres) + i);
  }

  return;
}

void
neut_sim_orispace (struct SIM Sim, char **porispace)
{
  *porispace = ut_alloc_1d_char (1000);

  if (Sim.OriSpace)
    sprintf (*porispace, "%s/orispace/%s", Sim.simdir, Sim.OriSpace);
  else
    ut_print_message (2, 2, "Failed to find orispace.\n");

  return;
}

int
neut_sim_entity_exist (struct SIM Sim, char *entity)
{
  int i;
  char *tmp = ut_alloc_1d_char (strlen (entity) + 1);

  ut_string_untilchar (entity, ':', NULL, tmp);

  for (i = 0; i < Sim.EntityQty; i++)
    if (!strcmp (tmp, Sim.Entities[i]))
      return 1;

  ut_free_1d_char (&tmp);

  return 0;
}

void
neut_sim_entity_name (char *entity, char **pname)
{
  *pname = ut_alloc_1d_char (strlen (entity) + 1);

  ut_string_untilchar (entity, ':', NULL, *pname);

  return;
}

int
neut_sim_entity_hasexpr (struct SIM Sim, char *entity)
{
  int pos;

  neut_sim_entity_pos (Sim, entity, &pos);

  return (pos == -1) ? 0 : (Sim.EntityMemberExpr[pos] != NULL);
}
